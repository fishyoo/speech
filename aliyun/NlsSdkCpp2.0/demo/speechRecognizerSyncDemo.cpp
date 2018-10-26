/*
 * Copyright 2015 Alibaba Group Holding Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined(_WIN32)
#include <windows.h>
#include "pthread.h"
#else
#include <unistd.h>
#include <pthread.h>
#endif


#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include "nlsClient.h"
#include "nlsEvent.h"
#include "speechRecognizerSyncRequest.h"

#define FRAME_SIZE 3200
#define SAMPLE_RATE 16000

using std::map;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;

using AlibabaNls::NlsClient;
using AlibabaNls::NlsEvent;
using AlibabaNls::LogDebug;
using AlibabaNls::LogInfo;
using AlibabaNls::AudioDataStatus;
using AlibabaNls::AUDIO_FIRST;
using AlibabaNls::AUDIO_MIDDLE;
using AlibabaNls::AUDIO_LAST;
using AlibabaNls::SpeechRecognizerSyncRequest;

// 自定义线程参数
struct ParamStruct {
	string fileName;
    string appkey;
	string token;
};

/**
    * @brief 获取sendAudio发送延时时间
    * @param dataSize 待发送数据大小
    * @param sampleRate 采样率 16k/8K
    * @param compressRate 数据压缩率，例如压缩比为10:1的16k opus编码，此时为10；非压缩数据则为1
    * @return 返回sendAudio之后需要sleep的时间
    * @note 对于8k pcm 编码数据, 16位采样，建议每发送1600字节 sleep 100 ms.
            对于16k pcm 编码数据, 16位采样，建议每发送3200字节 sleep 100 ms.
            对于其它编码格式的数据, 用户根据压缩比, 自行估算, 比如压缩比为10:1的 16k opus,
            需要每发送3200/10=320 sleep 100ms.
*/
unsigned int getSendAudioSleepTime(const int dataSize,
                                   const int sampleRate,
                                   const int compressRate) {
    // 仅支持16位采样
    const int sampleBytes = 16;
    // 仅支持单通道
    const int soundChannel = 1;

    // 当前采样率，采样位数下每秒采样数据的大小
    int bytes = (sampleRate * sampleBytes * soundChannel) / 8;

    // 当前采样率，采样位数下每毫秒采样数据的大小
    int bytesMs = bytes / 1000;

    // 待发送数据大小除以每毫秒采样数据大小，以获取sleep时间
    int sleepMs = (dataSize * compressRate) / bytesMs;

    return sleepMs;
}

// 工作线程
void* pthreadFunc(void* arg) {
    int sleepMs = 0;

    // 0: 从自定义线程参数中获取token, 音频文件等参数.
    ParamStruct* tst = (ParamStruct*)arg;
    if (tst == NULL) {
        cout << "arg is not valid." << endl;
        return NULL;
    }

    // 打开音频文件, 获取数据
	FILE* file = fopen(tst->fileName.c_str(), "rb");
	if (NULL == file) {
		cout << tst->fileName << " isn't exist." << endl;
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);  // 获取音频文件的长度
	fseek(file, 0, SEEK_SET);

    /*
    * 创建一句话同步识别SpeechRecognizerSyncRequest对象
    * request对象在一个会话周期内可以重复使用.
    * 会话周期是一个逻辑概念. 比如Demo中, 指读取, 发送完整个音频文件数据的时间.
    * 音频文件数据发送结束时, 可以releaseRecognizerSyncRequest()释放对象.
    * createRecognizerSyncRequest(), sendSyncAudio(), getRecognizerResult(), releaseRecognizerSyncRequest()请在
    * 同一线程内完成, 跨线程使用可能会引起异常错误。
	* 如果需要识别多次，请每次创建一个SpeechRecognizerSyncRequest请求，循环执行sendAudio-getRecognizerResult,
	* 然后释放SpeechRecognizerSyncRequest请求。
    */
    /*
     * 1: 创建一句话同步识别SpeechRecognizerSyncRequest对象
     */
    SpeechRecognizerSyncRequest* request = NlsClient::getInstance()->createRecognizerSyncRequest();
    if (request == NULL) {
	    cout << "createRecognizerSyncRequest failed." << endl;
		return NULL;
    }

	request->setAppKey(tst->appkey.c_str()); // 设置AppKey, 必填参数, 请参照官网申请
	request->setFormat("pcm"); // 设置音频数据编码格式, 可选参数, 目前支持pcm, opus, opu. 默认是pcm
	request->setSampleRate(SAMPLE_RATE); // 设置音频数据采样率, 可选参数, 目前支持16000, 8000. 默认是16000
	request->setIntermediateResult(true); // 设置是否返回中间识别结果, 可选参数. 默认false
	request->setPunctuationPrediction(true); // 设置是否在后处理中添加标点, 可选参数. 默认false
	request->setInverseTextNormalization(true); // 设置是否在后处理中执行ITN, 可选参数. 默认false
    request->setToken(tst->token.c_str()); // 设置账号校验token, 必填参数

	int sentSize = 0;   // 已发送的文件数据大小
	int retSize = 0;
	while (sentSize < fileSize) {
        char data[FRAME_SIZE] = {0};
		int size = fread(data, sizeof(char), sizeof(char) * FRAME_SIZE, file);
		AudioDataStatus status;
		if (sentSize == 0) {
			status = AUDIO_FIRST;  // 发送第一块音频数据
		}
		else if (sentSize + size < fileSize) {
			status = AUDIO_MIDDLE; // 发送中间音频数据
		}
		else if (sentSize + size == fileSize) {
			status = AUDIO_LAST; // 发送最后一块音频数据
		}

		sentSize += size;

        /*
        * 2: 发送音频数据. sendAudio返回-1表示发送失败, 可在getRecognizerResult函数中获得失败的具体信息
		* 对于第四个参数: request对象format参数为pcm时, 使用false即可. format为opu, 使用压缩数据时, 需设置为true.
        */
		retSize = request->sendSyncAudio(data, size, status);

        /*
        * 语音数据发送控制：
        * 语音数据是实时的, 不用sleep控制速率, 直接发送即可.
        * 语音数据来自文件, 发送时需要控制速率, 使单位时间内发送的数据大小接近单位时间原始语音数据存储的大小.
        */
		if (retSize > 0) {
			cout << "sendSyncAudio:" << retSize << endl;
			sleepMs = getSendAudioSleepTime(retSize, SAMPLE_RATE, 1); // 根据 发送数据大小，采样率，数据压缩比 来获取sleep时间
		}

        /*
        * 3: 语音数据发送延时控制
        */
#if defined(_WIN32)
        Sleep(sleepMs);
#else
        usleep(sleepMs * 1000);
#endif

		/*
		* 4: 获取识别结果
		* 接收到EventType为TaskFailed, closed, completed事件类型时，停止发送数据
		* 部分错误可收到多次TaskFailed事件，只要发生TaskFailed事件，请停止发送数据
		*/
		bool isFinished = false;
		std::queue<NlsEvent> eventQueue;
		request->getRecognizerResult(&eventQueue);
		while (!eventQueue.empty()) {
			NlsEvent _event = eventQueue.front();
			eventQueue.pop();

			NlsEvent::EventType type = _event.getMsgType();
			switch (type)
			{
			case NlsEvent::RecognitionStarted:
				cout << "************* Recognizer started *************" << endl;
				break;
			case NlsEvent::RecognitionResultChanged:
				cout << "************* Recognizer has middle result *************" << endl;
				cout << "result: " << _event.getResult() << endl;
				break;
			case NlsEvent::RecognitionCompleted:
				cout << "************* Recognizer completed *************" << endl;
				cout << "result: " << _event.getResult() << endl;
				isFinished = true;
				break;
			case NlsEvent::TaskFailed:
				cout << "************* TaskFailed *************" << endl;
				isFinished = true;
				break;
			case NlsEvent::Close:
				cout << "************* Closed *************" << endl;
				isFinished = true;
				break;
			default:
				break;
			}
			cout << "allMessage: " << _event.getAllResponse() << endl;
		}

		if (isFinished) {
			break;
		}
    }

    // 关闭音频文件
    fclose(file);

    // 5: 识别结束, 释放request对象
    NlsClient::getInstance()->releaseRecognizerSyncRequest(request);

	return NULL;
}

/** 
  * 线程循环识别
  * 需要调整count值和每次要识别的文件，Demo中默认每次识别一个文件
  */
void* multiRecognize(void* arg) {
	int count = 2;
	while (count > 0) {
		pthreadFunc(arg);
		count--;
	}

	return NULL;
}

/**
 * 识别单个音频数据
 */
int speechRecognizerFile(const char* token, const char* appkey) {

    ParamStruct pa;
    pa.token = token;
    pa.appkey = appkey;
    pa.fileName = "test0.wav";

    pthread_t pthreadId;

    // 启动一个工作线程, 用于单次识别
    pthread_create(&pthreadId, NULL, &pthreadFunc, (void *)&pa);

	// 启动一个工作线程, 用于循环识别
	// pthread_create(&pthreadId, NULL, &multiRecognize, (void *)&pa);

    pthread_join(pthreadId, NULL);
	
	return 0;

}

/**
 * 识别多个音频数据;
 * sdk多线程指一个音频数据源对应一个线程, 非一个音频数据对应多个线程.
 * 示例代码为同时开启4个线程识别4个文件;
 * 免费用户并发连接不能超过10个;
 */
#define AUDIO_FILE_NUMS 4
#define AUDIO_FILE_NAME_LENGTH 32
int speechRecognizerMultFile(const char* token, const char* appkey) {

    char audioFileNames[AUDIO_FILE_NUMS][AUDIO_FILE_NAME_LENGTH] = {"test0.wav", "test1.wav", "test2.wav", "test3.wav"};
    ParamStruct pa[AUDIO_FILE_NUMS];

    for (int i = 0; i < AUDIO_FILE_NUMS; i ++) {
        pa[i].token = token;
        pa[i].appkey = appkey;
        pa[i].fileName = audioFileNames[i];
    }

    vector<pthread_t> pthreadId(AUDIO_FILE_NUMS);
    // 启动四个工作线程, 同时识别四个音频文件
    for (int j = 0; j < AUDIO_FILE_NUMS; j++) {
        pthread_create(&pthreadId[j], NULL, &pthreadFunc, (void *)&(pa[j]));
    }

    for (int j = 0; j < AUDIO_FILE_NUMS; j++) {
        pthread_join(pthreadId[j], NULL);
    }

	return 0;
}

int main(int arc, char* argv[]) {
	if (arc < 3) {
        cout << "params is not valid. Usage: ./demo your_token your_appkey" << endl;
        return -1;
    }

    // 根据需要设置SDK输出日志, 可选. 此处表示SDK日志输出至log-recognizer.txt， LogDebug表示输出所有级别日志
    int ret = NlsClient::getInstance()->setLogConfig("log-recognizer.txt", LogInfo);
    if (-1 == ret) {
        cout << "set log failed." << endl;
        return -1;
    }

    // 识别单个音频数据
    speechRecognizerFile(argv[1], argv[2]);

    // 识别多个音频数据
    // speechRecognizerMultFile(argv[1], argv[2]);

    // 所有工作完成，进程退出前，释放nlsClient. 请注意, releaseInstance()非线程安全.
    NlsClient::releaseInstance();

	return 0;
}

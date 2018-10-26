语音智能服务C++ SDK2.0

接入前请仔细阅读C++ SDK2.0文档：https://help.aliyun.com/product/30413.html


SDK压缩包说明:

- CMakeLists.txt demo工程的CMakeList文件
- build 编译目录
- demo 包含demo.cpp，各语音服务配置文件。各文件描述见下表：

| 文件名  | 描述  |
| ------------ | ------------ |
| sdkDemo.cpp | windows专用，默认为一句话识别功能demo，如需可自行替换成其它功能(编码格式：UTF-8 代签名) |
|  speechRecognizerDemo.cpp | 一句话识别demo  |
|  speechSynthesizerDemo.cpp | 语音合成demo  |
|  speechTranscriberDemo.cpp | 实时音频流识别demo  |
| speechRecognizerSyncDemo.cpp | 一句话同步识别demo |
| speechTranscriberSyncDemo.cpp | 实时音频流同步识别demo |
|  testX.wav | 测试音频  |

- include 包含sdk头文件，以及部分第三方头文件。各文件描述见下表

| 文件名  | 描述  |
| ------------ | ------------ |
| openssl |  openssl  |
| pthread | pthread线（windows下使用） |
| uuid |  uuid(linux下使用)  |
| opus |  opus  |
| jsoncpp | jsoncpp |
| nlsClient.h | SDK实例  |
| nlsEvent.h | 回调事件说明  |
| speechRecognizerRequest.h | 一句哈识别  |
| speechSynthesizerRequest.h | 语音合成  |
| speechTranscriberRequest.h | 实时音频流识别  |
| speechRecognizerSyncRequest.h | 一句话同步识别 |
| speechTranscriberSyncRequest.h | 实时语音同步识别 |
| iNlsRequest.h | Request基础 |

- lib
  包含sdk，以及第三方依赖库。其中根据平台不同，可以选择linux版本32/64位libnlscppsdk.so(glibc2.5及以上, Gcc4, Gcc5), windows版本32/64位nlscppsdk.dll（VS2013、VS2015）
  使用前，请先解压linux.tar.gz，或windows.zip
- readme.txt SDK说明
- release.log 版本说明
- version 版本号
- build.sh demo编译脚本

注意：
1. linux环境下，运行环境最低要求：Glibc 2.5及以上， Gcc4、Gcc5
2. windows下，目前支持VS2013，VS2015


依赖库：
SDK 依赖 openssl(l-1.0.2j)，opus(1.2.1)，jsoncpp(0.10.6)，uuid(1.0.3)，pthread(2.9.1)。依赖库放置在 path/to/sdk/lib 下。
注意：path/to/sdk/lib/linux/uuid仅在linux下使用。path/to/sdk/lib/windwos/1x.0/pthread仅在windows下使用。
      SDK压缩包内提供的依赖库为64位，不提供32位。在32位下，需要用户自行编译。


Linux下demo编译过程:
cmake编译：
1: 请确认本地系统以安装Cmake，最低版本2.6
2: cd path/to/sdk/lib
3: tar -zxvpf linux.tar.gz
4: cd path/to/sdk
5: 执行./build.sh 编译demo
6: 编译完毕，进入path/to/sdk/demo目录，执行[./demo your-token your-appkey]

如果不支持cmake，可尝试手动编译:
1: cd path/to/sdk/demo
2: g++ -o srDemo speechRecognizerDemo.cpp -I../include -L../lib/linux -lnlsCppSdk -lssl -lcrypto -lopus -lpthread -luuid -ljsoncpp -D_GLIBCXX_USE_CXX11_ABI=0
2: export LD_LIBRARY_PATH=../lib/linux/
3: ./srDemo your-token your-appkey



#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_COREDEFINES_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_COREDEFINES_H

#pragma warning (disable : 4251)
#pragma warning (disable : 6031)
#pragma warning (disable : 4150)


#define _CRT_SECURE_NO_WARNINGS 0

#define USE_DEBUG 0
// #define USE_STOMP 0

/*
@ Date: 2023-12-30
@ Writer: ������
@ Explain
- ���� ����Ǵ� �ڵ��� ��Ȳ�� Window OS���� Linux OS���� �����Ѵ�.
*/
#define WINDOW_OS
// #define LINUX_OS

#ifdef WINDOW_OS
#define _WIN32_WINDOWS 0x6001
#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#else
#include <stdlib.h>
#include <sys/mman.h>
#endif

/*
@ Date: 2023-12-26
@ Writer: ������
@ Explain
- DLL ���� �����ϴ� ��ũ��, EXPORT �϶��� Exprot �ƴ� �� �����ϰ� ���ش�.
*/
#ifdef CORE_EXPORTS
#define CORE_DLL   _declspec(dllexport)
#define CACHE_ALGIN_CORE_DLL  _declspec(dllexport) __declspec(align(16))
#else
#define CORE_DLL  _declspec(dllimport)
#define CACHE_ALGIN_CORE_DLL  _declspec(dllimport) __declspec(align(16))
#endif

#ifdef _DEBUG
#pragma comment (lib, "PthMathD")
#pragma comment (lib, "gamelift-sdkd")
#else
#pragma comment (lib, "PthMath")
#pragma comment (lib, "gamelift-sdk")
#endif

#include <iostream>
#include <atomic>
#include <boost/asio.hpp>
#include <oneapi/tbb/concurrent_hash_map.h>
#include <oneapi/tbb/concurrent_priority_queue.h>
#include <oneapi/tbb/concurrent_queue.h>
#include <oneapi/tbb/concurrent_unordered_map.h>
#include <oneapi/tbb/concurrent_unordered_set.h>
#include <oneapi/tbb/concurrent_map.h>
#include <oneapi/tbb/concurrent_vector.h>
#include <oneapi/tbb/concurrent_set.h>
#include <oneapi/tbb/mutex.h>
#include <mutex>
#include <thread>
#include <string>
#include <array>
#include <concepts>
#include <chrono>
#include<concepts>
#include <random>
#include <queue>
#include <string>
#include <vector>

#include "PthMath/PthMath.h"

#include <aws/gamelift/internal/model/Message.h>

namespace Asio = boost::asio;

#include "CoreConcept.h"
#include "CoreGrobal.h"
#include "Allocator.h"

#include "CoreTypedef.h"
#include "CoreTypecast.h"
#include "CoreMacro.h"
#include "Lock.h"
#include "SmartPointer.h"

#include "CoreFunction.h"
#include "CorePacket.h"
#include "CoreEnum.h"
#include "CoreStruct.h"
#include "CoreTls.h"

#include "UCoreInstance.h"
#include "LockFree.h"
#include "UBuffer.h"

#endif 
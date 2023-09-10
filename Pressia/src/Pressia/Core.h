#pragma once

#ifdef PS_PLATFORM_WINDOWS
#ifdef PS_BUILD_DLL
#define PRESSIA_API __declspec(dllexport)
#else
#define PRESSIA_API __declspec(dllimport)
#endif // PS_BUILD_DLL
#else
#error Only Windows support for now!
#endif // PS_PLATFORM_WINDOWS

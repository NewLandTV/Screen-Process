#ifndef __DLL_H__
#define __DLL_H__

#ifndef DLL_DEVELOPMENT
#define DLL_DEFINE __declspec(dllimport)
#else
#define DLL_DEFINE __declspec(dllexport)
#endif

#endif
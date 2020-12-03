#ifndef _DLL_ERROR_HPP_
#define _DLL_ERROR_HPP_

#if BUILDING_DLL_E
#define DLL_E __declspec(dllexport)
#else
#define DLL_E __declspec(dllimport)
#endif

DLL_E void Error(const char*);
DLL_E void Status(DWORD, const char*);

#endif
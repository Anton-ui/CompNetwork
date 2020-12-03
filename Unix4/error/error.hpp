//g++ -fPIC -c error.cpp
//g++ -shared -o liberror.so error.o

#ifndef _DLL_ERROR_HPP_
#define _DLL_ERROR_HPP_

void Status(int, const char*);
void Error(const char*);

#endif

//g++ -fPIC -c sync_objs.cpp
//g++ -shared -o libsync_objs.so sync_objs.o -L../error  -lerror

#include <unistd.h>
#include <iostream>
#include "sync_objs.hpp"
#include "../error/error.hpp"

using namespace sync_objs;

static const useconds_t S = 10000;//мкс

//rotation
void rotation::enter_region(size_t process) 		/* process имеет значение 0 или 1 */ 
{ 
	while(*turn != process) usleep(S);
} 
void rotation::leave_region() 		 
{ 
	*turn = ! *turn;
}

//rotation_thread
rotation_thread::~rotation_thread() 		 
{ 
	delete turn;
}
rotation_thread::rotation_thread(size_t Count) 		 
{ 
	if(Count != 2)
        Error("Size is out of range");
	turn = new size_t();
	*turn = 0;
}

//g++ -fPIC -c sync_objs.cpp
//g++ -shared -o libsync_objs.so sync_objs.o

#ifndef _DLL_SO_HPP_
#define _DLL_SO_HPP_

namespace sync_objs 
{ 
    class rotation
	{
	protected:
		volatile size_t *turn; 		/* чья очередь? */
	public:
		void enter_region(size_t);
		void leave_region();
	};
	
	class rotation_thread : public rotation
	{
	public:
		~rotation_thread();
		rotation_thread(size_t);
	};
}
#endif

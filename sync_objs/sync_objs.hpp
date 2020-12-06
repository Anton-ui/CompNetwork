#ifndef _DLL_SO_HPP_
#define _DLL_SO_HPP_

#if BUILDING_DLL_SO
#define DLL_SO __declspec(dllexport)
#else
#define DLL_SO __declspec(dllimport)
#endif

namespace sync_objs 
{
//rotation
	class DLL_SO rotation
	{
	protected:
		volatile size_t *turn; 		/* чья очередь? */
		volatile size_t *count;
	public:
		void enter_region(size_t);
		void leave_region();
	};
	
//rotation_thread
	class DLL_SO rotation_thread : public rotation
	{
	public:
		~rotation_thread();
		rotation_thread(size_t);
	};

// Peterson's algorithm for 2 threads
	class DLL_SO critical_section_2_thread
	{
	protected:
		volatile int *interested;
		volatile int *turn;
	public:
		~critical_section_2_thread();
		critical_section_2_thread();
		void enter_region(int);
		void leave_region(int);
	};
	
// Peterson's algorithm for N threads
	class DLL_SO critical_section_n_thread
	{
	protected:
		volatile size_t *amount;
		volatile int *stage;
		volatile int *Turn;
	public:
		~critical_section_n_thread();
		critical_section_n_thread(size_t);
		void enter_region(int);
		void leave_region(int);
	};
	
// barrier
	class DLL_SO barrier
	{
	protected:
		volatile size_t *ibarrier;
		volatile size_t *count;
	public:
		void wait(size_t);
		void reset();	
	};
	
// barrier_thread
	class DLL_SO barrier_thread : public barrier
	{
		public:
			barrier_thread(size_t);
			~barrier_thread();
	};	
	
	
	//----------------processes---------------------
// rotation_process	
	class DLL_SO rotation_process : public rotation
	{
	private:
		HANDLE hMapping;
	public:
		rotation_process(const char *);
		rotation_process(const char *, size_t);
		~rotation_process();
	};
	
// Peterson's algorithm for N processes
	class DLL_SO critical_section_n_process
	{
	private:
		HANDLE hMapping;
		volatile size_t *count;
		volatile size_t *stage;
		volatile size_t *turn;
	public:
		critical_section_n_process(const char *);
		critical_section_n_process(const char *, size_t);
		~critical_section_n_process();
		void enter_region(size_t);
		void leave_region(size_t);
	};
	
// barrier_process
	class DLL_SO barrier_process : public barrier
	{
		private:
			HANDLE hMapping;
		public:
			barrier_process(const char*);
			barrier_process(const char*, size_t);
			~barrier_process();
	};	
}
#endif

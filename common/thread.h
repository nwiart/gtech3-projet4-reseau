#pragma once


class thread
{
public:

	typedef int ( *EntryPoint )( void* args );

public:

	thread( EntryPoint entry, void* args = 0 );

	~thread();

	void start();
	void stop( int exitCode = 0 );

		/// Blocks execution until thread exits.
	void wait();

		/// Returns whether this thread is still in execution.
	bool isRunning() const;

		/// Returns the thread's exit code. If the thread is still running, this function
		/// may crash, or return a random value, so make sure you call wait() or isRunning() first.
	int getExitCode() const;



private:

		/// The main start routine of the thread.
	EntryPoint m_entryPoint;

		/// The arguments given.
	void* m_threadArgs;

		/// Platform-specific object to keep track of the running thread.
		/// Ex: on Win32, this points to a HANDLE object.
	void* m_handle;
};

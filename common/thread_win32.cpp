#include "thread.h"

#include <Windows.h>



thread::thread( EntryPoint entry, void* args )
	: m_entryPoint( entry )
	, m_threadArgs( args )
	, m_handle( INVALID_HANDLE_VALUE )
{
	
}

thread::~thread()
{
	if ( m_handle != INVALID_HANDLE_VALUE ) {
		if ( this->isRunning() ) {
			TerminateThread( m_handle, 0x80000000 );
		}
		CloseHandle( m_handle );
	}
}

void thread::start()
{
	if ( m_handle != INVALID_HANDLE_VALUE && this->isRunning() ) return;

	m_handle = CreateThread(
		0, 0,
		(LPTHREAD_START_ROUTINE) m_entryPoint,
		m_threadArgs,
		0, 0 );
}

void thread::stop( int exitCode )
{
	TerminateThread( m_handle, exitCode );
	CloseHandle(m_handle);
	m_handle = INVALID_HANDLE_VALUE;
}

void thread::wait()
{
	WaitForSingleObject( m_handle, INFINITE );
}

bool thread::isRunning() const
{
	return m_handle != INVALID_HANDLE_VALUE && WaitForSingleObject(m_handle, 0);
}

int thread::getExitCode() const
{
	DWORD code = 0;
	GetExitCodeThread( m_handle, &code );

	return *((int*) &code);
}

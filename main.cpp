#include <Windows.h>
#include <process.h>
#include <iostream> 

using namespace std;

#define TIME_DELAY_THREAD_INTERVAL 1000 
#define TERMINATE_TIMEOUT 10000

HANDLE g_Thread;
HANDLE g_TerminateEvent;

static UINT __stdcall ThreadProc(LPVOID lpParam)
{
	DWORD dwRetVal = 0;

	while (TRUE) // 스레드 프록이 주기적으로 호출되도록 대기
	{
		cout << "thread proc" << endl;
		// do something
		dwRetVal = WaitForSingleObject(g_TerminateEvent, TIME_DELAY_THREAD_INTERVAL);
		if (WAIT_TIMEOUT == dwRetVal)
		{
			// 1초마다 어떤 동작을 수행
			cout << "do seomthing" << endl;
		}
		else
		{
			//cout << dwRetVal << endl;
			break;
		}
	}
	 
	_endthreadex(0);

	return dwRetVal;
}

int FunctionOn()
/*
* 함수 설명 : 스레드를 생성하고 스레드 프록을 설정한다
* 반환 값   : 함수 성공 여부 ( 0 : 성공, 그 외 : 실패)
* 
*/
{
	UINT unThreadId = 0;

	g_TerminateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (NULL == g_TerminateEvent)
	{
		return 1;
	}
	
	g_Thread = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, &unThreadId);
	if (NULL == g_Thread) // 스레드 생성 실패 시 앞서 만든 이벤트 객체 핸들을 닫아야함
	{
		CloseHandle(g_TerminateEvent);
		g_TerminateEvent = NULL;
		return 1;
	}

	return 0;
}

void FunctionOff()
{
	SetEvent(g_TerminateEvent);
	DWORD dwRetVal = WaitForSingleObject(g_Thread, TERMINATE_TIMEOUT); // 스레드가 종료되길 10 초간 기다림
	if (WAIT_OBJECT_0 == dwRetVal)
	{
		cout << "thread terminated" << endl;
		
	}

	CloseHandle(g_Thread);
	g_Thread = NULL;

	CloseHandle(g_TerminateEvent);
	g_TerminateEvent = NULL;

}

void main()
{
	FunctionOn();

	Sleep(20000);

	FunctionOff();
	
	
}

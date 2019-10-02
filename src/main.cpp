#include "MainManager.h"

int main(int argc, char** argv)
{
#if defined(_MSC_VER)
#if defined( _DEBUG )
	//Если в режиме Debug то включить контроль памяти
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDERR);
	/*
	* Set the debug-heap flag to keep freed blocks in the
	* heap's linked list - This will allow us to catch any
	* inadvertent use of freed memory
	*/
	int tmpDbgFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	tmpDbgFlag |= _CRTDBG_ALLOC_MEM_DF;
	//tmpDbgFlag |= _CRTDBG_DELAY_FREE_MEM_DF;
	tmpDbgFlag |= _CRTDBG_LEAK_CHECK_DF;
	_CrtSetDbgFlag(tmpDbgFlag);
#endif // if defined( _DEBUG )
#endif
	
	MainManager manager(argc, argv);
}


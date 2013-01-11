#include "stdafx.h"
#include "yuh/console.h"


namespace yuh
{
	console::console()
		: hStdout_(::GetStdHandle(STD_OUTPUT_HANDLE))
	{}


}
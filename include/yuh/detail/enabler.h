#pragma once
namespace yuh
{
	namespace
	{
		/**
		 * Never defined: for TMP
		 */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#endif
		extern void * enabler ; //警告されても実体作っちゃうと無駄だし宣言は必要
#ifdef __clang__
#pragma clang diagnostic pop
#endif
	}
	
}

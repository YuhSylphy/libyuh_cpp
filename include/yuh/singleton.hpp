#pragma once
#include <memory>

namespace yuh
{
	template <typename T>
	class singleton
	{
	public:
		static T& inst(){ return *inst_; }

#ifdef _MSC_VER
#define SINGLETON_EMPLACE( \
	TEMPLATE_LIST, PADDING_LIST, LIST, COMMA, X1, X2, X3, X4) \
	TEMPLATE_LIST(_CLASS_TYPE) \
	static void init_emplace(LIST(_TYPE_REFREF_ARG)) \
		{	/* insert element at beginning */ \
		inst_.reset( new T(LIST(_FORWARD_ARG))); \
		}

		/**
		 * 直に構築
		 */
		_VARIADIC_EXPAND_0X(SINGLETON_EMPLACE, , , , )
#undef SINGLETON_EMPLACE
#endif
		/** 
		 * コピーコンストラクタ使用
		 */
		inline static void init(T& val) { inst_.reset( new T(val) ); }
		/**
		 * オブジェクト解放
		 */
		inline static void reset() { inst_.reset(); }
	private:
		static std::unique_ptr<T> inst_;
	};

	template<typename T>
	std::unique_ptr<T> singleton<T>::inst_;
}
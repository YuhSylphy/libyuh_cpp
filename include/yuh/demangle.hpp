#include <cxxabi.h>

namespace yuh
{   // demangle
        std::string demangle(char const* mangled)
        {   // エラー処理とか知らない
                static int sta;
                return
                        std::unique_ptr<char, void(*)(void*)> (
                                abi::__cxa_demangle(mangled, 0, 0, &sta),
                                &free
                                ).get();
        }
        template<typename T>
        std::string demangle()
        {
                return demangle(typeid(T).name());
        }

}



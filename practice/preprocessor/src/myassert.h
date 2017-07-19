#ifndef __MY_ASSERT_H__
#define __MY_ASSERT_H__

#ifndef CONFIG_DEBUG
#define CONFIG_DEBUG 0
#endif

// TODO
#if CONFIG_DEBUG != 0
#include <iostream>
#define myassert(expr) do {\
    if (!(expr)) { \
        std::cerr << "assertion failed in " << __FILE__ << " function name: " << __func__ <<  std::endl << \
        "On line = " << __LINE__ << ". In expression " << #expr << std::endl; \
        abort(); \
    }\
} while(0, 0)
#else
#define myassert(...) do {} while(0, 0)
#endif

#endif //__MY_ASSERT_H__

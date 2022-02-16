#include <iostream>

#ifdef TALE_DEBUG_PRINT_OUTPUT
#define TALE_DEBUG_PRINT(x) std::cout << x
#else
#define TALE_DEBUG_PRINT(x)
#endif
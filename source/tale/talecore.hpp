#include <iostream>

#ifdef TALE_DEBUG_PRINT_OUTPUT
#define TALE_DEBUG_PRINT(x) std::cout << "[DEBUG][" << __TIME__ << "]\n" \
                                      << x << "\n"                       \
                                      << "----------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
#else
#define TALE_DEBUG_PRINT(x)
#endif

#ifdef TALE_VERBOSE_PRINT_OUTPUT
#define TALE_VERBOSE_PRINT(x) std::cout << "[ VERBOSE ][" << __TIME__ << "]\n" \
                                        << x
#else
#define TALE_VERBOSE_PRINT(x)
#endif

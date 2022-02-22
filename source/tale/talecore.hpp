#include "rang.hpp"

#ifdef TALE_DEBUG_PRINT_OUTPUT
#define TALE_DEBUG_PRINT(x) std::cout << rang::bg::gray << rang::fg::blue << rang::style::bold << "[ DEBUG ][" << __TIME__ << "]" \
                                      << rang::style::reset << rang::bg::reset << rang::fg::reset << "\n"                         \
                                      << x << "\n\n"

#else
#define TALE_DEBUG_PRINT(x)
#endif

#ifdef TALE_VERBOSE_PRINT_OUTPUT
#define TALE_VERBOSE_PRINT(x) std::cout << rang::bg::yellow << rang::fg::reset << rang::style::bold << "[VERBOSE][" << __TIME__ << "]" \
                                        << rang::style::reset << rang::bg::reset << rang::fg::reset << "\n"                            \
                                        << x << "\n\n"
#else
#define TALE_VERBOSE_PRINT(x)
#endif

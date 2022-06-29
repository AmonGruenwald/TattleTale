#include "rang.hpp"

#ifdef TATTLETALE_PROGRESS_PRINT_OUTPUT
#define TATTLETALE_PROGRESS_PRINT(x) std::cout << rang::style::reset << rang::bg::reset << rang::fg::green  << x << rang::fg::reset << "\r"; std::cout.flush()

#else
#define TATTLETALE_PROGRESS_PRINT(x)
#endif

#ifdef TATTLETALE_DEBUG_PRINT_OUTPUT
#define TATTLETALE_DEBUG_PRINT(x) std::cout << rang::bg::gray << rang::fg::blue << rang::style::bold << "[ DEBUG ][" << __TIME__ << "]" \
                                            << rang::style::reset << rang::bg::reset << rang::fg::reset << "\n"                         \
                                            << x << "\n\n"

#else
#define TATTLETALE_DEBUG_PRINT(x)
#endif

#ifdef TATTLETALE_VERBOSE_PRINT_OUTPUT
#define TATTLETALE_VERBOSE_PRINT(x) std::cout << rang::bg::yellow << rang::fg::reset << rang::style::bold << "[VERBOSE][" << __TIME__ << "]" \
                                              << rang::style::reset << rang::bg::reset << rang::fg::reset << "\n"                            \
                                              << x << "\n\n"
#else
#define TATTLETALE_VERBOSE_PRINT(x)
#endif

#ifdef TATTLETALE_ERROR_PRINT_OUTPUT
#define TATTLETALE_ERROR_PRINT(value, message)if (!(value)){std::cout << rang::bg::red << rang::fg::reset << rang::style::bold << "[ ERROR ][" << __TIME__ << "][" << __FILE__ << "]" << rang::style::reset << rang::bg::reset << rang::fg::reset << "\n"<< (message) << "\n\n";assert(false);}else{}
#else
#define TATTLETALE_ERROR_PRINT(value, message)
#endif

#ifndef AXIOM_ASSERT_HPP
#define AXIOM_ASSERT_HPP

// can override w/ AXIOM_ENABLE_ASSERTS=1/0 in CMake
#ifndef AXIOM_ENABLE_ASSERTS
  #ifndef NDEBUG
    #define AXIOM_ENABLE_ASSERTS 1
  #else
    #define AXIOM_ENABLE_ASSERTS 0
  #endif
#endif

#if AXIOM_ENABLE_ASSERTS

  #define AXIOM_ASSERT(cond, msg) \
    do { \
      if (!(cond)) { \
        std::cerr << "AXIOM_ASSERT failed: " << (msg) \
                  << "\n  Condition: " << #cond \
                  << "\n  File: " << __FILE__ << ":" << __LINE__ << "\n"; \
        std::abort(); \
      } \
    } while (0)

  #define AXIOM_UNREACHABLE(msg) \
    do { \
      std::cerr << "AXIOM_UNREACHABLE: " << (msg) \
                << "\n  File: " << __FILE__ << ":" << __LINE__ << "\n"; \
      std::abort(); \
    } while (0)

#else

  #define AXIOM_ASSERT(cond, msg) ((void)0)
  #define AXIOM_UNREACHABLE(msg)  ((void)0)

#endif

#endif //AXIOM_ASSERT_HPP
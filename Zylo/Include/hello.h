#pragma once

// Export/import macros for Windows
#ifdef _WIN32
#ifdef ZYLO_EXPORTS
#define ZYLO_API __declspec(dllexport)
#else
#define ZYLO_API __declspec(dllimport)
#endif
#else
    // For Linux/macOS, no __declspec needed
#define ZYLO_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

    // Public function declaration
    ZYLO_API void HelloWorld(void);

#ifdef __cplusplus
}
#endif

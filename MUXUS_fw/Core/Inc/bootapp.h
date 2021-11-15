#pragma once
#include <inttypes.h>

#if defined(__cplusplus)
extern "C" {
#endif //defined(__cplusplus)

#define BOOT_APP_MAGIC 0x55aa55aa
#define BOOT_APP_JMP_TO_SYSTEM_MEMORY 0x0001

extern uint32_t boot_app_magic;
extern uint32_t boot_app_flags;

extern void bootapp();

#if defined(__cplusplus)
}
#endif //defined(__cplusplus)

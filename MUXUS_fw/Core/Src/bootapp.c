#include "bootapp.h"
#include "main.h"

uint32_t __attribute__ ((section (".no_init"))) boot_app_magic;
uint32_t __attribute__ ((section (".no_init"))) boot_app_flags;

static void jmp_to_system_momory() {
	typedef void (*pFunction)(void);
	pFunction JumpToApplication;
	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;
	/** * Step: Disable all interrupts */
	__disable_irq();
	/* ARM Cortex-M Programming Guide to Memory Barrier Instructions.*/
	__DSB();
	__HAL_SYSCFG_REMAPMEMORY_SYSTEMFLASH();
	/* Remap is bot visible at once. Execute some unrelated command! */
	__DSB();
	__ISB();
	JumpToApplication = (void (*)(void)) (*((uint32_t *)(0x1FFF0000 + 4)));
	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) 0x1FFF0000);
	JumpToApplication();
	for(;;);
}

void bootapp() {
	if (boot_app_magic == BOOT_APP_MAGIC) {
		boot_app_magic = 0;
		if (boot_app_flags & BOOT_APP_JMP_TO_SYSTEM_MEMORY) {
			jmp_to_system_momory();
		}
	}
}

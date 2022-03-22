#include "exception_handlers.h"
#include "stdbool.h"

void Default_Handler (void) __attribute__((weak));

/* 1986BE1 Specific Interrupts */
void MIL_STD_1553B2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void MIL_STD_1553B1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void USB_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void CAN2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void DMA_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void UART1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void UART2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void SSP1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void BUSY_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ARINC429R_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void POWER_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void WWDG_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER4_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void TIMER3_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ADC_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ETHERNET_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void SSP3_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void SSP2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ARINC429T1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ARINC429T2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ARINC429T3_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void ARINC429T4_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void BKP_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXT_INT1_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXT_INT2_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXT_INT3_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));
void EXT_INT4_IRQHandler
	(void) __attribute__ ((weak, alias("Default_Handler")));

extern unsigned int __stack;

typedef void (*const pHandler)(void);

// The vector table.
// This relies on the linker script to place at correct location in memory.

pHandler __isr_vectors[] __attribute__ ((section(".isr_vector"),used)) =  {
        (pHandler) &__stack,         /* The initial stack pointer */
        Reset_Handler,               /* The reset handler */

        NMI_Handler,                 /* The NMI handler */
        HardFault_Handler,           /* The hard fault handler */

        0,							 /* Reserved */
		0,					         /* Reserved */
		0,				             /* Reserved */
        0,                           /* Reserved */
        0,                           /* Reserved */
        0,                           /* Reserved */
        0,                           /* Reserved */

        SVC_Handler,                 /* SVCall handler */

        0,					  		 /* Reserved */
        0,                           /* Reserved */

        PendSV_Handler,              /* The PendSV handler */
        SysTick_Handler,             /* The SysTick handler */

		/* External interrupts */
		MIL_STD_1553B2_IRQHandler,   /* IRQ0 */
		MIL_STD_1553B1_IRQHandler,   /* IRQ1 */
		USB_IRQHandler,              /* IRQ2 */
		CAN1_IRQHandler,             /* IRQ3 */
		CAN2_IRQHandler,             /* IRQ4 */
		DMA_IRQHandler,              /* IRQ5 */
		UART1_IRQHandler,            /* IRQ6 */
		UART2_IRQHandler,            /* IRQ7 */
		SSP1_IRQHandler,             /* IRQ8 */
		BUSY_IRQHandler,             /* IRQ9 */
		ARINC429R_IRQHandler,        /* IRQ10 */
		POWER_IRQHandler,            /* IRQ11 */
		WWDG_IRQHandler,             /* IRQ12 */
		TIMER4_IRQHandler,           /* IRQ13 */
		TIMER1_IRQHandler,           /* IRQ14 */
		TIMER2_IRQHandler,           /* IRQ15 */
		TIMER3_IRQHandler,           /* IRQ16 */
		ADC_IRQHandler,              /* IRQ17 */
		ETHERNET_IRQHandler,         /* IRQ18 */
		SSP3_IRQHandler,             /* IRQ19 */
		SSP2_IRQHandler,             /* IRQ20 */
		ARINC429T1_IRQHandler,       /* IRQ21 */
		ARINC429T2_IRQHandler,       /* IRQ22 */
		ARINC429T3_IRQHandler,       /* IRQ23 */
		ARINC429T4_IRQHandler,       /* IRQ24 */
		0,                           /* IRQ25 reserved */
		0,                           /* IRQ26 reserved */
		BKP_IRQHandler,              /* IRQ27 */
		EXT_INT1_IRQHandler,         /* IRQ28 */
		EXT_INT2_IRQHandler,         /* IRQ29 */
		EXT_INT3_IRQHandler,         /* IRQ30 */
		EXT_INT4_IRQHandler          /* IRQ31 */
};

// Processor ends up here if an unexpected interrupt occurs or a specific
// handler is not present in the application code.
__attribute__ ((section(".after_vectors")))
void Default_Handler (void)
{
	while(true) {

	}
}

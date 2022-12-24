/* Includes ------------------------------------------------------------------*/

#include "main.h"
#include <stdio.h>
#include <stdint.h>
#include "hl_hal_uart.h"

/* Private typedef -----------------------------------------------------------*/

typedef void (*pFunction)(void);

/* Private define ------------------------------------------------------------*/

#define ApplicationAddress 0x08004000  //应用程序APP, 运行的首地址

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

static pFunction Jump_To_Application;

static uint32_t JumpAddress;

/* Private  functions ---------------------------------------------------------*/

/* Exported functions --------------------------------------------------------*/

int main(void)
{
    hl_hal_uart_dbg_init();
    
    /* Configures the Internal High Speed oscillator */
    if(FLASH_HSICLOCK_DISABLE == FLASH_ClockInit()) {
        printf("HSI oscillator not yet ready\r\n");
    }

    printf("bootloader start\r\n"); 
    
    printf("bootloader jump to app, start...\r\n");


    if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000) == 0x20000000) {  // 判断栈顶地址是否在ram中,然后跳转到app
      
        hl_hal_uart_dbg_deinit();
        FLASH_Lock();

        /* Jump to user application */
        JumpAddress         = *(__IO uint32_t*)(ApplicationAddress + 4);
        Jump_To_Application = (pFunction)JumpAddress;
        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*)ApplicationAddress);
        Jump_To_Application();
    }

    return 0;
}

#ifdef USE_FULL_ASSERT

/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param file pointer to the source file name
 * @param line assert_param error line source number
 */
void assert_failed(const uint8_t* expr, const uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    rt_kprintf("Wrong parameters value: file %s on line %d\r\n", file, line);
}

#endif
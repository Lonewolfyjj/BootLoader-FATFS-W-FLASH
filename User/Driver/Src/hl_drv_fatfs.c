/**
 * @file hl_drv_fatfs.c
 * @author yijiujun (jiujun.yi@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-20
 * 
 * ██╗  ██╗ ██████╗ ██╗     ██╗  ██╗   ██╗██╗      █████╗ ███╗   ██╗██████╗ 
 * ██║  ██║██╔═══██╗██║     ██║  ╚██╗ ██╔╝██║     ██╔══██╗████╗  ██║██╔══██╗
 * ███████║██║   ██║██║     ██║   ╚████╔╝ ██║     ███████║██╔██╗ ██║██║  ██║
 * ██╔══██║██║   ██║██║     ██║    ╚██╔╝  ██║     ██╔══██║██║╚██╗██║██║  ██║
 * ██║  ██║╚██████╔╝███████╗███████╗██║   ███████╗██║  ██║██║ ╚████║██████╔╝
 * ╚═╝  ╚═╝ ╚═════╝ ╚══════╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝
 * @copyright Copyright (c) 2022 hollyland
 * 
 * @par 修改日志:
 * <table>
 * <tr><th>Date           <th>Version  <th>Author         <th>Description
 * <tr><td>2022-12-20     <td>v1.0     <td>yijiujun     <td>内容
 * </table>
 * 
 */
/* Define to prevent recursive inclusion -------------------------------------*/
/* Includes ------------------------------------------------------------------*/

#include "hl_drv_fatfs.h"
#include "n32l40x.h"
#include "string.h"
#include "stdio.h"
#include "stdint.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define debug_printf     printf

/* variables -----------------------------------------------------------------*/

static FATFS fs;    //文件系统

FIL fnew;      //文件对象

#if (FATFS_DRV_MKFS_FUN)
BYTE  work[FF_MAX_SS];          //一定是一个全局变量
#endif

/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

void hl_drv_fatfs_read(char* filename)
{
    uint8_t  path_string[20] = "0:";
    uint8_t  WriteBuffer[32] = { 0 }; /* 读缓冲区*/
    UINT      fnum;    /* 读写数量 */
    FRESULT      res_flash; /* 文件操作结果 */

    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
    //初始化函数调用流程如下
    //f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()

    res_flash = f_mount(&fs, "0:", 1);
    debug_printf("res_mount 1 = %d\r\n", res_flash);

    if (res_flash == FR_NO_FILESYSTEM) {
        debug_printf("FLASH还没有文件系统, 即将进行格式化...\r\n");

#if (FATFS_DRV_MKFS_FUN)
        /* 格式化 */
        res_flash = f_mkfs("0:", 0, work, sizeof(work));
        if (res_flash == FR_OK) {

            debug_printf("--FLASH已成功格式化文件系统。\r\n");

            /* 格式化后，先取消挂载 */
            res_flash = f_mount(RT_NULL, "0:", 1);
            debug_printf("res_demount = %d\r\n", res_flash);

            /* 重新挂载	*/
            res_flash = f_mount(&fs, "0:", 1);
            debug_printf("res_remount = %d\r\n", res_flash);

        } else {

            debug_printf("《《格式化失败。》》\r\n");
        }
#endif
    } else {
        debug_printf("\r\n****** 即将进行文件读取测试... ******\r\n");
    }

    strcat(path_string, filename);
    debug_printf("path_string = %s\r\n", path_string);

    res_flash = f_open(&fnew, path_string, FA_READ);  //只读
    debug_printf("res_open = %d\r\n", res_flash);

    if (res_flash == FR_OK) {
        debug_printf("--打开%s文件成功。\r\n", filename);

        res_flash = f_read(&fnew, WriteBuffer, sizeof(WriteBuffer), &fnum);
        debug_printf("--res_read = %d\r\n", res_flash);

        debug_printf("--文件读取成功，读到字节数据：%d\r\n", fnum);

        debug_printf("--从文件读到的数据为：\r\n%s\r\n", WriteBuffer);
        /* 不再读写，关闭文件 */
        f_close(&fnew);
        
    }
}

#if (FATFS_DRV_WRITE_FUN)
void fatfs_write_test(int argc, char* argv[])
{
    char     path_string[20] = "0:";
    char    *file_name       = argv;
    uint8_t  WriteBuffer[32] = { 0 }; /* 写缓冲区*/
    
    int      fnum; /* 文件成功读写数量 */
    FRESULT      res_flash; /* 文件操作结果 */
    //在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
    //初始化函数调用流程如下
    //f_mount()->find_volume()->disk_initialize->SPI_FLASH_Init()

    res_flash = f_mount(&fs, "0:", 1);
    debug_printf("res_mount 1 = %d\r\n", res_flash);

    if (res_flash == FR_NO_FILESYSTEM) {
        debug_printf("FLASH还没有文件系统, 即将进行格式化...\r\n");

#if (FATFS_DRV_MKFS_FUN)
        /* 格式化 */
        //res_flash = f_mkfs("0:", 0, work, sizeof(work));
        res_flash = FR_DISK_ERR;
        if (res_flash == FR_OK) {
            debug_printf("》FLASH已成功格式化文件系统。\r\n");
            /* 格式化后，先取消挂载 */
            res_flash = f_mount(RT_NULL, "0:", 1);
            debug_printf("res_demount 2 = %d\r\n", res_flash);
            /* 重新挂载	*/
            res_flash = f_mount(&fs, "0:", 1);
            debug_printf("res_mount 3 = %d\r\n", res_flash);
        } else {
            debug_printf("《《格式化失败。》》\r\n");
        }
#endif
    } else {
        debug_printf("\r\n****** 即将进行文件写入测试... ******\r\n");
    }

    strcat(path_string, file_name);  //将磁盘路径和文件名称组合成一个文件路径

    res_flash = f_open(&fnew, path_string, FA_CREATE_ALWAYS | FA_WRITE);  //创建和写入权限
    debug_printf("res_open = %d\r\n", res_flash);

    if (res_flash == FR_OK) {
        debug_printf("--打开/创建%s文件成功, 向文件写入数据。\r\n", argv);

        /* 将指定存储区内容写入到文件内 */
        res_flash = f_write(&fnew, argv[2], sizeof(argv[2]), &fnum);
        debug_printf("res_write = %d\r\n", res_flash);

        if (res_flash == FR_OK) {
            debug_printf("文件写入成功，写入字节数据：%d\r\n", fnum);

            debug_printf("向文件写入的数据为：\r\n%s\r\n", argv[2]);

        } else {
            debug_printf("！！文件写入失败：(%d)\n", res_flash);
        }
        /* 不再读写，关闭文件 */
        f_close(&fnew);
    } else {

        debug_printf("！！打开/创建文件失败。\r\n");
    }
}
#endif

void hl_drv_fatfs_del_file(char* filename)
{
    char path_string[20] = "0:";
    FRESULT      res_flash; /* 文件操作结果 */
    strcat(path_string, filename);  //将磁盘路径和文件名称组合成一个文件路径

    res_flash = f_mount(&fs, "0:", 1);
    debug_printf("res_mount = %d\r\n", res_flash);

    f_unlink(path_string);
    debug_printf("res_unlink = %d\r\n", res_flash);

    debug_printf("delete %s success\r\n", filename);
}


/*
 * EOF
 */
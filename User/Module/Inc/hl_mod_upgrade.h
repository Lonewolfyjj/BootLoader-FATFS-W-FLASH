/**
 * @file hl_mod_upgrade.h
 * @author yijiujun (jiujun.yi@hollyland-tech.com)
 * @brief 
 * @version V1.0
 * @date 2022-12-24
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
 * <tr><td>2022-12-24     <td>v1.0     <td>yijiujun     <td>内容
 * </table>
 * 
 */ 
/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _HL_MOD_UPGRADE_H_
#define _HL_MOD_UPGRADE_H_

/* Includes ------------------------------------------------------------------*/

#include "hl_drv_fatfs.h"

/* typedef -------------------------------------------------------------------*/
/* define --------------------------------------------------------------------*/

#define UPGRADE_BOX_APP_ADDRESS 0x08004000  //BOX的APP运行地址

#define UPGRADE_FILE_MAX_SIZE   102400      //升级文件大小最大值，100KB

//#define UPGRADE_FILE_NAME "12345678.txt"  //文件名长度最多8个字符，超过会无效
#define UPGRADE_FILE_NAME "A6902Box.bin"

#define UPGRADE_FUN_RET_ERR     -1
#define UPGRADE_FUN_RET_OK      0

/* variables -----------------------------------------------------------------*/
/* Private function(only *.c)  -----------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int hl_mod_upgrade_init();

int hl_mod_upgrade_deinit();

int hl_mod_upgrade_start();

#endif
/*
 * EOF
 */
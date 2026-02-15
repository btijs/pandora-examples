/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    Secure_nsclib/secure_nsc.h
 * @author  MCD Application Team
 * @brief   Header for secure non-secure callable APIs list
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* USER CODE BEGIN Non_Secure_CallLib_h */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SECURE_NSC_H
#define SECURE_NSC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/**
 * @brief  non-secure callback ID enumeration definition
 */
typedef enum {
    SECURE_FAULT_CB_ID = 0x00U, /*!< System secure fault callback ID */
    GTZC_ERROR_CB_ID = 0x01U /*!< GTZC secure error callback ID */
} SECURE_CallbackIDTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void cmse_abort(void);

int read_safe(int* ptr);
void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void* func);
int SECURE_confused_deputy_read_unsafe(int* ptr);
int SECURE_confused_deputy_read_safe(int* ptr);
void SECURE_confused_deputy_write_unsafe(int* ptr, int value);
void SECURE_confused_deputy_write_safe(int* ptr, int value);
void SECURE_confused_deputy_copy_unsafe(int* in, int* out);
void SECURE_confused_deputy_copy_safe(int* in, int* out);
void SECURE_memcpy_unsafe(int* in_start, int* in_end, int* out);
void SECURE_memcpy_safe(int* in_start, int* in_end, int* out);
void SECURE_SAU_configure_invalid();

#endif /* SECURE_NSC_H */
/* USER CODE END Non_Secure_CallLib_h */

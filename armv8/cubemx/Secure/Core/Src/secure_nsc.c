/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    Secure/Src/secure_nsc.c
 * @author  MCD Application Team
 * @brief   This file contains the non-secure callable APIs (secure world)
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

/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "secure_nsc.h"
#include "cmsis_gcc.h"
#include "main.h"
#include "stm32l552xx.h"
#include <arm_cmse.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/** @addtogroup STM32L5xx_HAL_Examples

  * @{
  */

/** @addtogroup Templates
 * @{
 */

/* Global variables ----------------------------------------------------------*/
void* pSecureFaultCallback = NULL; /* Pointer to secure fault callback in Non-secure */
void* pSecureErrorCallback = NULL; /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#define MAX_LEN 3

/**
 * @brief  Abort function for CMSE security violations
 * @retval None
 */
__attribute__((noreturn)) void cmse_abort(void)
{
    /* Trigger a secure fault or hang the system */
    while (1) {
        __asm volatile("bkpt #0"); /* Breakpoint for debugging */
    }
}

// /**
//  * @brief  Secure registration of non-secure callback.
//  * @param  CallbackId  callback identifier
//  * @param  func        pointer to non-secure function
//  * @retval None
//  */
// CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void* func)
// {
//     if (func != NULL) {
//         switch (CallbackId) {
//         case SECURE_FAULT_CB_ID: /* SecureFault Interrupt occurred */
//             pSecureFaultCallback = func;
//             break;
//         case GTZC_ERROR_CB_ID: /* GTZC Interrupt occurred */
//             pSecureErrorCallback = func;
//             break;
//         default:
//             /* unknown */
//             break;
//         }
//     }
// }

// CMSE_NS_ENTRY int read_unsafe(int* ptr)
// {
//     // You should NOT do this!
//     // You should check that ptr is non-secure before dereferencing it
//     // if (ptr < (int*)0x2000) {
//     //     cmse_abort();
//     // }
//     int value = *ptr;
//     return value;
// }

// __attribute__((cmse_nonsecure_entry)) char read_safe_char(char* ptr)
// {
//     if (cmse_TTA(ptr).flags.secure == 1) {
//         // Input pointer is secure, abort
//         return -1;
//     }
//     return *ptr;
// }

CMSE_NS_ENTRY int read_safe(int* ptr)
{
    if (cmse_nonsecure_caller() == 1 && cmse_check_pointed_object(ptr, CMSE_NONSECURE) == NULL) {
        // Caller is non-secure and input pointer is secure, abort
        cmse_abort();
    }
    int value = *ptr;
    return value;
}

// CMSE_NS_ENTRY void SECURE_confused_deputy_write_unsafe(int* ptr, int value)
// {
//     // You should NOT do this!
//     // You should check that ptr is non-secure before dereferencing it
//     *ptr = value;
// }

// CMSE_NS_ENTRY void SECURE_confused_deputy_write_safe(int* ptr, int value)
// {
//     if (cmse_nonsecure_caller() == 1
//         && cmse_check_pointed_object(ptr, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//         // Input pointer is secure, caller is non-secure, abort
//         cmse_abort();
//     }
//     *ptr = value;
// }

// CMSE_NS_ENTRY void SECURE_confused_deputy_copy_unsafe(int* in, int* out)
// {
//     // You should NOT do this!
//     // You should check that in is non-secure before dereferencing it
//     // and that out is non-secure before writing to it
//     *out = *in;
// }

// CMSE_NS_ENTRY void SECURE_confused_deputy_copy_safe(int* in, int* out)
// {
//     if (cmse_nonsecure_caller() == 1 && cmse_check_pointed_object(in, CMSE_NONSECURE) == NULL) {
//         // Input pointer is secure, caller is non-secure, abort
//         cmse_abort();
//     }
//     if (cmse_nonsecure_caller() == 1
//         && cmse_check_pointed_object(out, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//         // Output pointer is secure, caller is non-secure, abort
//         cmse_abort();
//     }
//     *out = *in;
// }

// CMSE_NS_ENTRY void SECURE_memcpy_unsafe(int* in_start, int* in_end, int* out)
// {
//     // You should NOT do this!
//     // You should check that in is non-secure before dereferencing it
//     // and that out is non-secure before writing to it
//     for (size_t i = 0; in_start < in_end && i < MAX_LEN; in_start++, i++) {
//         out[i] = *in_start;
//     }
// }

// CMSE_NS_ENTRY void SECURE_memcpy_safe(int* in_start, int* in_end, int* out)
// {
//     // start and end pointer might be unaligned
//     // e.g. in_start = 0x0; in_end = 0x3 => in_end - in_start = 0, but actual length of copied bytes
//     // is 4 bytes (1 int) so we calculate length as a byte difference by first casting to void* and
//     // round up to nearest mutliple of 4
//     size_t length = (void*)in_end - (void*)in_start;
//     length = (length + 3) & ~0x3; // Round up to nearest multiple of 4

//     void* src_chk = cmse_check_address_range(in_start, length, CMSE_NONSECURE);
//     if (src_chk == 0) {
//         cmse_abort();
//     }

//     void* dst_chk = cmse_check_address_range(out, length, CMSE_NONSECURE);
//     if (dst_chk == 0) {
//         cmse_abort();
//     }

//     for (size_t i = 0; in_start < in_end && i < MAX_LEN; in_start++, i++) {
//         out[i] = *in_start;
//     }
// }

// CMSE_NS_ENTRY void SECURE_SAU_configure_invalid()
// {
//     // You should NOT do this!
//     // The SAU configuration should only be done in secure initialization code
//     SAU->CTRL = 0xFFFFFFFF;
//     SAU->RNR = (6 & SAU_RNR_REGION_Msk);
//     SAU->RBAR = (0x50000000 & SAU_RBAR_BADDR_Msk);
//     SAU->RLAR
//         = (0x5FFFFFFF & SAU_RLAR_LADDR_Msk) | ((0x1 << SAU_RLAR_NSC_Pos) & SAU_RLAR_NSC_Msk) | 1U;
// }

// typedef CMSE_NS_CALL int nsfunc(int x);

// CMSE_NS_ENTRY int SECURE_func_call_safe(nsfunc* callback)
// {
//     cmse_address_info_t tt_payload;
//     tt_payload = cmse_TTA_fptr(callback);
//     if (tt_payload.flags.nonsecure_read_ok && ((uintptr_t)callback % 4) == 0
//         && cmse_is_nsfptr(callback)) {
//         return callback(42);
//     } else {
//         return 1; // Function pointer is not accessible from the Non-secure side
//     }
// }

// CMSE_NS_ENTRY int fun_call_safe(int (*callback)(int))
// {
//     if (cmse_is_nsfptr(callback)) {
//         // nsfunc* ns_callback = (nsfunc*)callback;
//         return ((nsfunc*)callback)(42);
//     } else {
//         return 1; // Function pointer is not accessible from the Non-secure side
//     }
// }

// CMSE_NS_ENTRY int fun_call_unsafe(int (*callback)(int)) { return callback(42); }

// CMSE_NS_ENTRY void SECURE_str_capitalize_safe(char* str)
// {
//     for (size_t i = 0; i < MAX_LEN; i++) {
//         if (cmse_nonsecure_caller() == 1
//             && cmse_check_address_range(str + i, 1, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//             cmse_abort();
//         } else if (str[i] == '\0') {
//             // Do this here instead of in the for condition to avoid reading in secure memory
//             break;
//         } else if (str[i] >= 'a' && str[i] <= 'z') {
//             str[i] += ('A' - 'a');
//         }
//     }
// }

// CMSE_NS_ENTRY void SECURE_str_capitalize_unsafe(char* str)
// {
//     for (size_t i = 0; str[i] != '\0' && i < MAX_LEN; i++) {
//         if (str[i] >= 'a' && str[i] <= 'z') {
//             str[i] += ('A' - 'a');
//         }
//     }
// }

// CMSE_NS_ENTRY int SECURE_double_pointer_safe(int** ptr)
// {
//     if (cmse_nonsecure_caller() == 1
//         && cmse_check_pointed_object(ptr, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//         cmse_abort();
//     }
//     int* inner_ptr = *ptr;
//     if (cmse_nonsecure_caller() == 1
//         && cmse_check_pointed_object(inner_ptr, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//         cmse_abort();
//     }
//     return *inner_ptr;
// }

// CMSE_NS_ENTRY int SECURE_double_pointer_unsafe(int** ptr)
// {
//     if (cmse_nonsecure_caller() == 1
//         && cmse_check_pointed_object(ptr, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//         cmse_abort();
//     }
//     if (cmse_nonsecure_caller() == 1
//         && cmse_check_pointed_object(*ptr, CMSE_NONSECURE | CMSE_MPU_READWRITE) == NULL) {
//         cmse_abort();
//     }
//     return **ptr;
// }

// CMSE_NS_ENTRY void test_bxns()
// {
//     // Store return address on stack
//     __asm volatile("push {lr}          \n");

//     // Test branching instructions
//     uintptr_t secure_addr = 0x0c000288U;
//     uintptr_t non_secure_addr = 0x080406baU;

//     uintptr_t secure_addr_lsb0 = secure_addr & ~1U; // Invalid
//     uintptr_t secure_addr_lsb1 = secure_addr | 1U; // Valid
//     uintptr_t non_secure_addr_lsb0 = non_secure_addr & ~1U; // Valid
//     uintptr_t non_secure_addr_lsb1 = non_secure_addr | 1U; // Invalid

//     // BLXNS with lsb 1 and address in S
//     __asm volatile("ldr r1, %[addr]  \n"
//                    "blxns r1            \n"
//         :
//         : [addr] "m"(non_secure_addr_lsb0)
//         : "r0", "r1");

//     // Pop return address from stack
//     __asm volatile("pop {lr}           \n");
// }

// #define GEN_TEST_FUNC(NAME, ADDR_EXPR, INSTR_TEXT)                                                 \
//     CMSE_NS_ENTRY void NAME(void)                                                                  \
//     {                                                                                              \
//         __asm volatile("push {lr}");                                                               \
//                                                                                                    \
//         uintptr_t secure_addr = 0x0c000288U;                                                       \
//         uintptr_t non_secure_addr = 0x080406baU;                                                   \
//                                                                                                    \
//         uintptr_t target = (ADDR_EXPR);                                                            \
//                                                                                                    \
//         __asm volatile("ldr r1, %[tgt]      \n" INSTR_TEXT " r1      \n"                           \
//             :                                                                                      \
//             : [tgt] "m"(target)                                                                    \
//             : "r0", "r1");                                                                         \
//                                                                                                    \
//         __asm volatile("pop {lr}");                                                                \
//     }

// #define TEST_VARIANTS(X)                                                                           \
//     X(secure, lsb0, (secure_addr & ~1U), bxns, "bxns")                                             \
//     X(secure, lsb0, (secure_addr & ~1U), blxns, "blxns")                                           \
//     X(nonsec, lsb0, (non_secure_addr & ~1U), bxns, "bxns")                                         \
//     X(nonsec, lsb0, (non_secure_addr & ~1U), blxns, "blxns")                                       \
//     X(secure, lsb1, (secure_addr | 1U), bxns, "bxns")                                              \
//     X(secure, lsb1, (secure_addr | 1U), blxns, "blxns")                                            \
//     X(nonsec, lsb1, (non_secure_addr | 1U), bxns, "bxns")                                          \
//     X(nonsec, lsb1, (non_secure_addr | 1U), blxns, "blxns")

// #define GEN_FUNC(prefix, lsb, expr, instr_ident, instr_text)                                       \
//     GEN_TEST_FUNC(test_##prefix##_##lsb##_##instr_ident, expr, instr_text);

// TEST_VARIANTS(GEN_FUNC)

// CMSE_NS_ENTRY void custom_memcpy(void* dest, void* src)
// {
//     memcpy(dest, src, 12);
//     memset(dest, (unsigned char) src, 12);
// }
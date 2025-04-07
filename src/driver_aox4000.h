/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_aox4000.h
 * @brief     driver aox4000 header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2024-11-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2024/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_AOX4000_H
#define DRIVER_AOX4000_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup aox4000_driver aox4000 driver function
 * @brief    aox4000 driver modules
 * @{
 */

/**
 * @addtogroup aox4000_basic_driver
 * @{
 */

/**
 * @brief aox4000 uart delay definition
 */
#ifndef AOX4000_UART_DELAY_MS
    #define AOX4000_UART_DELAY_MS        200        /**< 200ms */
#endif

/**
 * @brief aox4000 uart poll delay definition
 */
#ifndef AOX4000_UART_POLL_DELAY_MS
    #define AOX4000_UART_POLL_DELAY_MS        3500        /**< 3500ms */
#endif

/**
 * @brief aox4000 status enumeration definition
 */
typedef enum
{
    AOX4000_STATUS_OK              = 0x00,        /**< ok */
    AOX4000_STATUS_OVERRUN         = 0x01,        /**< overrun */
    AOX4000_STATUS_INVALID_COMMAND = 0x02,        /**< invalid command */
    AOX4000_STATUS_INVALID_FRAME   = 0x03,        /**< invalid frame */
    AOX4000_STATUS_INVALID_PARAM   = 0x04,        /**< invalid param */
    AOX4000_STATUS_UNKNOWN         = 0x05,        /**< unknown */
} aox4000_status_t;

/**
 * @brief aox4000 mode enumeration definition
 */
typedef enum
{
    AOX4000_MODE_AUTO    = 0x00,        /**< auto mode */
    AOX4000_MODE_REQUEST = 0x01,        /**< request mode */
} aox4000_mode_t;

/**
 * @brief aox4000 handle structure definition
 */
typedef struct aox4000_handle_s
{
    uint8_t (*uart_init)(void);                               /**< point to an uart_init function address */
    uint8_t (*uart_deinit)(void);                             /**< point to an uart_deinit function address */
    uint16_t (*uart_read)(uint8_t *buf, uint16_t len);        /**< point to an uart_read function address */
    uint8_t (*uart_write)(uint8_t *buf, uint16_t len);        /**< point to an uart_write function address */
    uint8_t (*uart_flush)(void);                              /**< point to an uart_flush function address */
    void (*delay_ms)(uint32_t ms);                            /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);          /**< point to a debug_print function address */
    uint8_t mode;                                             /**< mode */
    uint8_t last_error;                                       /**< last error */
    uint8_t inited;                                           /**< inited flag */
} aox4000_handle_t;

/**
 * @brief aox4000 information structure definition
 */
typedef struct aox4000_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} aox4000_info_t;

/**
 * @}
 */

/**
 * @defgroup aox4000_link_driver aox4000 link driver function
 * @brief    aox4000 link driver modules
 * @ingroup  aox4000_driver
 * @{
 */

/**
 * @brief     initialize aox4000_handle_t structure
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] STRUCTURE aox4000_handle_t
 * @note      none
 */
#define DRIVER_AOX4000_LINK_INIT(HANDLE, STRUCTURE)   memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link uart_init function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to an uart_init function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_UART_INIT(HANDLE, FUC)   (HANDLE)->uart_init = FUC

/**
 * @brief     link uart_deinit function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to an uart_deinit function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_UART_DEINIT(HANDLE, FUC) (HANDLE)->uart_deinit = FUC

/**
 * @brief     link uart_read function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to an uart_read function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_UART_READ(HANDLE, FUC)   (HANDLE)->uart_read = FUC

/**
 * @brief     link uart_write function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to an uart_write function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_UART_WRITE(HANDLE, FUC)  (HANDLE)->uart_write = FUC

/**
 * @brief     link uart_flush function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to an uart_flush function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_UART_FLUSH(HANDLE, FUC)  (HANDLE)->uart_flush = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to a delay_ms function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_DELAY_MS(HANDLE, FUC)    (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE pointer to an aox4000 handle structure
 * @param[in] FUC pointer to a debug_print function address
 * @note      none
 */
#define DRIVER_AOX4000_LINK_DEBUG_PRINT(HANDLE, FUC) (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup aox4000_basic_driver aox4000 basic driver function
 * @brief    aox4000 basic driver modules
 * @ingroup  aox4000_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an aox4000 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t aox4000_info(aox4000_info_t *info);

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to an aox4000 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t aox4000_init(aox4000_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle pointer to an aox4000 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t aox4000_deinit(aox4000_handle_t *handle);

/**
 * @brief      read data
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *oxygen_ppo2_mbar pointer to a oxygen ppo2 mbar buffer
 * @param[out] *oxygen_density_percentage pointer to an oxygen density percentage buffer
 * @param[out] *temperature_degree pointer to a temperature degree buffer
 * @param[out] *pressure_mbar pointer to a pressure mbar buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_read(aox4000_handle_t *handle, float *oxygen_ppo2_mbar,
                     float *oxygen_density_percentage,
                     float *temperature_degree, float *pressure_mbar);

/**
 * @brief      read oxygen ppo2
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *mbar pointer to a oxygen ppo2 mbar buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_read_oxygen_ppo2(aox4000_handle_t *handle, float *mbar);

/**
 * @brief      read oxygen density
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *percentage pointer to an oxygen density percentage buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_read_oxygen_density(aox4000_handle_t *handle, float *percentage);

/**
 * @brief      read temperature
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *degree pointer to a temperature degree buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_read_temperature(aox4000_handle_t *handle, float *degree);

/**
 * @brief      read pressure
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *mbar pointer to a pressure mbar buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_read_pressure(aox4000_handle_t *handle, float *mbar);

/**
 * @brief     set mode
 * @param[in] *handle pointer to an aox4000 handle structure
 * @param[in] mode chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t aox4000_set_mode(aox4000_handle_t *handle, aox4000_mode_t mode);

/**
 * @brief      get mode
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *mode pointer to a chip mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_get_mode(aox4000_handle_t *handle, aox4000_mode_t *mode);

/**
 * @brief      get last error
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_get_last_error(aox4000_handle_t *handle, aox4000_status_t *status);

/**
 * @}
 */

/**
 * @defgroup aox4000_extern_driver aox4000 extern driver function
 * @brief    aox4000 extern driver modules
 * @ingroup  aox4000_driver
 * @{
 */

/**
 * @brief      command read
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len buffer length
 * @return     status code
 *             - 0 success
 *             - 1 command read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t aox4000_command_read(aox4000_handle_t *handle, char *buf, uint16_t len);

/**
 * @brief     command write
 * @param[in] *handle pointer to an aox4000 handle structure
 * @param[in] *buf pointer to a data buffer
 * @param[in] len buffer length
 * @return    status code
 *            - 0 success
 *            - 1 command write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t aox4000_command_write(aox4000_handle_t *handle, char *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

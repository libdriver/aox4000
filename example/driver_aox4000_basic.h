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
 * @file      driver_aox4000_basic.h
 * @brief     driver aox4000 basic header file
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

#ifndef DRIVER_AOX4000_BASIC_H
#define DRIVER_AOX4000_BASIC_H

#include "driver_aox4000_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup aox4000_example_driver aox4000 example driver function
 * @brief    aox4000 example driver modules
 * @ingroup  aox4000_driver
 * @{
 */

/**
 * @brief aox4000 basic example default definition
 */
#define AOX4000_BASIC_DEFAULT_MODE        AOX4000_MODE_REQUEST        /**< request mode */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t aox4000_basic_init(void);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t aox4000_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *oxygen_ppo2_mbar pointer to a oxygen ppo2 mbar buffer
 * @param[out] *oxygen_density_percentage pointer to an oxygen density percentage buffer
 * @param[out] *temperature_degree pointer to a temperature degree buffer
 * @param[out] *pressure_mbar pointer to a pressure mbar buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t aox4000_basic_read(float *oxygen_ppo2_mbar, float *oxygen_density_percentage,
                           float *temperature_degree, float *pressure_mbar);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif

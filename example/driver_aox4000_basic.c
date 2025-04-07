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
 * @file      driver_aox4000_basic.c
 * @brief     driver aox4000 basic source file
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

#include "driver_aox4000_basic.h"

static aox4000_handle_t gs_handle;        /**< aox4000 handle */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t aox4000_basic_init(void)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_AOX4000_LINK_INIT(&gs_handle, aox4000_handle_t);
    DRIVER_AOX4000_LINK_UART_INIT(&gs_handle, aox4000_interface_uart_init);
    DRIVER_AOX4000_LINK_UART_DEINIT(&gs_handle, aox4000_interface_uart_deinit);
    DRIVER_AOX4000_LINK_UART_READ(&gs_handle, aox4000_interface_uart_read);
    DRIVER_AOX4000_LINK_UART_WRITE(&gs_handle, aox4000_interface_uart_write);
    DRIVER_AOX4000_LINK_UART_FLUSH(&gs_handle, aox4000_interface_uart_flush);
    DRIVER_AOX4000_LINK_DELAY_MS(&gs_handle, aox4000_interface_delay_ms);
    DRIVER_AOX4000_LINK_DEBUG_PRINT(&gs_handle, aox4000_interface_debug_print);
    
    /* aox4000 init */
    res = aox4000_init(&gs_handle);
    if (res != 0)
    {
        aox4000_interface_debug_print("aox4000: init failed.\n");
        
        return 1;
    }
    
    /* set default mode */
    res = aox4000_set_mode(&gs_handle, AOX4000_BASIC_DEFAULT_MODE);
    if (res != 0)
    {
        aox4000_interface_debug_print("aox4000: set mode failed.\n");
        (void)aox4000_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

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
                           float *temperature_degree, float *pressure_mbar)
{
    uint8_t res;
    
    /* read */
    res = aox4000_read(&gs_handle, oxygen_ppo2_mbar, oxygen_density_percentage, 
                      temperature_degree, pressure_mbar);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t aox4000_basic_deinit(void)
{
    /* deinit aox4000 */
    if (aox4000_deinit(&gs_handle) != 0)
    {
        return 1;
    }
    
    return 0;
}

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
 * @file      driver_aox4000_read_test.c
 * @brief     driver aox4000 read test source file
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

#include "driver_aox4000_read_test.h"

static aox4000_handle_t gs_handle;        /**< aox4000 handle */

/**
 * @brief     read test
 * @param[in] times test times
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t aox4000_read_test(uint32_t times)
{
    uint8_t res;
    uint32_t i;
    aox4000_info_t info;
    
    /* link interface function */
    DRIVER_AOX4000_LINK_INIT(&gs_handle, aox4000_handle_t);
    DRIVER_AOX4000_LINK_UART_INIT(&gs_handle, aox4000_interface_uart_init);
    DRIVER_AOX4000_LINK_UART_DEINIT(&gs_handle, aox4000_interface_uart_deinit);
    DRIVER_AOX4000_LINK_UART_READ(&gs_handle, aox4000_interface_uart_read);
    DRIVER_AOX4000_LINK_UART_WRITE(&gs_handle, aox4000_interface_uart_write);
    DRIVER_AOX4000_LINK_UART_FLUSH(&gs_handle, aox4000_interface_uart_flush);
    DRIVER_AOX4000_LINK_DELAY_MS(&gs_handle, aox4000_interface_delay_ms);
    DRIVER_AOX4000_LINK_DEBUG_PRINT(&gs_handle, aox4000_interface_debug_print);
    
    /* get aox4000 information */
    res = aox4000_info(&info);
    if (res != 0)
    {
        aox4000_interface_debug_print("aox4000: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        aox4000_interface_debug_print("aox4000: chip is %s.\n", info.chip_name);
        aox4000_interface_debug_print("aox4000: manufacturer is %s.\n", info.manufacturer_name);
        aox4000_interface_debug_print("aox4000: interface is %s.\n", info.interface);
        aox4000_interface_debug_print("aox4000: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        aox4000_interface_debug_print("aox4000: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        aox4000_interface_debug_print("aox4000: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        aox4000_interface_debug_print("aox4000: max current is %0.2fmA.\n", info.max_current_ma);
        aox4000_interface_debug_print("aox4000: max temperature is %0.1fC.\n", info.temperature_max);
        aox4000_interface_debug_print("aox4000: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* aox4000 init */
    res = aox4000_init(&gs_handle);
    if (res != 0)
    {
        aox4000_interface_debug_print("aox4000: init failed.\n");

        return 1;
    }
    
    /* start read test */
    aox4000_interface_debug_print("aox4000: start read test.\n");
    
    /* auto mode read test */
    aox4000_interface_debug_print("aox4000: auto mode read test.\n");
    
    /* set auto mode */
    res = aox4000_set_mode(&gs_handle, AOX4000_MODE_AUTO);
    if (res != 0)
    {
        aox4000_interface_debug_print("aox4000: set mode failed.\n");
        (void)aox4000_deinit(&gs_handle);
        
        return 1;
    }
    
    /* read test */
    aox4000_interface_debug_print("aox4000: read test.\n");
    
    for (i = 0; i < times; i++)
    {
        float oxygen_ppo2_mbar;
        float oxygen_density_percentage;
        float temperature_degree;
        float pressure_mbar;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read(&gs_handle,
                          &oxygen_ppo2_mbar,
                          &oxygen_density_percentage,
                          &temperature_degree, 
                          &pressure_mbar);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
        aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
        aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
        aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
    }
    
    /* read oxygen ppo2 test */
    aox4000_interface_debug_print("aox4000: read oxygen ppo2 test.\n");
    
    for (i = 0; i < times; i++)
    {
        float oxygen_ppo2_mbar;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_oxygen_ppo2(&gs_handle, &oxygen_ppo2_mbar);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
    }
    
    /* read oxygen density test */
    aox4000_interface_debug_print("aox4000: read oxygen density test.\n");
    
    for (i = 0; i < times; i++)
    {
        float oxygen_density_percentage;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_oxygen_density(&gs_handle, &oxygen_density_percentage);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
    }
    
    /* read temperature test */
    aox4000_interface_debug_print("aox4000: read temperature test.\n");
    
    for (i = 0; i < times; i++)
    {
        float temperature_degree;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_temperature(&gs_handle, &temperature_degree);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
    }
    
    /* read pressure test */
    aox4000_interface_debug_print("aox4000: read pressure test.\n");
    
    for (i = 0; i < times; i++)
    {
        float pressure_mbar;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_pressure(&gs_handle, &pressure_mbar);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
    }
    
    /* request mode read test */
    aox4000_interface_debug_print("aox4000: request mode read test.\n");
    
    /* set request mode */
    res = aox4000_set_mode(&gs_handle, AOX4000_MODE_REQUEST);
    if (res != 0)
    {
        aox4000_interface_debug_print("aox4000: set mode failed.\n");
        (void)aox4000_deinit(&gs_handle);
        
        return 1;
    }
    
    /* read test */
    aox4000_interface_debug_print("aox4000: read test.\n");
    
    for (i = 0; i < times; i++)
    {
        float oxygen_ppo2_mbar;
        float oxygen_density_percentage;
        float temperature_degree;
        float pressure_mbar;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read(&gs_handle,
                          &oxygen_ppo2_mbar,
                          &oxygen_density_percentage,
                          &temperature_degree, 
                          &pressure_mbar);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
        aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
        aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
        aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
    }
    
    /* read oxygen ppo2 test */
    aox4000_interface_debug_print("aox4000: read oxygen ppo2 test.\n");
    
    for (i = 0; i < times; i++)
    {
        float oxygen_ppo2_mbar;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_oxygen_ppo2(&gs_handle, &oxygen_ppo2_mbar);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
    }
    
    /* read oxygen density test */
    aox4000_interface_debug_print("aox4000: read oxygen density test.\n");
    
    for (i = 0; i < times; i++)
    {
        float oxygen_density_percentage;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_oxygen_density(&gs_handle, &oxygen_density_percentage);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
    }
    
    /* read temperature test */
    aox4000_interface_debug_print("aox4000: read temperature test.\n");
    
    for (i = 0; i < times; i++)
    {
        float temperature_degree;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_temperature(&gs_handle, &temperature_degree);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
    }
    
    /* read pressure test */
    aox4000_interface_debug_print("aox4000: read pressure test.\n");
    
    for (i = 0; i < times; i++)
    {
        float pressure_mbar;
        
        /* delay 1000ms */
        aox4000_interface_delay_ms(1000);
        
        /* read */
        res = aox4000_read_pressure(&gs_handle, &pressure_mbar);
        if (res != 0)
        {
            aox4000_interface_debug_print("aox4000: read failed.\n");
            (void)aox4000_deinit(&gs_handle);
            
            return 1;
        }
        
        /* output */
        aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
    }
    
    /* finish read test */
    aox4000_interface_debug_print("aox4000: finish read test.\n");
    (void)aox4000_deinit(&gs_handle);

    return 0;
}

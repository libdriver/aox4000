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
 * @file      main.c
 * @brief     main source file
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
#include "driver_aox4000_read_test.h"
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief     aox4000 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t aox4000(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"times", required_argument, NULL, 1},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* times */
            case 1 :
            {
                /* set times */
                times = atoi(optarg);
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (aox4000_read_test(times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
        uint8_t res;
        uint32_t i;
        
        res = aox4000_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        for (i = 0; i < times; i++)
        {
            float oxygen_ppo2_mbar;
            float oxygen_density_percentage;
            float temperature_degree;
            float pressure_mbar;
            
            /* delay 1000ms */
            aox4000_interface_delay_ms(1000);
            
            /* read data */
            res = aox4000_basic_read(&oxygen_ppo2_mbar, &oxygen_density_percentage,
                                    &temperature_degree, &pressure_mbar);
            if (res != 0)
            {
                (void)aox4000_basic_deinit();
                
                return 1;
            }
            
            /* output */
            aox4000_interface_debug_print("aox4000: %d/%d.\n", i + 1, times);
            aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
            aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
            aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
            aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
        }
        
        /* deinit */
        (void)aox4000_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        aox4000_interface_debug_print("Usage:\n");
        aox4000_interface_debug_print("  aox4000 (-i | --information)\n");
        aox4000_interface_debug_print("  aox4000 (-h | --help)\n");
        aox4000_interface_debug_print("  aox4000 (-p | --port)\n");
        aox4000_interface_debug_print("  aox4000 (-t read | --test=read) [--times=<num>]\n");
        aox4000_interface_debug_print("  aox4000 (-e read | --example=read) [--times=<num>]\n");
        aox4000_interface_debug_print("\n");
        aox4000_interface_debug_print("Options:\n");
        aox4000_interface_debug_print("  -e <read>, --example=<read>        Run the driver example.\n");
        aox4000_interface_debug_print("  -h, --help                         Show the help.\n");
        aox4000_interface_debug_print("  -i, --information                  Show the chip information.\n");
        aox4000_interface_debug_print("  -p, --port                         Display the pins used by this device to connect the chip.\n");
        aox4000_interface_debug_print("  -t <read>, --test=<read>           Run the driver test.\n");
        aox4000_interface_debug_print("      --times=<num>                  Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        aox4000_info_t info;
        
        /* print aox4000 info */
        aox4000_info(&info);
        aox4000_interface_debug_print("aox4000: chip is %s.\n", info.chip_name);
        aox4000_interface_debug_print("aox4000: manufacturer is %s.\n", info.manufacturer_name);
        aox4000_interface_debug_print("aox4000: interface is %s.\n", info.interface);
        aox4000_interface_debug_print("aox4000: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        aox4000_interface_debug_print("aox4000: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        aox4000_interface_debug_print("aox4000: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        aox4000_interface_debug_print("aox4000: max current is %0.2fmA.\n", info.max_current_ma);
        aox4000_interface_debug_print("aox4000: max temperature is %0.1fC.\n", info.temperature_max);
        aox4000_interface_debug_print("aox4000: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        aox4000_interface_debug_print("aox4000: TX connected to GPIO15(BCM).\n");
        aox4000_interface_debug_print("aox4000: RX connected to GPIO14(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = aox4000(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        aox4000_interface_debug_print("aox4000: run failed.\n");
    }
    else if (res == 5)
    {
        aox4000_interface_debug_print("aox4000: param is invalid.\n");
    }
    else
    {
        aox4000_interface_debug_print("aox4000: unknown status code.\n");
    }

    return 0;
}

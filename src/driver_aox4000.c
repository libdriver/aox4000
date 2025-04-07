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
 * @file      driver_aox4000.c
 * @brief     driver aox4000 source file
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

#include "driver_aox4000.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "ASAIR AOX4000"       /**< chip name */
#define MANUFACTURER_NAME         "ASAIR"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        4.75f                 /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.25f                 /**< chip max supply voltage */
#define MAX_CURRENT               32.0f                 /**< chip max current */
#define TEMPERATURE_MIN           -30.0f                /**< chip min operating temperature */
#define TEMPERATURE_MAX           60.0f                 /**< chip max operating temperature */
#define DRIVER_VERSION            1000                  /**< driver version */

/**
 * @brief chip command definition
 */
#define AOX4000_COMMAND_MODE                  "M"        /**< mode command */
#define AOX4000_COMMAND_OXYGEN_PPO2           "O"        /**< oxygen ppo2 command */
#define AOX4000_COMMAND_OXYGEN_DENSITY        "%"        /**< oxygen density command */
#define AOX4000_COMMAND_TEMPERATURE           "T"        /**< temperature command */
#define AOX4000_COMMAND_PRESSURE              "P"        /**< pressure command */
#define AOX4000_COMMAND_ALL                   "A"        /**< all command */
#define AOX4000_COMMAND_ERROR                 "E"        /**< error command */

/**
 * @brief         uart write and read
 * @param[in]     *handle pointer to an aox4000 handle structure
 * @param[in]     *input pointer to an input buffer
 * @param[in]     input_len input length
 * @param[out]    *output pointer to an output buffer
 * @param[in,out] *output_len pointer to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 write read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 * @note          none
 */
static uint8_t a_aox4000_write_read(aox4000_handle_t *handle, char *input, uint16_t input_len, char *output, uint16_t *output_len)
{
    uint8_t res;
    
    res = handle->uart_flush();                                             /* flush */
    if (res != 0)                                                           /* check result */
    {
        return 1;                                                           /* return error */
    }
    res = handle->uart_write((uint8_t *)input, input_len);                  /* write data */
    if (res != 0)                                                           /* check result */
    {
        return 1;                                                           /* return error */
    }
    handle->delay_ms(AOX4000_UART_DELAY_MS);                                /* delay */
    *output_len = handle->uart_read((uint8_t *)output, *output_len);        /* read data */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief         uart read
 * @param[in]     *handle pointer to an aox4000 handle structure
 * @param[out]    *output pointer to an output buffer
 * @param[in,out] *output_len pointer to a length buffer
 * @return        status code
 *                - 0 success
 *                - 1 read failed
 *                - 2 handle is NULL
 *                - 3 handle is not initialized
 * @note          none
 */
static uint8_t a_aox4000_read(aox4000_handle_t *handle, char *output, uint16_t *output_len)
{
    uint8_t res;
    
    res = handle->uart_flush();                                             /* flush */
    if (res != 0)                                                           /* check result */
    {
        return 1;                                                           /* return error */
    }
    handle->delay_ms(AOX4000_UART_POLL_DELAY_MS);                           /* delay */
    *output_len = handle->uart_read((uint8_t *)output, *output_len);        /* read data */
    
    return 0;                                                               /* success return 0 */
}

/**
 * @brief      parse mode
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[in]  *input pointer to an input buffer
 * @param[out] *mode pointer to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 parse mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
static uint8_t aox4000_parse_mode(aox4000_handle_t *handle, char *input, uint8_t *mode)
{
    if (strstr(input, AOX4000_COMMAND_MODE) != NULL)            /* check m */
    {
        char buf[8];
        
        memset(buf, 0, sizeof(char) * 8);                       /* clear buffer */
        buf[0] = input[2];                                      /* set buf[0] */
        buf[1] = input[3];                                      /* set buf[1] */
        handle->last_error = (uint8_t)AOX4000_STATUS_OK;        /* set last error */
        *mode = (uint8_t)atoi(buf);                             /* convert mode */
        
        return 0;                                               /* success return 0 */
    }
    if (strstr(input, AOX4000_COMMAND_ERROR) != NULL)           /* check e */
    {
        char buf[8];
        
        memset(buf, 0, sizeof(char) * 8);                       /* clear buffer */
        buf[0] = input[2];                                      /* set buf[0] */
        buf[1] = input[3];                                      /* set buf[1] */
        handle->last_error = (uint8_t)atoi(buf);                /* set last error */
        
        return 1;                                               /* return error */
    }
    
    handle->last_error = (uint8_t)AOX4000_STATUS_UNKNOWN;       /* set unknown */
    
    return 1;                                                   /* return error */
}

/**
 * @brief      parse number
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[in]  *input pointer to an input buffer
 * @param[out] *number pointer to a number buffer
 * @return     status code
 *             - 0 success
 *             - 1 parse number failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
static uint8_t aox4000_parse_number(aox4000_handle_t *handle, const char *command, char *input, float *number)
{
    if (strstr(input, command) != NULL)                         /* check command */
    {
        uint8_t i;
        char buf[16];
        
        memset(buf, 0, sizeof(char) * 16);                      /* clear buffer */
        for (i = 0; i < 15; i++)                                /* copy all */
        {
            if (((input[2 + i] >= '0') && 
                 (input[2 + i] <= '9')) ||
                 (input[2 + i] == '.') ||
                 (input[2 + i] == '+') ||
                 (input[2 + i] == '-'))                         /* check end */
            {
                buf[i] = input[2 + i];                          /* save to buffer */
            }
            else
            {
                break;                                          /* break */
            }
        }
        handle->last_error = (uint8_t)AOX4000_STATUS_OK;        /* set last error */
        *number = (float)atof(buf);                             /* convert mode */
        
        return 0;                                               /* success return 0 */
    }
    if (strstr(input, AOX4000_COMMAND_ERROR) != NULL)           /* check e */
    {
        char buf[8];
        
        memset(buf, 0, sizeof(char) * 8);                       /* clear buffer */
        buf[0] = input[2];                                      /* set buf[0] */
        buf[1] = input[3];                                      /* set buf[1] */
        handle->last_error = (uint8_t)atoi(buf);                /* set last error */
        
        return 1;                                               /* return error */
    }
    
    handle->last_error = (uint8_t)AOX4000_STATUS_UNKNOWN;       /* set unknown */
    
    return 1;                                                   /* return error */
}

/**
 * @brief      parse data
 * @param[in]  *handle pointer to an aox4000 handle structure
 * @param[in]  *input pointer to an input buffer
 * @param[out] *oxygen_ppo2_mbar pointer to a oxygen ppo2 mbar buffer
 * @param[out] *oxygen_density_percentage pointer to an oxygen density percentage buffer
 * @param[out] *temperature_degree pointer to a temperature degree buffer
 * @param[out] *pressure_mbar pointer to a pressure mbar buffer
 * @return     status code
 *             - 0 success
 *             - 1 parse data failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
static uint8_t aox4000_parse_data(aox4000_handle_t *handle, char *input, float *oxygen_ppo2_mbar,
                                 float *oxygen_density_percentage, float *temperature_degree, float *pressure_mbar)
{
    uint8_t res;
    char *p;
    
    if (strstr(input, AOX4000_COMMAND_ERROR) != NULL)                         /* check e */
    {
        char buf[8];
        
        memset(buf, 0, sizeof(char) * 8);                                     /* clear buffer */
        buf[0] = input[2];                                                    /* set buf[0] */
        buf[1] = input[3];                                                    /* set buf[1] */
        handle->last_error = (uint8_t)atoi(buf);                              /* set last error */
        
        return 1;                                                             /* return error */
    }
    
    p = strstr(input, AOX4000_COMMAND_OXYGEN_PPO2);                           /* find oxygen ppo2 */
    if (p != NULL)                                                            /* check oxygen ppo2 */
    {
        res = aox4000_parse_number(handle, AOX4000_COMMAND_OXYGEN_PPO2, 
                                  p, oxygen_ppo2_mbar);                       /* parse number*/
        if (res != 0)                                                         /* check result */
        {
            return 1;                                                         /* return error */
        }
    }
    else
    {
        return 1;                                                             /* return error */
    }
    
    p = strstr(input, AOX4000_COMMAND_OXYGEN_DENSITY);                        /* find oxygen density */
    if (p != NULL)                                                            /* check oxygen density */
    {
        res = aox4000_parse_number(handle, AOX4000_COMMAND_OXYGEN_DENSITY, 
                                  p, oxygen_density_percentage);              /* parse number*/
        if (res != 0)                                                         /* check result */
        {
            return 1;                                                         /* return error */
        }
    }
    else
    {
        return 1;                                                             /* return error */
    }
    
    p = strstr(input, AOX4000_COMMAND_TEMPERATURE);                           /* find temperature */
    if (p != NULL)                                                            /* check temperature */
    {
        res = aox4000_parse_number(handle, AOX4000_COMMAND_TEMPERATURE, 
                                  p, temperature_degree);                     /* parse number*/
        if (res != 0)                                                         /* check result */
        {
            return 1;                                                         /* return error */
        }
    }
    else
    {
        return 1;                                                             /* return error */
    }
    
    p = strstr(input, AOX4000_COMMAND_PRESSURE);                              /* find pressure */
    if (p != NULL)                                                            /* check pressure */
    {
        res = aox4000_parse_number(handle, AOX4000_COMMAND_PRESSURE, 
                                  p, pressure_mbar);                          /* parse number*/
        if (res != 0)                                                         /* check result */
        {
            return 1;                                                         /* return error */
        }
    }
    else
    {
        return 1;                                                             /* return error */
    }
    
    handle->last_error = (uint8_t)AOX4000_STATUS_UNKNOWN;                     /* set unknown */
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t aox4000_init(aox4000_handle_t *handle)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->debug_print == NULL)                                  /* check debug_print */
    {
        return 3;                                                     /* return error */
    }
    if (handle->uart_init == NULL)                                    /* check uart_init */
    {
        handle->debug_print("aox4000: uart_init is null.\n");         /* uart_init is null */
        
        return 3;                                                     /* return error */
    }
    if (handle->uart_deinit == NULL)                                  /* check uart_deinit */
    {
        handle->debug_print("aox4000: uart_deinit is null.\n");       /* uart_deinit is null */
        
        return 3;                                                     /* return error */
    }
    if (handle->uart_read == NULL)                                    /* check uart_read */
    {
        handle->debug_print("aox4000: uart_read is null.\n");         /* uart_read is null */
        
        return 3;                                                     /* return error */
    }
    if (handle->uart_write == NULL)                                   /* check uart_write */
    {
        handle->debug_print("aox4000: uart_write is null.\n");        /* uart_write is null */
        
        return 3;                                                     /* return error */
    }
    if (handle->uart_flush == NULL)                                   /* check uart_flush */
    {
        handle->debug_print("aox4000: uart_flush is null.\n");        /* uart_flush is null */
        
        return 3;                                                     /* return error */
    }
    if (handle->delay_ms == NULL)                                     /* check delay_ms */
    {
        handle->debug_print("aox4000: delay_ms is null.\n");          /* delay_ms is null */
        
        return 3;                                                     /* return error */
    }

    if (handle->uart_init() != 0)                                     /* uart init */
    {
        handle->debug_print("aox4000: uart init failed.\n");          /* uart init failed */
        
        return 1;                                                     /* return error */
    }
    handle->mode = (uint8_t)(AOX4000_MODE_REQUEST);                   /* set request mode */
    handle->inited = 1;                                               /* flag finish initialization */
    
    return 0;                                                         /* success return 0 */
}

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
uint8_t aox4000_deinit(aox4000_handle_t *handle)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    
    if (handle->uart_deinit() != 0)                                  /* uart deinit */
    {
        handle->debug_print("aox4000: uart deinit failed.\n");       /* uart deinit failed */
        
        return 1;                                                    /* return error */
    }
    handle->inited = 0;                                              /* flag close */
    
    return 0;                                                        /* success return 0 */ 
}

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
uint8_t aox4000_get_last_error(aox4000_handle_t *handle, aox4000_status_t *status)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
    
    *status = (aox4000_status_t)(handle->last_error);       /* get last error */
    
    return 0;                                               /* success return 0 */ 
}

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
uint8_t aox4000_set_mode(aox4000_handle_t *handle, aox4000_mode_t mode)
{
    uint8_t res;
    uint16_t input_len;
    uint16_t output_len;
    uint8_t buf[1];
    char input[16];
    char output[16];
    
    if (handle == NULL)                                                                    /* check handle */
    {
        return 2;                                                                          /* return error */
    }
    if (handle->inited != 1)                                                               /* check handle initialization */
    {
        return 3;                                                                          /* return error */
    }
    
    if (mode == AOX4000_MODE_AUTO)                                                         /* auto mode */
    {
        memset(input, 0, sizeof(char) * 16);                                               /* init input buffer */
        memset(output, 0, sizeof(char) * 16);                                              /* init output buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s 0\r\n", AOX4000_COMMAND_MODE);       /* get input length */
    }
    else                                                                                   /* request mode */
    {
        memset(input, 0, sizeof(char) * 16);                                               /* init input buffer */
        memset(output, 0, sizeof(char) * 16);                                              /* init output buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s 1\r\n", AOX4000_COMMAND_MODE);       /* get input length */
    }
    
    output_len = 15;                                                                       /* set 15 */
    res = a_aox4000_write_read(handle, input, input_len, output, &output_len);             /* write read */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("aox4000: uart write read failed.\n");                         /* uart write read failed */
       
        return 1;                                                                          /* return error */
    }
    if (output_len < 5)                                                                    /* check length */
    {
        handle->debug_print("aox4000: response error failed..\n");                         /* response error failed. */
       
        return 1;                                                                          /* return error */
    }
    res = aox4000_parse_mode(handle, output, buf);                                         /* parse the mode */
    if (res != 0)                                                                          /* check result */
    {
        handle->debug_print("aox4000: response error failed.\n");                          /* response error failed */
       
        return 1;                                                                          /* return error */
    }
    if (buf[0] != (uint8_t)(mode))                                                         /* check response */
    {
        handle->debug_print("aox4000: response error failed.\n");                          /* response error failed */
       
        return 1;                                                                          /* return error */
    }
    handle->mode = (uint8_t)(mode);                                                        /* save mode */
    
    return 0;                                                                              /* success return 0 */
}

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
uint8_t aox4000_get_mode(aox4000_handle_t *handle, aox4000_mode_t *mode)
{
    uint8_t res;
    uint16_t input_len;
    uint16_t output_len;
    uint8_t buf[1];
    char input[16];
    char output[16];
    
    if (handle == NULL)                                                              /* check handle */
    {
        return 2;                                                                    /* return error */
    }
    if (handle->inited != 1)                                                         /* check handle initialization */
    {
        return 3;                                                                    /* return error */
    }
    
    memset(input, 0, sizeof(char) * 16);                                             /* init input buffer */
    memset(output, 0, sizeof(char) * 16);                                            /* init output buffer */
    input_len = (uint16_t)snprintf(input, 15, "%s\r\n", AOX4000_COMMAND_MODE);       /* get input length */
    
    output_len = 15;                                                                 /* set 15 */
    res = a_aox4000_write_read(handle, input, input_len, output, &output_len);       /* write read */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("aox4000: uart write read failed.\n");                   /* uart write read failed */
       
        return 1;                                                                    /* return error */
    }
    res = aox4000_parse_mode(handle, output, buf);                                   /* parse the mode */
    if (res != 0)                                                                    /* check result */
    {
        handle->debug_print("aox4000: response error failed.\n");                    /* response error failed */
       
        return 1;                                                                    /* return error */
    }
    if (output_len < 5)                                                              /* check length */
    {
        handle->debug_print("aox4000: response error failed..\n");                   /* response error failed. */
       
        return 1;                                                                    /* return error */
    }
    if ((buf[0] == 0) || (buf[0] == 1))                                              /* check response */
    {
        *mode = (aox4000_mode_t)(buf[0]);                                            /* save mode */
        
        return 0;                                                                    /* success return 0 */
    }
    else
    {
        handle->debug_print("aox4000: response error failed.\n");                    /* response error failed */
       
        return 1;                                                                    /* return error */
    }
}

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
                     float *temperature_degree, float *pressure_mbar)
{
    uint8_t res;
    
    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }
    
    if (handle->mode == (uint8_t)(AOX4000_MODE_AUTO))                                    /* auto mode */
    {
        char output[48];
        uint16_t output_len;
        
        output_len = 47;                                                                 /* set 47 */
        res = a_aox4000_read(handle, output, &output_len);                               /* read */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("aox4000: uart read failed.\n");                         /* uart read failed */
           
            return 1;                                                                    /* return error */
        }
        if (output_len < 5)                                                              /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");                   /* response error failed. */
           
            return 1;                                                                    /* return error */
        }
        res = aox4000_parse_data(handle, output, 
                                oxygen_ppo2_mbar, oxygen_density_percentage,
                                temperature_degree, pressure_mbar);                      /* parse data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("aox4000: response error.\n");                           /* response error */
           
            return 1;                                                                    /* return error */
        }
        
        return 0;                                                                        /* success return 0 */
    }
    else                                                                                 /* request mode */
    {
        char input[16];
        char output[48];
        uint16_t input_len;
        uint16_t output_len;
        
        memset(input, 0, sizeof(char) * 16);                                             /* init input buffer */
        memset(output, 0, sizeof(char) * 48);                                            /* init buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s\r\n", AOX4000_COMMAND_ALL);        /* get input length */
        output_len = 47;                                                                 /* set 47 */
        res = a_aox4000_write_read(handle, input, input_len, output, &output_len);       /* write read */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("aox4000: uart write read failed.\n");                   /* uart write read failed */
           
            return 1;                                                                    /* return error */
        }
        if (output_len < 5)                                                              /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");                   /* response error failed. */
           
            return 1;                                                                    /* return error */
        }
        res = aox4000_parse_data(handle, output, 
                                oxygen_ppo2_mbar, oxygen_density_percentage,
                                temperature_degree, pressure_mbar);                      /* parse data */
        if (res != 0)                                                                    /* check result */
        {
            handle->debug_print("aox4000: response error.\n");                           /* response error */
           
            return 1;                                                                    /* return error */
        }
        
        return 0;                                                                        /* success return 0 */
    }
}

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
uint8_t aox4000_read_oxygen_ppo2(aox4000_handle_t *handle, float *mbar)
{
    uint8_t res;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    if (handle->mode == (uint8_t)(AOX4000_MODE_AUTO))                        /* auto mode */
    {
        char output[48];
        uint16_t output_len;
        float oxygen_density_percentage;
        float temperature_degree;
        float pressure_mbar;
        
        output_len = 47;                                                     /* set 47 */
        res = a_aox4000_read(handle, output, &output_len);                   /* read */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: uart read failed.\n");             /* uart read failed */
           
            return 1;                                                        /* return error */
        }
        if (output_len < 5)                                                  /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");       /* response error failed. */
           
            return 1;                                                        /* return error */
        }
        res = aox4000_parse_data(handle, output, 
                                mbar, &oxygen_density_percentage,
                                &temperature_degree, &pressure_mbar);        /* parse data */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: response error.\n");               /* response error */
           
            return 1;                                                        /* return error */
        }
        
        return 0;                                                            /* success return 0 */
    }
    else                                                                     /* request mode */
    {
        char input[16];
        char output[48];
        uint16_t input_len;
        uint16_t output_len;
        
        memset(input, 0, sizeof(char) * 16);                                 /* init input buffer */
        memset(output, 0, sizeof(char) * 48);                                /* init buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s\r\n", 
                                       AOX4000_COMMAND_OXYGEN_PPO2);         /* get input length */
        output_len = 47;                                                     /* set 47 */
        res = a_aox4000_write_read(handle, input, input_len, 
                                  output, &output_len);                      /* write read */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: uart write read failed.\n");       /* uart write read failed */
           
            return 1;                                                        /* return error */
        }
        if (output_len < 5)                                                  /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");       /* response error failed. */
           
            return 1;                                                        /* return error */
        }
        res = aox4000_parse_number(handle, AOX4000_COMMAND_OXYGEN_PPO2, 
                                  output, mbar);                             /* parse data */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: response error.\n");               /* response error */
           
            return 1;                                                        /* return error */
        }
        
        return 0;                                                            /* success return 0 */
    }
}

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
uint8_t aox4000_read_oxygen_density(aox4000_handle_t *handle, float *percentage)
{
    uint8_t res;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    if (handle->mode == (uint8_t)(AOX4000_MODE_AUTO))                        /* auto mode */
    {
        char output[48];
        uint16_t output_len;
        float oxygen_ppo2_mbar;
        float temperature_degree;
        float pressure_mbar;
        
        output_len = 47;                                                     /* set 47 */
        res = a_aox4000_read(handle, output, &output_len);                   /* read */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: uart read failed.\n");             /* uart read failed */
           
            return 1;                                                        /* return error */
        }
        if (output_len < 5)                                                  /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");       /* response error failed. */
           
            return 1;                                                        /* return error */
        }
        res = aox4000_parse_data(handle, output, 
                                &oxygen_ppo2_mbar, percentage,
                                &temperature_degree, &pressure_mbar);        /* parse data */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: response error.\n");               /* response error */
           
            return 1;                                                        /* return error */
        }
        
        return 0;                                                            /* success return 0 */
    }
    else                                                                     /* request mode */
    {
        char input[16];
        char output[48];
        uint16_t input_len;
        uint16_t output_len;
        
        memset(input, 0, sizeof(char) * 16);                                 /* init input buffer */
        memset(output, 0, sizeof(char) * 48);                                /* init buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s\r\n", 
                                       AOX4000_COMMAND_OXYGEN_DENSITY);      /* get input length */
        output_len = 47;                                                     /* set 47 */
        res = a_aox4000_write_read(handle, input, input_len, 
                                  output, &output_len);                      /* write read */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: uart write read failed.\n");       /* uart write read failed */
           
            return 1;                                                        /* return error */
        }
        if (output_len < 5)                                                  /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");       /* response error failed. */
           
            return 1;                                                        /* return error */
        }
        res = aox4000_parse_number(handle, AOX4000_COMMAND_OXYGEN_DENSITY, 
                                  output, percentage);                       /* parse data */
        if (res != 0)                                                        /* check result */
        {
            handle->debug_print("aox4000: response error.\n");               /* response error */
           
            return 1;                                                        /* return error */
        }
        
        return 0;                                                            /* success return 0 */
    }
}

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
uint8_t aox4000_read_pressure(aox4000_handle_t *handle, float *mbar)
{
    uint8_t res;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    if (handle->mode == (uint8_t)(AOX4000_MODE_AUTO))                                 /* auto mode */
    {
        char output[48];
        uint16_t output_len;
        float oxygen_ppo2_mbar;
        float oxygen_density_percentage;
        float temperature_degree;
        
        output_len = 47;                                                              /* set 47 */
        res = a_aox4000_read(handle, output, &output_len);                            /* read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: uart read failed.\n");                      /* uart read failed */
           
            return 1;                                                                 /* return error */
        }
        if (output_len < 5)                                                           /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");                /* response error failed. */
           
            return 1;                                                                 /* return error */
        }
        res = aox4000_parse_data(handle, output, 
                                &oxygen_ppo2_mbar, &oxygen_density_percentage,
                                &temperature_degree, mbar);                           /* parse data */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: response error.\n");                        /* response error */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
    else                                                                              /* request mode */
    {
        char input[16];
        char output[48];
        uint16_t input_len;
        uint16_t output_len;
        
        memset(input, 0, sizeof(char) * 16);                                          /* init input buffer */
        memset(output, 0, sizeof(char) * 48);                                         /* init buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s\r\n", 
                                       AOX4000_COMMAND_PRESSURE);                     /* get input length */
        output_len = 47;                                                              /* set 47 */
        res = a_aox4000_write_read(handle, input, input_len, 
                                  output, &output_len);                               /* write read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: uart write read failed.\n");                /* uart write read failed */
           
            return 1;                                                                 /* return error */
        }
        if (output_len < 5)                                                           /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");                /* response error failed. */
           
            return 1;                                                                 /* return error */
        }
        res = aox4000_parse_number(handle, AOX4000_COMMAND_PRESSURE, output, mbar);   /* parse data */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: response error.\n");                        /* response error */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
}

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
uint8_t aox4000_read_temperature(aox4000_handle_t *handle, float *degree)
{
    uint8_t res;
    
    if (handle == NULL)                                                               /* check handle */
    {
        return 2;                                                                     /* return error */
    }
    if (handle->inited != 1)                                                          /* check handle initialization */
    {
        return 3;                                                                     /* return error */
    }
    
    if (handle->mode == (uint8_t)(AOX4000_MODE_AUTO))                                 /* auto mode */
    {
        char output[48];
        uint16_t output_len;
        float oxygen_ppo2_mbar;
        float oxygen_density_percentage;
        float pressure_mbar;
        
        output_len = 47;                                                              /* set 47 */
        res = a_aox4000_read(handle, output, &output_len);                            /* read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: uart read failed.\n");                      /* uart read failed */
           
            return 1;                                                                 /* return error */
        }
        if (output_len < 5)                                                           /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");                /* response error failed. */
           
            return 1;                                                                 /* return error */
        }
        res = aox4000_parse_data(handle, output, 
                                &oxygen_ppo2_mbar, &oxygen_density_percentage,
                                degree, &pressure_mbar);                              /* parse data */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: response error.\n");                        /* response error */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
    else                                                                              /* request mode */
    {
        char input[16];
        char output[48];
        uint16_t input_len;
        uint16_t output_len;
        
        memset(input, 0, sizeof(char) * 16);                                          /* init input buffer */
        memset(output, 0, sizeof(char) * 48);                                         /* init buffer */
        input_len = (uint16_t)snprintf(input, 15, "%s\r\n", 
                                       AOX4000_COMMAND_TEMPERATURE);                  /* get input length */
        output_len = 47;                                                              /* set 47 */
        res = a_aox4000_write_read(handle, input, input_len, 
                                  output, &output_len);                               /* write read */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: uart write read failed.\n");                /* uart write read failed */
           
            return 1;                                                                 /* return error */
        }
        if (output_len < 5)                                                           /* check length */
        {
            handle->debug_print("aox4000: response error failed..\n");                /* response error failed. */
           
            return 1;                                                                 /* return error */
        }
        res = aox4000_parse_number(handle, AOX4000_COMMAND_TEMPERATURE, 
                                  output, degree);                                    /* parse data */
        if (res != 0)                                                                 /* check result */
        {
            handle->debug_print("aox4000: response error.\n");                        /* response error */
           
            return 1;                                                                 /* return error */
        }
        
        return 0;                                                                     /* success return 0 */
    }
}

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
uint8_t aox4000_command_read(aox4000_handle_t *handle, char *buf, uint16_t len)
{
    uint16_t l;

    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    l = handle->uart_read((uint8_t *)buf, len);                    /* uart read */
    if (l != len)                                                  /* check result */
    {
        handle->debug_print("aox4000: uart read failed.\n");       /* uart read failed */
       
        return 1;                                                  /* return error */
    }
    
    return 0;                                                      /* success return 0 */ 
}

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
uint8_t aox4000_command_write(aox4000_handle_t *handle, char *buf, uint16_t len)
{
    uint8_t res;

    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    res = handle->uart_flush();                                     /* uart flush */ 
    if (res != 0)                                                   /* check result */
    {
        handle->debug_print("aox4000: uart flush failed.\n");       /* uart flush failed */
       
        return 1;                                                   /* return error */
    }
    res = handle->uart_write((uint8_t *)buf, len);
    if (res != 0)                                                   /* check result */
    {
        handle->debug_print("aox4000: uart write failed.\n");       /* uart write failed */
       
        return 1;                                                   /* return error */
    }
    
    return 0;                                                       /* success return 0 */ 
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to an aox4000 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t aox4000_info(aox4000_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(aox4000_info_t));                        /* initialize aox4000 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}

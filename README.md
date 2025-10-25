[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver AOX4000

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/aox4000/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

AOX4000 is an oxygen sensor based on the principle of fluorescence quenching, which measures the partial pressure of oxygen in the environment. The sensor is equipped with a built-in air pressure sensor, which can directly output the values of ambient air pressure, oxygen partial pressure, and oxygen concentration, making it easy for users to intuitively read the data. Compared with electrochemical sensors, AOX4000 uses non lossy oxygen sensitive materials, thus having a longer lifespan. Due to its temperature compensation function, no additional compensation system is required. AOX4000 is very stable and environmentally friendly, without lead or any other harmful substances, and is almost unaffected by cross interference from other gases.

LibDriver AOX4000 is a full-featured driver for AOX4000, launched by LibDriver. It provides oxygen partial pressure reading, oxygen concentration reading, atmospheric pressure reading, temperature reading and additional features. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver AOX4000 source files.

/interface includes LibDriver AOX4000 UART platform independent template.

/test includes LibDriver AOX4000 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver AOX4000 sample code.

/doc includes LibDriver AOX4000 offline document.

/datasheet includes AOX4000 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface UART platform independent template and finish your platform UART driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_aox4000_basic.h"

uint8_t res;
uint32_t i;

res = aox4000_basic_init();
if (res != 0)
{
    return 1;
}

...
    
for (i = 0; i < 3; i++)
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
    aox4000_interface_debug_print("aox4000: %d/%d.\n", i + 1, 3);
    aox4000_interface_debug_print("aox4000: oxygen ppo2 is %0.2fmbar.\n", oxygen_ppo2_mbar);
    aox4000_interface_debug_print("aox4000: oxygen density is %0.2f%%.\n", oxygen_density_percentage);
    aox4000_interface_debug_print("aox4000: temperature is %0.2fC.\n", temperature_degree);
    aox4000_interface_debug_print("aox4000: pressure is %0.2fmbar.\n", pressure_mbar);
    
    ...
}

...
    
/* deinit */
(void)aox4000_basic_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/aox4000/index.html](https://www.libdriver.com/docs/aox4000/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.
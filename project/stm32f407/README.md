### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

UART2 Pin: TX/RX PA2/PA3.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. AOX4000

#### 3.1 Command Instruction

1. Show aox4000 chip and driver information.

   ```shell
   aox4000 (-i | --information)
   ```

2. Show aox4000 help.

   ```shell
   aox4000 (-h | --help)
   ```

3. Show aox4000 pin connections of the current board.

   ```shell
   aox4000 (-p | --port)
   ```

4. Run aox4000 read test, num is the test times.

   ```shell
   aox4000 (-t read | --test=read) [--times=<num>]
   ```


6. Run aox4000 read function, num is the test times.

   ```shell
   aox4000 (-e read | --example=read) [--times=<num>]
   ```

#### 3.2 Command Example

```shell
aox4000 -i

aox4000: chip is ASAIR AOX4000.
aox4000: manufacturer is ASAIR.
aox4000: interface is UART.
aox4000: driver version is 1.0.
aox4000: min supply voltage is 4.8V.
aox4000: max supply voltage is 5.2V.
aox4000: max current is 32.00mA.
aox4000: max temperature is 60.0C.
aox4000: min temperature is -30.0C.
```

```shell
aox4000 -p

aox4000: TX connected to GPIOA PIN3.
aox4000: RX connected to GPIOA PIN2.
```

```shell
aox4000 -t read --times=3

aox4000: chip is ASAIR AOX4000.
aox4000: manufacturer is ASAIR.
aox4000: interface is UART.
aox4000: driver version is 1.0.
aox4000: min supply voltage is 4.8V.
aox4000: max supply voltage is 5.2V.
aox4000: max current is 32.00mA.
aox4000: max temperature is 60.0C.
aox4000: min temperature is -30.0C.
aox4000: start read test.
aox4000: auto mode read test.
aox4000: read test.
aox4000: oxygen ppo2 is 221.90mbar.
aox4000: oxygen density is 21.97%.
aox4000: temperature is 26.20C.
aox4000: pressure is 1010.00mbar.
aox4000: oxygen ppo2 is 221.90mbar.
aox4000: oxygen density is 21.97%.
aox4000: temperature is 26.20C.
aox4000: pressure is 1010.00mbar.
aox4000: oxygen ppo2 is 221.90mbar.
aox4000: oxygen density is 21.97%.
aox4000: temperature is 26.20C.
aox4000: pressure is 1010.00mbar.
aox4000: read oxygen ppo2 test.
aox4000: oxygen ppo2 is 221.90mbar.
aox4000: oxygen ppo2 is 220.80mbar.
aox4000: oxygen ppo2 is 220.80mbar.
aox4000: read oxygen density test.
aox4000: oxygen density is 21.86%.
aox4000: oxygen density is 21.86%.
aox4000: oxygen density is 21.86%.
aox4000: read temperature test.
aox4000: temperature is 26.50C.
aox4000: temperature is 26.50C.
aox4000: temperature is 26.50C.
aox4000: read pressure test.
aox4000: pressure is 1010.00mbar.
aox4000: pressure is 1010.00mbar.
aox4000: pressure is 1010.00mbar.
aox4000: request mode read test.
aox4000: read test.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen density is 21.75%.
aox4000: temperature is 26.60C.
aox4000: pressure is 1010.00mbar.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen density is 21.75%.
aox4000: temperature is 26.60C.
aox4000: pressure is 1010.00mbar.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen density is 21.75%.
aox4000: temperature is 26.60C.
aox4000: pressure is 1010.00mbar.
aox4000: read oxygen ppo2 test.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: read oxygen density test.
aox4000: oxygen density is 21.75%.
aox4000: oxygen density is 21.75%.
aox4000: oxygen density is 21.75%.
aox4000: read temperature test.
aox4000: temperature is 26.67C.
aox4000: temperature is 26.67C.
aox4000: temperature is 26.67C.
aox4000: read pressure test.
aox4000: pressure is 1010.00mbar.
aox4000: pressure is 1010.00mbar.
aox4000: pressure is 1010.00mbar.
aox4000: finish read test.
```

```shell
aox4000 -e read --times=3

aox4000: 1/3.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen density is 21.75%.
aox4000: temperature is 26.70C.
aox4000: pressure is 1010.00mbar.
aox4000: 2/3.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen density is 21.75%.
aox4000: temperature is 26.70C.
aox4000: pressure is 1010.00mbar.
aox4000: 3/3.
aox4000: oxygen ppo2 is 219.70mbar.
aox4000: oxygen density is 21.75%.
aox4000: temperature is 26.70C.
aox4000: pressure is 1010.00mbar.
```

```shell
aox4000 -h

Usage:
  aox4000 (-i | --information)
  aox4000 (-h | --help)
  aox4000 (-p | --port)
  aox4000 (-t read | --test=read) [--times=<num>]
  aox4000 (-e read | --example=read) [--times=<num>]

Options:
  -e <read>, --example=<read>        Run the driver example.
  -h, --help                         Show the help.
  -i, --information                  Show the chip information.
  -p, --port                         Display the pins used by this device to connect the chip.
  -t <read>, --test=<read>           Run the driver test.
      --times=<num>                  Set the running times.([default: 3])
```


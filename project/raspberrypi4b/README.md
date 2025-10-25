### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

UART Pin: TX/RX GPIO14/GPIO15.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(aox4000 REQUIRED)
```

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


5. Run aox4000 read function, num is the test times.

   ```shell
   aox4000 (-e read | --example=read) [--times=<num>]
   ```

#### 3.2 Command Example

```shell
./aox4000 -i

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
./aox4000 -p

aox4000: TX connected to GPIO15(BCM).
aox4000: RX connected to GPIO14(BCM).
```

```shell
./aox4000 -t read --times=3

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
./aox4000 -e read --times=3

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
./aox4000 -h

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


## Description

This example uses the I2C Master to find the addresses of any I2C Slave devices connected to the same bus as I2C2 on the MAX32655EVKIT and I2C1 on the MAX32655FTHR.

Please check schematic diagram to get more information about the discovered devices.

## Software

### Project Usage

Universal instructions on building, flashing, and debugging this project can be found in the **[MSDK User Guide](https://analogdevicesinc.github.io/msdk/USERGUIDE/)**.

### Project-Specific Build Notes

* This project comes pre-configured for the MAX32655EVKIT.  See [Board Support Packages](https://analogdevicesinc.github.io/msdk/USERGUIDE/#board-support-packages) in the MSDK User Guide for instructions on changing the target board.

## Required Connections
If using the MAX32655EVKIT (EvKit\_V1):
-   Connect a USB cable between the PC and the CN1 (USB/PWR) connector.
-   Connect pins JP4(RX_SEL) and JP5(TX_SEL) to RX0 and TX0  header.
-   Open an terminal application on the PC and connect to the EV kit's console UART at 115200, 8-N-1.
-   You must also connect the pull-up jumpers (JP21 and JP22) to the proper I/O voltage.
-   Connect I2C2 (SCL - P0.30 and SDA - P0.31) to the I2C bus to scan.

If using the MAX32655FTHR (FTHR\_Apps\_V1):
-   Connect a USB cable between the PC and the J4 (USB/PWR) connector.
-   Open an terminal application on the PC and connect to the board's console UART at 115200, 8-N-1.

## Expected Output

The Console UART of the device will output these messages:

```
******** I2C SLAVE ADDRESS SCANNER *********

This example finds the addresses of any I2C Slave devices connected to the
same bus as I2C2 (SCL - P0.30, SDA - P0.31). You must connect the pull-up
jumpers (JP21 and JP22) to the proper I/O voltage.
-->I2C Master Initialization Complete
-->Scanning started
.................
Found slave ID 024; 0x18
...............................................................................................
-->Scan finished. 1 devices found
```

#include <xc.h>
#include "include/uart.h"

void uart_init(){
    
    // Configure UART peripherial pin select.
    U2RXR = 1;
    RPB14R = 2;
    
    // Initalize the UART with:
    //   - Baud: 115200
    //   - Stop Bits: 1
    //   - Parity: None
    //   - Data: 8-bit
    U2MODEbits.RTSMD = 1;
    U2MODEbits.UEN = 0; 
    U2MODEbits.PDSEL = 0;
    U2MODEbits.STSEL = 0;
    
    U2STAbits.UTXISEL = 1;
    U2STAbits.URXISEL = 0;
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;
    
    // This value is dependent on PBCLK2 = 100MHz.
    // Target baud is 115200, best we can do is 115,740.
    // Need to slow down? Set to 324 for a baud of 19200.
    U2BRG = 53;
    
    // Now actually turn the UART on. Future calls to printf() will print to
    // the COM port.
    U2MODEbits.ON = 1;
    
    return;
}
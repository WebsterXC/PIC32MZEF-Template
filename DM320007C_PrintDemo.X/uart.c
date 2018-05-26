#include <xc.h>
#include "uart.h"

/* I could only find one set of baud rates that would work correctly with a 
 * typical off-the-shelf USB-UART converter. Even the onboard MCP2221 would
 * only work with these settings. This program has been tested working with
 * the onboard MCP2221, but since a Windows Update bricked mine, I used a
 * CY7C65211 breakout board.
 *
 * In Windows PowerShell:
 * Baud: 19200, Bits: 8, Stop: 1, Parity, None
 * The UART2 runs at 9600 baud.
 */

void uart_init(){
    
    /* UART is PPS based (peripherial pin select). Configure
     * Rx and Tx pins accordingly. */
    U2RXR = 1;
    RPB14R = 2;
    
    /* U2 MODE */
    U2MODEbits.RTSMD = 1;
    U2MODEbits.UEN = 0; 
    U2MODEbits.PDSEL = 0; // 8b, No Parity
    U2MODEbits.STSEL = 0; // 1 Stop Bit
    
    /* U2 Status */
    U2STAbits.UTXISEL = 1;
    U2STAbits.URXISEL = 0;
    U2STAbits.URXEN = 1;
    U2STAbits.UTXEN = 1;
    
    /* U2 Baud Rate (9600) */
    /* Dependent on (PBCLK2 = 50MHz) */
    U2BRG = 0x144;
    
    U2MODEbits.ON = 1;
    
    return;
}
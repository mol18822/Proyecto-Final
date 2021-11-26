// Universidad del Valle de Guatemala
// Electrónica Digital II
// Proyecto Final
// Luis Pedro Molina - 18822
// Guillermo Lam - 19267

// --------------------------------- Librerias --------------------------------------------------

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"
#include "driverlib/uart.h"
#include "driverlib/pin_map.h"

// ------------------------------------ Variables ----------------------------------------------

uint8_t bandera = 0;


// ----------------------------- Prototipos de funciones ---------------------------------------

// ------------------------------------- Main --------------------------------------------------

int main(void)
{

    // Oscilador
    SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

    // PORTC para PBS
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    GPIOPadConfigSet(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);

    // PORTE para leds rojos --> Parqueos ocupados
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // PORTD para leds verdes --> Parqueos disponibles
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);

    // UART configuration
    //SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA); // enable A
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1); // enable uart1
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    GPIOPinConfigure(GPIO_PB0_U1RX);
    GPIOPinConfigure(GPIO_PB1_U1TX);
    GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1); // uart controls pins
    UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200, (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE)); // config uart
    UARTIntClear(UART1_BASE, UART_INT_RX | UART_INT_RT | UART_INT_TX | UART_INT_FE | UART_INT_PE | UART_INT_BE | UART_INT_OE | UART_INT_RI | UART_INT_CTS | UART_INT_DCD | UART_INT_DSR); // clear isr
    UARTEnable (UART1_BASE);

    uint8_t bandera_dos = 1;


// ----------------------------------- Loop Principal ----------------------------------------------

    while (1)
    {
        bandera = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7) >> 4;//CORRIMIENTO DE BITS 1101 0110 0011 0001
        if (bandera != bandera_dos)
        {
            GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, bandera);
            GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3, ~bandera);
            bandera_dos = bandera;
            UARTCharPut(UART1_BASE, bandera);
        }
    }
}


    //GPIOPinWrite(GPIO_PORTB_BASE, 0xFF, temp);


/*
 * 7-Segment Up/Down Counter
 * MCU      : PIC16F877A
 * Compiler : HI-TECH C (PICC)
 * Clock    : 4 MHz (set Processor Clock Frequency = 4MHz in Proteus)
 *
 * Connections:
 *   RD0..RD6 -> segments a..g of a COMMON CATHODE 7-segment display
 *   RB0      -> UP button   (other side of button to GND, 10k pull-up to +5V)
 *   RB1      -> DOWN button (other side of button to GND, 10k pull-up to +5V)
 *   MCLR     -> +5V through a 10k resistor
 */

#include <htc.h>

#define _XTAL_FREQ 4000000      // 4 MHz, needed by __delay_ms()

// Configuration bits
__CONFIG(FOSC_XT & WDTE_OFF & PWRTE_ON & BOREN_ON & LVP_OFF & CPD_OFF & WRT_OFF & CP_OFF);

#define BTN_UP    RB0
#define BTN_DOWN  RB1

// Segment codes for a COMMON CATHODE display (bit0 = a ... bit6 = g)
const unsigned char seg[10] = {
    0x3F,   // 0
    0x06,   // 1
    0x5B,   // 2
    0x4F,   // 3
    0x66,   // 4
    0x6D,   // 5
    0x7D,   // 6
    0x07,   // 7
    0x7F,   // 8
    0x6F    // 9
};

void main(void)
{
    unsigned char count = 0;

    ADCON1 = 0x06;      // Make PORTA and PORTE digital I/O
    TRISD  = 0x00;      // PORTD = output (segments)
    TRISB  = 0xFF;      // PORTB = input (buttons)

    PORTD = seg[count]; // Show 0 at start

    while (1)
    {
        // ---- UP button ----
        if (BTN_UP == 0)
        {
            __delay_ms(30);             // debounce
            if (BTN_UP == 0)
            {
                count++;
                if (count > 9)
                {
                    count = 0;          // 9 -> 0
                }
                PORTD = seg[count];
                while (BTN_UP == 0);    // wait until released
                __delay_ms(30);
            }
        }

        // ---- DOWN button ----
        if (BTN_DOWN == 0)
        {
            __delay_ms(30);             // debounce
            if (BTN_DOWN == 0)
            {
                if (count == 0)
                {
                    count = 9;          // 0 -> 9
                }
                else
                {
                    count--;
                }
                PORTD = seg[count];
                while (BTN_DOWN == 0);  // wait until released
                __delay_ms(30);
            }
        }
    }
}

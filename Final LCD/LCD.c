#include "LCD.h"
#include "TM4C123.h"
#include "systick.h"

// Rs = PD0
// Rw = PD1
// En = PD2
// D0 = PA7, D1 = PA6, D2 = PA5, D3 = PB4
// D4 = PE5, D5 = PE4, D6 = PE1, D7 = PD3


void LCD_Init(void) {
    
		LCD_Init_GPIO ();
    SysTick_Init();   // Initialize SysTick for delays

    delay_ms(50); // Wait for LCD to power up
    LCD_Command(0x38); // 8-bit, 2-line, 5x7 font
    LCD_Command(0x0C); // Display ON, Cursor OFF
    LCD_Command(0x06); // Entry Mode Set
    LCD_Command(0x01); // Clear Display
    delay_ms(2);       // Extra delay after clear
}

static void LCD_WritePins(uint8_t data) {
    // D0 -> PA7
    if (data & (1U << 0)) GPIOA->DATA |= (1U << 7); else GPIOA->DATA &= ~(1U << 7);
    // D1 -> PA6
    if (data & (1U << 1)) GPIOA->DATA |= (1U << 6); else GPIOA->DATA &= ~(1U << 6);
    // D2 -> PA5
    if (data & (1U << 2)) GPIOA->DATA |= (1U << 5); else GPIOA->DATA &= ~(1U << 5);
    // D3 -> PB4
    if (data & (1U << 3)) GPIOB->DATA |= (1U << 4); else GPIOB->DATA &= ~(1U << 4);
    // D4 -> PE5
    if (data & (1U << 4)) GPIOE->DATA |= (1U << 5); else GPIOE->DATA &= ~(1U << 5);
    // D5 -> PE4
    if (data & (1U << 5)) GPIOE->DATA |= (1U << 4); else GPIOE->DATA &= ~(1U << 4);
    // D6 -> PE1
    if (data & (1U << 6)) GPIOE->DATA |= (1U << 1); else GPIOE->DATA &= ~(1U << 1);
    // D7 -> PD3
    if (data & (1U << 7)) GPIOD->DATA |= (1U << 3); else GPIOD->DATA &= ~(1U << 3);
}

void LCD_Command(uint8_t cmd) {
    LCD_WritePins(cmd);
    GPIOD->DATA &= ~(1U << 0); // RS = 0
    GPIOD->DATA &= ~(1U << 1); // RW = 0
    GPIOD->DATA |= (1U << 2);  // EN = 1
    delay_ms(1);
    GPIOD->DATA &= ~(1U << 2); // EN = 0
    delay_ms(2);
}

void LCD_Data(uint8_t data) {
    LCD_WritePins(data);
    GPIOD->DATA |= (1U << 0);  // RS = 1
    GPIOD->DATA &= ~(1U << 1); // RW = 0
    GPIOD->DATA |= (1U << 2);  // EN = 1
    delay_ms(1);
    GPIOD->DATA &= ~(1U << 2); // EN = 0
    delay_ms(2);
}

void LCD_String(char *str) {
    while (*str) {
        LCD_Data(*str++);
    }
}


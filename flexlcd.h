

#ifndef __flex_lcd_H
#define __flex_lcd_H
#define _XTAL_FREQ 4000000
//******************************************
//  Libreria para control de LCD           *
//  Modo Control 4 bits                    * 
//  Hector Ramos
//******************************************


#define LCD_RD4     PORTBbits.RB4      // D4
#define TRISRB4     TRISBbits.TRISB4

#define LCD_RD3     PORTBbits.RB3      // D3
#define TRISRB3     TRISBbits.TRISB3

#define LCD_RD2     PORTBbits.RB2      // D2
#define TRISRB2     TRISBbits.TRISB2

#define LCD_RD1     PORTBbits.RB1       // D1
#define TRISRB1     TRISBbits.TRISB1

#define LCD_EN      PORTBbits.RB6      // EN
#define TRISEN      TRISBbits.TRISB6

#define LCD_RS      PORTBbits.RB5      // RS
#define TRISRS      TRISBbits.TRISB5

//comandos disponibles
#define      LCD_FIRST_ROW           128
#define      LCD_SECOND_ROW          192
#define      LCD_THIRD_ROW           148
#define      LCD_FOURTH_ROW          212
#define      LCD_CLEAR               1
#define      LCD_RETURN_HOME         2
#define      LCD_CURSOR_OFF          12
#define      LCD_UNDERLINE_ON        14
#define      LCD_BLINK_CURSOR_ON     15
#define      LCD_MOVE_CURSOR_LEFT    16
#define      LCD_MOVE_CURSOR_RIGHT   20
#define      LCD_TURN_OFF            0
#define      LCD_TURN_ON             8
#define      LCD_SHIFT_LEFT          24
#define      LCD_SHIFT_RIGHT         28


void Lcd_Init(void);
void Lcd_Out(unsigned char y, unsigned char x, const char *buffer);
void Lcd_Out2(unsigned char y, unsigned char x, char *buffer);
void Lcd_Chr_CP(char data);
void Lcd_Cmd(unsigned char data);

void Lcd_Init(void){
unsigned char data;
TRISRB4 = 0;
TRISRB3 = 0;
TRISRB2 = 0;
TRISRB1 = 0;
TRISEN = 0;
TRISRS = 0;
LCD_RD4 = 0;
LCD_RD3 = 0;
LCD_RD2 = 0;
LCD_RD1 = 0;
LCD_EN = 0;
LCD_RS = 0;
__delay_us(5500);
__delay_us(5500);
__delay_us(5500);
__delay_us(5500);
__delay_us(5500);
__delay_us(5500);
for(data = 1; data < 4; data ++)
{
    LCD_RD4 = 0;    LCD_RD3 = 0;    LCD_RD2 = 1;    LCD_RD1 = 1;    LCD_EN = 0;
    LCD_RS = 0;    LCD_RD4 = 0;    LCD_RD3 = 0;    LCD_RD2 = 1;    LCD_RD1 = 1;
    LCD_EN = 1;    LCD_RS = 0;
    __delay_us(5);
    LCD_RD4 = 0;    LCD_RD3 = 0;    LCD_RD2 = 1;    LCD_RD1 = 1;    LCD_EN = 0;
    LCD_RS = 0;
    __delay_us(5500);
}
LCD_RD4 = 0; LCD_RD3 = 0; LCD_RD2 = 1; LCD_RD1 = 0; LCD_EN = 0; LCD_RS = 0;
LCD_RD4 = 0; LCD_RD3 = 0; LCD_RD2 = 1; LCD_RD1 = 0; LCD_EN = 1; LCD_RS = 0;
__delay_us(5);
LCD_RD4 = 0; LCD_RD3 = 0; LCD_RD2 = 1; LCD_RD1 = 0; LCD_EN = 0; LCD_RS = 0;
__delay_us(5500);
data = 40; Lcd_Cmd(data);
data = 16; Lcd_Cmd(data);
data = 1;  Lcd_Cmd(data);
data = 15; Lcd_Cmd(data);
}


void Lcd_Out(unsigned char y, unsigned char x, const char *buffer)
{
unsigned char data;
switch (y)
{   
    case 1: data = 128 + x; break;
    case 2: data = 192 + x; break;
    case 3: data = 148 + x; break;
    case 4: data = 212 + x; break;
    default: break;
}
Lcd_Cmd(data);
while(*buffer)              // Write data to LCD up to null
{                
    Lcd_Chr_CP(*buffer);
    buffer++;               // Increment buffer
}
return;
}


void Lcd_Out2(unsigned char y, unsigned char x, char *buffer)
{
unsigned char data;
switch (y)
{
    case 1: data = 128 + x; break;
    case 2: data = 192 + x; break;
    case 3: data = 148 + x; break;
    case 4: data = 212 + x; break;
    default: break;
}
Lcd_Cmd(data);
while(*buffer)              // Write data to LCD up to null
{                
    Lcd_Chr_CP(*buffer);
    buffer++;               // Increment buffer
}
return;
}


void Lcd_Chr_CP(char data){
LCD_EN = 0; LCD_RS = 1;
LCD_RD4 = (data & 0b10000000)>>7; LCD_RD3 = (data & 0b01000000)>>6;
LCD_RD2 = (data & 0b00100000)>>5; LCD_RD1 = (data & 0b00010000)>>4;
_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
LCD_RD4 = (data & 0b00001000)>>3; LCD_RD3 = (data & 0b00000100)>>2;
LCD_RD2 = (data & 0b00000010)>>1; LCD_RD1 = (data & 0b00000001);
_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
__delay_us(5); __delay_us(5500);
}


void Lcd_Cmd(unsigned char data){
LCD_EN = 0; LCD_RS = 0;
LCD_RD4 = (data & 0b10000000)>>7; LCD_RD3 = (data & 0b01000000)>>6;
LCD_RD2 = (data & 0b00100000)>>5; LCD_RD1 = (data & 0b00010000)>>4;
_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
LCD_RD4 = (data & 0b00001000)>>3; LCD_RD3 = (data & 0b00000100)>>2;
LCD_RD2 = (data & 0b00000010)>>1; LCD_RD1 = (data & 0b00000001);
_delay(10);
LCD_EN = 1; __delay_us(5); LCD_EN = 0;
__delay_us(5500);//Delay_5us();
}
#endif
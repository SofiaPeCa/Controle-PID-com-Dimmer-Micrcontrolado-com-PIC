/*
 * File:   mainDeb.c
 * Author: Sofia Pereira Campos
 *
 * Created on 12 de Novembro de 2020, 12:13
 */
#include "Config_Inicial.h"
#include <xc.h>
#include "flexlcd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //para usar funçoes de string deve se adicionar este header
#include "max6675.h"
#include "controle.h"
#include "atraso.h"

#define _XTAL_FREQ 8000000

/*-------------------------------------------------------------------
 Variáveis Globais
-------------------------------------------------------------------*/
float setpoint;
float temperatura;
int valorl;
double PIDa; // controlador
int Aux;
void main(void) 
{
    /*-------------------------------------------------------------------
    ESTADO INICIAL REGISTRADORES
    -------------------------------------------------------------------*/
     OSCCONbits.IRCF2 = 1 ;  
     OSCCONbits.IRCF1 = 1;  //Frequência so oscilador interno de 8Mhz
     OSCCONbits.IRCF0 = 1;
     PIE1 = 0x00;  //Interrupções desabilitadas
     PIE2 = 0x00;  //Interrupções desabilitadas
     PIR1 = 0x00;  //Flags não transbordaram
     PIR2 = 0x00;  //Flags não transbordaram
     
     ANSEL = 0x00;  //Todos os pinos de entrada e saída digital
     ANSELH = 0x00; //Todos os pinos de entrada e saída digital
     WPUB = 0x00;
     IOCB = 0x00;
     
     T1CON = 0X00;
     TMR1H = 0X00;
     TMR1L = 0X00;
     T2CON = 0X00;
     PR2 = 0X00;
     TMR2 = 0X00;
     
     CM1CON0 = 0X00;   //CONFIG COMPARARDOR 1 - DESABILITADO
     CM2CON0 = 0X00;   //CONFIG COMPARADOR 2 - DESABILITADO
     CM2CON1= 0X00;
     SRCON = 0X00;
     VRCON= 0X00;

     
     ADCON0 = 0X00;
     ADCON1= 0X00;
     ADRESH = 0X00;
     ADRESL = 0X00;

     
     CCP1CON   = 0X00;   //P1A modulado  P1B,P1C,P1D são portas
     CCPR1H    = 0X00;
     CCPR1L   = 0X00;
     CCP2CON   = 0X00;
     CCPR2H  = 0X00;
     CCPR2L   = 0X00;
    
    /*-------------------------------------------------------------------
     ESTADO INICIAL DAS PORTAS
    -------------------------------------------------------------------*/
    PORTA = 0;           //Limpa PORTA
    PORTB = 0;           //Limpa PORTB
    PORTC = 0;           //Limpa PORTC
    /*-------------------------------------------------------------------
      SELEÇÃO DE SAÍDA OU ENTRADA
    -------------------------------------------------------------------*/
     TRISA = 0b00000001; //Todas as portas são saídas menos RA0
     TRISB = 0b10000000; //Todas as portas são saídas menos RB7
     TRISC = 0b00010000; //Todas as portas são saídas menos RC4
     
     /*-------------------------------------------------------------------
      ESTADO INICIAL PARA LEITURA DO MAX6675
    -------------------------------------------------------------------*/
     Cs=1;
     Sck=0; 
   
     /*-------------------------------------------------------------------
      ROTINA DE CÓDIGO PRINCIPAL
     -------------------------------------------------------------------*/
    Lcd_Init();                     //Inicia LCD
    Lcd_Cmd(LCD_CURSOR_OFF);        //Desliga Cursor
    Lcd_Cmd(LCD_CLEAR);        //Desliga Cursor
    setpoint = 120;             //Seta setpoint
    PIDa = 0;
    Aux = 0;
    for(;;){ 
        if(PORTAbits.RA0){
          atraso(PIDa);
          PORTCbits.RC1=1;
          __delay_us(164);
          PORTCbits.RC1=0;
          valorl=le_max6675();
          temperatura = mostra_no_LCD(valorl);
        }
        if(Aux == 15000){
            calc_erro(temperatura,setpoint);
            PIDa = controlador();   
            Aux = 0;
        }  
        Aux = Aux +1;
    }
}







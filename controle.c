//CÛdigo Adaptado de: 

//Carrinho seguidor de linha com controlador PID- Cleyson e Kassia

/*
Esse codigo consiste no controle de um carrinho seguidor de linha por 
meio da leitura de uma placa de 5 sensores infraverelho 
presente no carrinho e um controlador PID a apartir de sua leitura
Configura√ßao do erro com relacao a leitura dos sensores

Controlador PID:
PID = (Kp*P)+(Ki*I)+(Kd*D), onde Kp, Ki e Kd sao completamente ajustaveis 
e foram encontradas por meio de tentativa e erro
*/

#include "pwm.h"
#include "controle.h"
#include <stdio.h>
#include <xc.h>
#define _XTAL_FREQ 8000000


// Valores das constantes de controle encontadas pelo mÈtodo da curva reaÁ„o
// podendo variar de acordo com o projeto
#define Kp 1
#define	Ki 0.00050
#define	Kd 0.5


//Criancao das constantes do controlador e iniciando-as em 0
double PID; // controlador
float I = 0; // integral
float P = 0; // proporcional 
float D = 0; // derivativo

// CALCULO DO ERRO A PARTIR DA LEITURA DOS SENSORES

float erro = 0;
float erroanterior = 0;

// funcao para calcular o erro a partir das variaveis de leitura de temperatura
void calc_erro(float temp, float setpoint) {
    erro = setpoint - temp ;  //Definir setpoint com ajuste de temperatura de cada ciclo
}


// CALCULO DO CONTROLADOR PID A PARTIR DO ERRO

// Funcao para calcular o controlador PID, bem como suas variaveis
double controlador(){
	P = erro;        
	I = I + erro;                  
	D = erro - erroanterior;          
	erroanterior = erro;
	PID = (Kp*P)+(Ki*I)+(Kd*D);
    if(PID>100){
        PID = 100;
    }else if(PID< 0){
        PID = 0;
    }
    return PID;
}



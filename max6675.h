
//define pinos de comunica��o com o Max6675
#define   So       PORTCbits.RC4
#define   Sck      PORTCbits.RC3  
#define   Cs       PORTAbits.RA5


unsigned int  le_max6675();
float mostra_no_LCD(int  value);

//vari�veis globais
 int dig3=0,dig2=0,dig1=0,dig0=0;
 int dig3a=0,dig2a=0,dig1a=0,dig0a=0,digdec=0,digcent=0;
 float temp;
 char dlyread=0;
 char dlyread1=0;
 char dlympx=0;
 int  valor=0;
 char str2[10];
 char grau = 223;
 __bit f_conv_ok=0; //flag de convers�o n�o realizada
 __bit fsts=0;//flag status 'existe sensor ou n�o ligado'


//==============================================================================
//         ROTINA DE LEITURA DOS DADOS DO MAX6675
//==============================================================================

unsigned int  le_max6675(){
  char a;
  int  dado;   
 #define DELAY_ENTRE_CONVERCOES 0 //Aprox. 0,5 seg
   //temporiza de forma din�mica os intervalos necess�rios para a convers�o
   //m�nimo 250 ms
     dlyread++;
     if(dlyread==0){
        dlyread1++;
        if(dlyread1<DELAY_ENTRE_CONVERCOES){         
          return 0xffff;
        }
     }
     else{ return 0xffff;}
    dlyread1=0;
   //inicia a comunica��o com o Max6675
    dado=0;
    Cs=0; //Para qualquer sinal de convers�o imediatamente 
    for(a=0;a<16;a++){//16 bits
      Sck=1;//clock=1   //Ler os resultados de S0
      dado=dado<<1;//desloca para esquerda
      if(So)dado=dado|1;//se '1' seta bit0 de dado
      Sck=0;//clock=0 //Encerra leitura
    }

   Cs=1;//libera para novas conver��es de temperatura


   if(dado & 4)fsts=1; else fsts=0;//termopar status: 1 = aberto e 0 = conectado
   dado=dado>>3;//ajusta para direita 3 casa

   f_conv_ok=1;//sucesso na conver��o da temperatura
   return dado;

}
//==============================================================================
//     ROTINA DE CONVERS�O E APRESENTA��O DOS DADOS NO LCD
//==============================================================================
float mostra_no_LCD(int  valor){
    if(f_conv_ok==1){//se '1' ent�o  tem uma conver��o
       f_conv_ok=0;
       digcent=0;
       digdec=0;
       Lcd_Out(1,0,"Temp:"); 
       
       if(valor & 1)digcent=1;//obtem valor da segunda casa p�s v�rgula
       if(valor & 2)digdec=1;//obt�m valor da primeira casa p�s v�rgula

       if((digcent==1) && (digdec==1)){//converte p/ valores corretos
         digdec=7;
         digcent=5;
       }
       else if((digcent==1) && (digdec==0)){//converte p/ valores corretos
         digdec=5;
         digcent=0;
       }
       else if((digcent==0) && (digdec==1)){//converte p/ valores corretos
         digdec=2;
         digcent=5;
       }

       valor=valor>>2;//divide por 4 (lsb=0,25)

       if(valor>=1000){ //valores iguais ou acima de 1000 graus C�     
         dig3a=valor/1000;//converte para decimal nos d�gitos auxiliares
         valor=valor%1000;     
         dig2a=valor/100;
         valor=valor%100;                       
         dig1a=valor/10;                
         dig0a=valor%10;
         temp = ((dig3a*1000)+(dig2a*100)+(dig1a*10)+dig0a);
         sprintf(str2, "%d",dig3a);
         Lcd_Out(1,5,str2); 
         sprintf(str2, "%d",dig2a);
         Lcd_Out(1,6,str2);
         sprintf(str2, "%d",dig1a);
         Lcd_Out(1,7,str2);
         sprintf(str2, "%d%cC",dig0a,grau);
         Lcd_Out(1,8,str2);
         Lcd_Out(1,9,"");
       }
       else{
         if(valor>=100){//valores iguais ou acima de 100 graus C�   
           dig3a=valor/100;
           valor=valor%100;         
           dig2a=valor/10;
           dig1a=valor%10;
           dig0a=digdec;
           temp = ((dig3a*100)+(dig2a*10)+(dig1a)+(dig0a*0.1));
           sprintf(str2, "%d",dig3a);
           Lcd_Out(1,5,str2); 
           sprintf(str2, "%d",dig2a);
           Lcd_Out(1,6,str2);
           sprintf(str2, "%d",dig1a);
           Lcd_Out(1,7,str2);
           sprintf(str2, ".%d%cC",dig0a,grau);
           Lcd_Out(1,8,str2);               
         }
         else{//valores abaixo de 100 graus C�   
             dig3a=valor/10;            
             dig2a=valor%10;            
             dig1a=digdec;
             dig0a=digcent; 
             temp = (dig3a*10)+(dig2a)+(dig1a*0.1)+(dig0a*0.01);
             sprintf(str2, "%d",dig3a);
             Lcd_Out(1,5,str2); 
             sprintf(str2, "%d",dig2a);
             Lcd_Out(1,6,str2);
             sprintf(str2, ".%d",dig1a);
             Lcd_Out(1,7,str2);
             sprintf(str2, "%d%cC",dig0a,grau);
             Lcd_Out(1,8,str2);
             Lcd_Out(1,9,"");
         }      
      }
       
       if(fsts==1){//verifica se o termopar est� interrompido ou desconectado
       dig3a=0x4E;//'n' envia mensagem 'noSE' 
       dig2a=0x4F;//'o'
       dig1a=0x53; //'S'
       dig0a=0x45;//'E'   
       }       
       //__delay_ms(1000);
       
     }
    return temp;
    
}


    
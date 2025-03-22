/**
 * Créé par
 * Claude Barbaud
 * College de Maisonneuve
 * 16 février 2017
 * 
 * Modifié par
 * Maxime Champagne
 * 3 mars 2022
 * 
 * SPI/main.c
 * 
*/
#include <stdio.h>
#include "mcc_generated_files/mcc.h"

#define MAX 60
uint8_t const sin[MAX] ={
              254,254,252,249,244,238,231,222,213,202,
              191,179,167,154,141,127,114,101,88,76,
              64,53,42,33,24,17,11,6,3,1,
              0,1,3,6,11,17,24,33,42,53,
              64,76,88,101,114,128,141,154,167,179,
              191,202,213,222,231,238,244,249,252,254};

uint8_t const car[MAX] ={
             0,0,0,0,0,0,0,0,0,0,
			  0,0,0,0,0,0,0,0,0,0,
			  0,0,0,0,0,0,0,0,0,0,
			  255,255,255,255,255,255,255,255,255,255,
			  255,255,255,255,255,255,255,255,255,255,
			  255,255,255,255,255,255,255,255,255,255};

uint8_t const tri[MAX] ={
            9,17,26,34,43,51,60,68,77,85,
			 94,102,111,119,128,136,145,153,162,170,
			 179,187,196,204,213,221,230,238,247,255,
			 247,238,230,221,213,204,196,187,179,170,
			 162,153,145,136,128,119,111,102,94,86,
			 77,68,60,51,43,34,26,17,9,0};

unsigned long TimerStartValue = 65034; //valeur de timer pour être a 20Hz
uint8_t lecture; //les caracteres seront mis dans lecture
uint8_t last_lecture; //valeur de lecture précédente (entre T, S et C)
// 0xFE0A = 20Hz
// 0xFF0E = 40Hz
void out_dig(uint8_t x);
void sinus_60(void);
void car_60(void);
void tri_60(void);
void myTimer1_ISR(void);
void FrequencyChangeSin(void);//Cette fonction permet de changer la frequence pendant qu'on est dans une onde sin
void FrequencyChangeCar(void);//Cette fonction permet de changer la frequence pendant qu'on est dans une onde car
void FrequencyChangeTri(void);//Cette fonction permet de changer la frequence pendant qu'on est dans une onde tri

/*
                         Main application
 */
void main(void)
{
    int write_20Hz_loop = 0; //simplement pour écrire 20Hz au début du programme
    uint8_t valeur;
    float tension;
    
    SYSTEM_Initialize();
    
    INTERRUPT_GlobalInterruptEnable();
    
    INTERRUPT_PeripheralInterruptEnable();
    
    TMR1_SetInterruptHandler(myTimer1_ISR);
   

    SSPCON1bits.SSPEN = 1;
    IO_RA5_SetHigh();
    
    while (1)
    {
        
        
        //Code de test pour valider le fonctionnement du potentiomètre
//        {
//            printf("\n\rEntrer une valeur entre 0 et 255, suivie de [Enter]");
//            valeur = 0;
//            do
//            {
//                do
//                {
//                    
//                    lecture = EUSART1_Read();
//                                       
//                }
//                while (((lecture < '0') || (lecture > '9')) && (lecture != 0x0d));
//                if ((lecture >= '0') && (lecture <= '9')) 
//                {
//                    valeur = 10 * valeur + lecture - '0';
//                    putchar(lecture);
//                }
//            }
//        
//            while ((lecture != 0x0d) && (valeur < 26)); 
//            tension = (float)5* valeur /256;
//            printf("\n\rValeur = %u tension = %3.2f ", valeur, tension);
//            out_dig(valeur);    // envoi sur potentiometre 
//        } 
        vide_ecran();
        curseurPosition(0x00);
        printf("Onde:");
        curseurPosition(0x40);
        printf("Frequence:");
        while (!EUSART1_is_rx_ready){
        }
        while(1)
        {
            lecture = EUSART1_Read();
            if (lecture == 'S'){     //si on lit la valeur S, on va rentrer dans la fonction qui permet de changer la frequence 
                last_lecture = 'S';
                curseurPosition(0x06);
                printf("Sin");
                if (write_20Hz_loop == 0){
                    curseurPosition(0x4B);
                    printf("20Hz ");
                    write_20Hz_loop++;
                }   
                while (lecture == 'S'){
                    FrequencyChangeSin();
                } 
            }
            if (lecture == 'C'){    //si on lit la valeur S, on va rentrer dans la fonction qui permet de changer la frequence   
                last_lecture = 'C';
                curseurPosition(0x06);
                printf("Car");
                if (write_20Hz_loop == 0){
                    curseurPosition(0x4B);
                    printf("20Hz ");
                    write_20Hz_loop++;
                }   
                while (lecture == 'C'){
                    FrequencyChangeCar();
                } 
            }
            if (lecture == 'T'){    //si on lit la valeur S, on va rentrer dans la fonction qui permet de changer la frequence   
                last_lecture = 'T';
                curseurPosition(0x06);
                printf("Tri");
                if (write_20Hz_loop == 0){
                    curseurPosition(0x4B);
                    printf("20Hz ");
                    write_20Hz_loop++;
                }   
                while (lecture == 'T'){
                    FrequencyChangeTri();
                } 
            }
            if (lecture != 'T'|'S'|'C'|'+'|'-') // si la lecture est une valeur autre que une qu'on s'attend, on prend une des valeurs précédentes 
            {
                lecture = last_lecture;
            }
        }
            
        
    }

           
}

//---------------------------------------------------------------
// Routine d'interruption du Timer1
//---------------------------------------------------------------
void myTimer1_ISR(void){
    static uint8_t i; 
    
    TMR1_WriteTimer(TimerStartValue);
    if (lecture == 'S'){
    out_dig(sin[i]);
    }
    if (lecture == 'C'){
    out_dig(car[i]);
    }
    if (lecture == 'T'){
    out_dig(tri[i]);
    }
    
    i++;
    if (i==MAX){
        i=0;
    }
}  
//----------------------------------------------------------------
// Transmission au pot. d'une onde comprenant 60 points par cycle.
//----------------------------------------------------------------
void sinus_60(void) {
    uint8_t i;
    while(1) {
        for (i=0;i<MAX;i++) {
            out_dig(sin[i]);

            }
        } 
}

void car_60(void) {
    uint8_t i;
    while(1) {
        for (i=0;i<MAX;i++) {
            out_dig(car[i]);

            }
        } 
}

void tri_60(void) {
    uint8_t i;
    while(1) {
        for (i=0;i<MAX;i++) {
            out_dig(tri[i]);

            }
        } 
}


//----------------------------------------------------------------
//  Transmission d'une donnee a la sortie du pot. numerique
//----------------------------------------------------------------
void out_dig(uint8_t x)
{
	IO_RA5_SetLow();   // selection du potentiometre
	SPI_ExchangeByte(0x11);  // ecriture, pot. 0
	SPI_ExchangeByte(x);
	IO_RA5_SetHigh();
	//__delay_ms(1);
}

void FrequencyChangeSin(void) //cette fonction premet de passer de 20Hz, 40Hz, 60Hz,80Hz et 100Hz dans une sin
{
    lecture = EUSART1_Read();
    if (lecture == '+'){ //+0x0104 comptes
        lecture = 'S';
        int loop = 0;
                if (TimerStartValue == 65034 & loop == 0){
                    TimerStartValue = 65294;
                    curseurPosition(0x4B);
                    printf("40Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65294 & loop == 0){
                    TimerStartValue = 65380;
                    curseurPosition(0x4B);
                    printf("60Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65380 & loop == 0){
                    TimerStartValue = 65424;
                    curseurPosition(0x4B);
                    printf("80Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65424 & loop == 0){
                    TimerStartValue = 65450;
                    curseurPosition(0x4B);
                    printf("100Hz");
                    loop ++;
                }
    }
            if (lecture == '-'){ //-0x0104 comptes
                lecture = 'S';
                int loop = 0;
                if (TimerStartValue == 65450 & loop == 0){
                    TimerStartValue = 65424;
                    curseurPosition(0x4B);
                    printf("80Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65424 & loop == 0){
                    TimerStartValue = 65380;
                    curseurPosition(0x4B);
                    printf("60Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65380 & loop == 0){
                    TimerStartValue = 65294;
                    curseurPosition(0x4B);
                    printf("40Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65294 & loop == 0){
                    TimerStartValue = 65034;
                    curseurPosition(0x4B);
                    printf("20Hz" );
                    loop ++;
                }
            }
}
void FrequencyChangeCar(void)//cette fonction premet de passer de 20Hz, 40Hz, 60Hz,80Hz et 100Hz dans un car
{
    lecture = EUSART1_Read();
    if (lecture == '+'){ //+0x0104 comptes
        lecture = 'C';
        int loop = 0;
                if (TimerStartValue == 65034 & loop == 0){
                    TimerStartValue = 65294;
                    curseurPosition(0x4B);
                    printf("40Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65294 & loop == 0){
                    TimerStartValue = 65380;
                    curseurPosition(0x4B);
                    printf("60Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65380 & loop == 0){
                    TimerStartValue = 65424;
                    curseurPosition(0x4B);
                    printf("80Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65424 & loop == 0){
                    TimerStartValue = 65450;
                    curseurPosition(0x4B);
                    printf("100Hz");
                    loop ++;
                }
    }
            if (lecture == '-'){ //-0x0104 comptes
                lecture = 'C';
                int loop = 0;
                if (TimerStartValue == 65450 & loop == 0){
                    TimerStartValue = 65424;
                    curseurPosition(0x4B);
                    printf("80Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65424 & loop == 0){
                    TimerStartValue = 65380;
                    curseurPosition(0x4B);
                    printf("60Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65380 & loop == 0){
                    TimerStartValue = 65294;
                    curseurPosition(0x4B);
                    printf("40Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65294 & loop == 0){
                    TimerStartValue = 65034;
                    curseurPosition(0x4B);
                    printf("20Hz" );
                    loop ++;
                }
            }
}
void FrequencyChangeTri(void)//cette fonction premet de passer de 20Hz, 40Hz, 60Hz,80Hz et 100Hz dans une tri
{
    lecture = EUSART1_Read();
    if (lecture == '+'){ //+0x0104 comptes
        lecture = 'T';
        int loop = 0;
                if (TimerStartValue == 65034 & loop == 0){
                    TimerStartValue = 65294;
                    curseurPosition(0x4B);
                    printf("40Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65294 & loop == 0){
                    TimerStartValue = 65380;
                    curseurPosition(0x4B);
                    printf("60Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65380 & loop == 0){
                    TimerStartValue = 65424;
                    curseurPosition(0x4B);
                    printf("80Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65424 & loop == 0){
                    TimerStartValue = 65450;
                    curseurPosition(0x4B);
                    printf("100Hz");
                    loop ++;
                }
    }
            if (lecture == '-'){ //-0x0104 comptes
                lecture = 'T';
                int loop = 0;
                if (TimerStartValue == 65450 & loop == 0){
                    TimerStartValue = 65424;
                    curseurPosition(0x4B);
                    printf("80Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65424 & loop == 0){
                    TimerStartValue = 65380;
                    curseurPosition(0x4B);
                    printf("60Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65380 & loop == 0){
                    TimerStartValue = 65294;
                    curseurPosition(0x4B);
                    printf("40Hz ");
                    loop ++;
                }
                if (TimerStartValue == 65294 & loop == 0){
                    TimerStartValue = 65034;
                    curseurPosition(0x4B);
                    printf("20Hz" );
                    loop ++;
                }
            }
}
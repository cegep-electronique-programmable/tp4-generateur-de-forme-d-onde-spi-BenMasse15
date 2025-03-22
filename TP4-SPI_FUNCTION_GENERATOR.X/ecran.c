#include "ecran.h"
#include "mcc_generated_files/eusart1.h"

void ecranAllume(void) //allumer l'ecran
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x41);
}

void ecranEteint(void) //eteint l'ecran
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x42);
}
curseurPosition(int position)//puts cursor on a position between 0x00 to 0x67
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x45);
    EUSART1_Write(position);
}
Blinking_ON()//fait clignoter le curseur
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x4B); 
}
Blinking_OFF()//eteint le clignotement du curseur
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x4C); 
}
vide_ecran()//vide l'ecran
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x51);
}
ecrireCaractere(char caractere)//permet d'ecrire
{
    EUSART1_Write (caractere);
}        
Backspace(int x)//efface un caractere
{
    for (int i=0;i<x;i++)
    {
    EUSART1_Write(0xFE);
    EUSART1_Write(0x4E);
    }
}
Backlight()
{
    EUSART1_Write(0xFE);
    EUSART1_Write(0x53);
    EUSART1_Write(8);
}
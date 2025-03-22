/*
* ecran.h
* Contient toutes les fonctions pour interagir avec l'ecran modele
* NHD-0420D3Z-FL-GBW
*
* Auteur : Philippe Lefebvre
*/

#ifndef ECRAN_H
#define ECRAN_H

/***********Includes************/


/************Defines************/


/*********Declarations**********/

/*
* Fonction : ecranEteint
* Description : Envoie la commande pour eteindre l'ecran
*
* Params : Aucun
* 
* Retour : Aucun
*/
void ecranEteint(void);
/*
* Fonction : curseurPosition
* Description : Permet de deplacer le curseur
*
* Params : position
* 
* Retour : Aucun
*/
curseurPosition(int position);
/*
* Fonction : vide_ecran
* Description : vide l'ecran
*
* Params : Aucun
* 
* Retour : Aucun
*/
vide_ecran();
/*
* Fonction : ecrireCaractere
* Description : Envoie un ou des caracteres sur l'ecran
*
* Params : caractere
* 
* Retour : Aucun
*/
ecrireCaractere(char caractere);
/*
* Fonction : Blinking_ON
* Description : allume le clignotement du curseur
*
* Params : Aucun
* 
* Retour : Aucun
*/
Blinking_ON();
/*
* Fonction : Blinking_OFF
* Description : Eteint le clignotement du curseur
*
* Params : Aucun
* 
* Retour : Aucun
*/
Blinking_OFF();
/*
* Fonction : Backspace
* Description : Supprime des caracteres
*
* Params : Backspace
* 
* Retour : Aucun
*/
Backspace(int x);


Backlight();

#endif
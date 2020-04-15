#ifndef GEFFE_H
#define GEFFE_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define R_SIZE 16
#define S_SIZE 16

//############################# Struct #############################//

//Représente un registre ayant une taille de 16 bits
struct Register
{
    int value[R_SIZE];
};
typedef struct Register REGISTER;

//Représente un lfsr qui possède un registre et des coefficients de rétroaction
struct lfsr
{
    REGISTER L;
    int coefficient[R_SIZE];
};
typedef struct lfsr LFSR;

//Représente une sous-clé de 16 bits qui compose la clé principale K, sert à l'initialisation d'un registre
struct subkey
{
    int value[S_SIZE];
};
typedef struct subkey SUBKEY;

//Représente la clé principale de 48 bits, composé de 3 sous-clés
struct key
{
    SUBKEY subkey1;
    SUBKEY subkey2;
    SUBKEY subkey3;
};
typedef struct key KEY;

//Représente la suite chiffrante, ayant une taille définie parl'utilisateur et un ensemble de valeur
struct ciphersuite
{
    int size;
    int* value;
};
typedef struct ciphersuite CIPHERSUITE;

//Représente le générateur de Geffe, composé d'une clé K de 48 bits, de 3 lfsr et de la fonction de filtrage
struct geffe
{
    KEY key;
    LFSR lfsr1;
    LFSR lfsr2;
    LFSR lfsr3;
    int F[8];
};
typedef struct geffe GEFFE;


//############################# Function #############################//


//Initialise les 3 sous clès ki de 16 bits à partir de la clès K de 48 bits saisie par l'utilisateur
void initKey(char* _argv, KEY* _key);

//Initialise les coefficients de rétroaction de chaque LFSR
void initCoefficient(LFSR* _lfsr1, LFSR* _lfsr2, LFSR* _lfsr3);

//Initialise les 3 registres du LFSR à partir des 3 sous clès
void initRegister(GEFFE* _geffe);

//Initialise la fonction de filtrage à partir des saisies de l'utilisateur
void initF(int _F[], char *_argv[]);

//Applique la fonction de filtrage sur 3 bits d'entrées (x0x1x2) pour fournir 1 bit de la suite chiffrante (si)
int filtrage(int _x1, int _x2, int _x3, int _F[]);

//Réalise le décalage des bits au sein d'un LFSR à chaque tour d'horloge
void offset(LFSR* _lfsr);

//Calcul les corrélations entre x0x1x2 (2³ = 8 combinaison possible) et F(x0x1x2)
void correlation(float _correlation[],int _F[8]);

//Calcul la corrélation entre une sous-clé ki (parmis les 2^16 possibilités de sous-clés) avec la suite chiffrante
float isGoodKey(SUBKEY* _subkey, CIPHERSUITE* _suite);

//Permet de tester toute les valeurs possibles pour une sous-clé, en cheangant la valeur de celle-ci (2¹⁶ = 65 536 combinaisons)
void brutForce(SUBKEY* _subkey, int* i);

//Initialise une sous-clé avec une valeur par défault avant le début de la recherche exhaustive 
void subkeyAsDefaultValue(SUBKEY* _subkey);

//Test si la corrélation trouver avec la fonction isGoodKey() est égale à cele trouver avec la fonction corrélation, si oui renvoi la sous-clé actuel sinon test avec une autre sous-clé
SUBKEY findSubkey(float _correlation, CIPHERSUITE* _suite);

//Lance une attaque par corrélation de type diviser pour régner pour récuperer les 3 sous-clés indépendamment
KEY attaque(float _correlation[3], CIPHERSUITE* _suite);

//Construit le génerateur, produit la suite chiffrante, réalise les corrélations avec F(x0x1x2) et permet de réaliser l'attaque sur celui-ci
void Generate(char* _filtrageArg[], char* _keyArg, int _n);


#endif
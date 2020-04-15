#include "../lib/geffe.h"


//############################# Function #############################//


//Initialise les 3 sous clès ki de 16 bits à partir de la clès K de 48 bits saisie par l'utilisateur
void initKey(char* _argv, KEY* _key)
{
    
    int tmpInt;

    for (int i = 0; i < 16; i++)
    {
        tmpInt = (int)_argv[i];

        if (tmpInt == 48) _key->subkey1.value[i] = 0;
        else              _key->subkey1.value[i] = 1;
    }

    for (int j = 16; j < 32; j++)
    {
        tmpInt = (int)_argv[j];

        if (tmpInt == 48) _key->subkey2.value[j-16] = 0;
        else              _key->subkey2.value[j-16] = 1;
    }

    for (int k = 32; k < 48; k++)
    {
        tmpInt = (int)_argv[k];

        if (tmpInt == 48) _key->subkey3.value[k-32] = 0;
        else              _key->subkey3.value[k-32] = 1;
    }
}

//Initialise les coefficients de rétroaction de chaque LFSR
void initCoefficient(LFSR* _lfsr1, LFSR* _lfsr2, LFSR* _lfsr3)
{
    _lfsr1->coefficient[0] = 0;
    _lfsr1->coefficient[1] = 0;
    _lfsr1->coefficient[2] = 0;
    _lfsr1->coefficient[3] = 0;
    _lfsr1->coefficient[4] = 0;
    _lfsr1->coefficient[5] = 0;
    _lfsr1->coefficient[6] = 0;
    _lfsr1->coefficient[7] = 0;
    _lfsr1->coefficient[8] = 1;
    _lfsr1->coefficient[9] = 0;
    _lfsr1->coefficient[10] = 0;
    _lfsr1->coefficient[11] = 1;
    _lfsr1->coefficient[12] = 0;
    _lfsr1->coefficient[13] = 0;
    _lfsr1->coefficient[14] = 1;
    _lfsr1->coefficient[15] = 1;

    _lfsr2->coefficient[0] = 0;
    _lfsr2->coefficient[1] = 0;
    _lfsr2->coefficient[2] = 0;
    _lfsr2->coefficient[3] = 0;
    _lfsr2->coefficient[4] = 1;
    _lfsr2->coefficient[5] = 0;
    _lfsr2->coefficient[6] = 0;
    _lfsr2->coefficient[7] = 0;
    _lfsr2->coefficient[8] = 1;
    _lfsr2->coefficient[9] = 0;
    _lfsr2->coefficient[10] = 0;
    _lfsr2->coefficient[11] = 0;
    _lfsr2->coefficient[12] = 0;
    _lfsr2->coefficient[13] = 0;
    _lfsr2->coefficient[14] = 1;
    _lfsr2->coefficient[15] = 1;

    _lfsr3->coefficient[0] = 0;
    _lfsr3->coefficient[1] = 0;
    _lfsr3->coefficient[2] = 0;
    _lfsr3->coefficient[3] = 0;
    _lfsr3->coefficient[4] = 0;
    _lfsr3->coefficient[5] = 0;
    _lfsr3->coefficient[6] = 0;
    _lfsr3->coefficient[7] = 0;
    _lfsr3->coefficient[8] = 0;
    _lfsr3->coefficient[9] = 0;
    _lfsr3->coefficient[10] = 1;
    _lfsr3->coefficient[11] = 0;
    _lfsr3->coefficient[12] = 1;
    _lfsr3->coefficient[13] = 1;
    _lfsr3->coefficient[14] = 0;
    _lfsr3->coefficient[15] = 1;
}

//Initialise les 3 registres du LFSR à partir des 3 sous clès
void initRegister(GEFFE* _geffe)
{
    for (int i = 0; i < S_SIZE; i++)
    {
        _geffe->lfsr1.L.value[i] = _geffe->key.subkey1.value[i];
        _geffe->lfsr2.L.value[i] = _geffe->key.subkey2.value[i];
        _geffe->lfsr3.L.value[i] = _geffe->key.subkey3.value[i];
    }

    initCoefficient(&_geffe->lfsr1, &_geffe->lfsr2, &_geffe->lfsr3);
}

//Initialise la fonction de filtrage à partir des saisies de l'utilisateur
void initF(int _F[], char *_argv[])
{
    _F[0] = atoi(_argv[1]);
    _F[1] = atoi(_argv[2]);
    _F[2] = atoi(_argv[3]);
    _F[3] = atoi(_argv[4]);
    _F[4] = atoi(_argv[5]);
    _F[5] = atoi(_argv[6]);
    _F[6] = atoi(_argv[7]);
    _F[7] = atoi(_argv[8]);
}

//Applique la fonction de filtrage sur 3 bits d'entrées (x0x1x2) pour fournir 1 bit de la suite chiffrante (si)
int filtrage(int _x1, int _x2, int _x3, int _F[])
{

    int xres = 0;

         if (_x1 == 0 && _x2 == 0 && _x3 == 0)  xres = _F[0];
    else if (_x1 == 1 && _x2 == 0 && _x3 == 0)  xres = _F[1];
    else if (_x1 == 0 && _x2 == 1 && _x3 == 0)  xres = _F[2];
    else if (_x1 == 1 && _x2 == 1 && _x3 == 0)  xres = _F[3];
    else if (_x1 == 0 && _x2 == 0 && _x3 == 1)  xres = _F[4];
    else if (_x1 == 1 && _x2 == 0 && _x3 == 1)  xres = _F[5];
    else if (_x1 == 0 && _x2 == 1 && _x3 == 1)  xres = _F[6];
    else if (_x1 == 1 && _x2 == 1 && _x3 == 1)  xres = _F[7];

    return xres;
}

//Réalise le décalage des bits au sein d'un LFSR à chaque tour d'horloge
void offset(LFSR* _lfsr)
{
    int temp = _lfsr->L.value[R_SIZE-1];

    for (int i = R_SIZE-1; i > 0; i--)
    {
        if (_lfsr->coefficient[i] == 1)
        {
            temp ^= _lfsr->L.value[i];
        }
    }

    for (int j = R_SIZE-1; j > 0; j--)
    {
        _lfsr->L.value[j] = _lfsr->L.value[j - 1];
    }

    _lfsr->L.value[0] = temp;
}

//Calcul les corrélations entre x0x1x2 (2³ = 8 combinaison possible) et F(x0x1x2)
void correlation(float _correlation[],int _F[8])
{
    float cptX1 = 0.;
    float cptX2 = 0.;
    float cptX3 = 0.;

    int F[8][4] = {
                    {0,0,0,_F[0]},
                    {1,0,0,_F[1]},
                    {0,1,0,_F[2]},
                    {1,1,0,_F[3]},
                    {0,0,1,_F[4]},
                    {1,0,1,_F[5]},
                    {0,1,1,_F[6]},
                    {1,1,1,_F[7]}
                  };

    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(j == 0 && F[i][j] == F[i][j+3]) cptX1+= 1.0;
            if(j == 1 && F[i][j] == F[i][j+2]) cptX2+= 1.0;
            if(j == 2 && F[i][j] == F[i][j+1]) cptX3+= 1.0;
        }
    }   

    _correlation[0] = (cptX1 / 8.) * 100.; 
    _correlation[1] = (cptX2 / 8.) * 100.; 
    _correlation[2] = (cptX3 / 8.) * 100.;   

    printf("\n\nThere is a %f %% correlation between x0 and F(x0x1x2)\n", _correlation[0]);
    printf("There is a %f %% correlation between x1 and F(x0x1x2)\n", _correlation[1]);         
    printf("There is a %f %% correlation between x2 and F(x0x1x2)\n", _correlation[2]);                 
}

//Calcul la corrélation entre une sous-clé ki (parmis les 2^16 possibilités de sous-clés) avec la suite chiffrante
float isGoodKey(SUBKEY* _subkey, CIPHERSUITE* _suite)
{
    float egality = 0.;
    float res = 0.;

    for(int i = 0; i < _suite->size; i++)
    {
        if(_subkey->value[_suite->size-i] == _suite->value[i]) egality += 1.0;
    }

    res = (egality/(float)_suite->size) *100.;

    return res;
}

//Permet de tester toute les valeurs possibles pour une sous-clé, en cheangant la valeur de celle-ci (2¹⁶ = 65 536 combinaisons)
void brutForce(SUBKEY* _subkey, int* i)
{        
    int nbr = *i;
    
    for(int j=15; j>=0; j--)
    {
        _subkey->value[j] = nbr%2;
        nbr = nbr/2;
    }

    *i = *i +1;
}

//Initialise une sous-clé avec une valeur par défault avant le début de la recherche exhaustive 
void subkeyAsDefaultValue(SUBKEY* _subkey)
{
    _subkey->value[0] = 0;
    _subkey->value[1] = 0;
    _subkey->value[2] = 0;
    _subkey->value[3] = 0;
    _subkey->value[4] = 0;
    _subkey->value[5] = 0;
    _subkey->value[6] = 0;
    _subkey->value[7] = 0;
    _subkey->value[8] = 0;
    _subkey->value[9] = 0;
    _subkey->value[10] = 0;
    _subkey->value[11] = 0;
    _subkey->value[12] = 0;
    _subkey->value[13] = 0;
    _subkey->value[14] = 0;
    _subkey->value[15] = 0;
}

//Test si la corrélation trouver avec la fonction isGoodKey() est égale à cele trouver avec la fonction corrélation, si oui renvoi la sous-clé actuel sinon test avec une autre sous-clé
SUBKEY findSubkey(float _correlation, CIPHERSUITE* _suite)
{
    SUBKEY subkey;
    int i = 0;

    subkeyAsDefaultValue(&subkey);

    while(isGoodKey(&subkey, _suite) != _correlation)
    {
        brutForce(&subkey, &i);

        if(i == 65536) break;
    }

    return subkey;
}

//Lance une attaque par corrélation de type diviser pour régner pour récuperer les 3 sous-clés indépendamment
KEY attaque(float _correlation[3], CIPHERSUITE* _suite)
{
    KEY key;

    key.subkey1 = findSubkey(_correlation[0], _suite);
    key.subkey2 = findSubkey(_correlation[1], _suite);
    key.subkey3 = findSubkey(_correlation[2], _suite);

    printf("\nSubkey1 found : = ");

    for(int i=0; i < _suite->size; i++)
    {
        printf("%d", key.subkey1.value[i]);
    }

    printf("\nSubkey2 found : = ");

    for(int j=0; j < _suite->size; j++)
    {
        printf("%d", key.subkey2.value[j]);
    }

    printf("\nSubkey3 found : = ");

     for(int k=0; k < _suite->size; k++)
    {
        printf("%d", key.subkey3.value[k]);
    }

    return key;
}

//Construit le génerateur et produit la suite chiffrante
void Generate(char* _filtrageArg[], char* _keyArg, int _n)
{
    GEFFE geffe;
    CIPHERSUITE suite;
    suite.size = _n;
    suite.value = malloc(_n * sizeof (int) );
    float _correlation[3] = {0.,0.,0.};

    initKey(_keyArg, &geffe.key);
    initRegister(&geffe);
    initF(geffe.F, _filtrageArg);

    for (int i = 0; i < suite.size; i++)
    {
        suite.value[i] = filtrage(geffe.lfsr1.L.value[R_SIZE-1], geffe.lfsr2.L.value[R_SIZE-1], geffe.lfsr3.L.value[R_SIZE-1], geffe.F);

        offset(&geffe.lfsr1);
        offset(&geffe.lfsr2);
        offset(&geffe.lfsr3);
    }

    printf("\nCIPHERSUITE IS := ");

    for(int j = 0; j < _n; j++)
    {
        printf("%d",suite.value[j]);
    }

    correlation(_correlation,geffe.F);

    attaque(_correlation, &suite);

    free(suite.value);
}


//############################# Main #############################//

int main(int argc, char** argv)
{
    Generate(argv, argv[9], atoi(argv[10]));

    return 0;
}

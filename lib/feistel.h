//
//  fiestel.h
//

#ifndef fiestel_h
#define fiestel_h

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int* dec_to_binaire(int nbr);
int rotation_gauche_7bits( int T );
int * chiffrement_par_bloc(int x0L, int x0R, int k0, int k1, int * chiffree);
int * cryptanalyse_1_tour(int clairL, int clairR, int chiffreL, int chiffreR, int * keys);
int * cryptanalyse_12_tours(int clairL, int clairR, int chiffreL, int chiffreR, int * keys);
void Affichage(int x0L, int x0R, int k0, int k1);
void Affichage_cryptanalyse_1_tour(int clairL, int clairR, int chiffreL, int chiffreR);

void Affichage_cryptanalyse_12_tours(int clairL, int clairR, int chiffreL, int chiffreR);


#endif /* fiestel_h */

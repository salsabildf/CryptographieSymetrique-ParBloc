#include <stdlib.h>
#include <stdio.h>


// Cette fonction permet initialiser les 256 fonctions de filtrages possibles dans un tableau à 2 dimensions
int ** init_filtrages_possibles(){
    
    // Déclaration de Tableau
    int** F = malloc(256 * sizeof (int*) );
    for (int i=0; i<256; i++) F[i] = malloc (8 * sizeof (int) );
    
    //On test si l'allocation a bien été effectué
    if(F == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
 
    //déclaration d'une variable temporaire
    int nbr =0;
    
    //on boucle de 0 à 256 pour incrémentant le i pour remplir le tableau
    for (int i=0; i<256; i++){
        
    //on affecte le i à la variable temporaire pour ne pas modifier l'incrémentation
       nbr =i;
        
        // on boucle de 7 à 0 pour remplir le tableau à l'endroit. Passage du décimal au  binaire.
        for(int j=7; j>=0; j--){
            //on récupère le binaire en faisant un modulo 2 sur la variable i (nbr)
            F[i][j]= nbr%2;
           //on divise nbr pour avancer dans le binaire vers la droite
            nbr = nbr/2;
        }
    }
    return F;
}


// Cette fonction permet initialiser les 8 combinaisons possibles des x0x1x2 dans un tableau à 2 dimensions
int ** init_x0x1x2(){
      // Déclaration de Tableau
       int** tab_x0x1x2= malloc(8 * sizeof (int*) );
       for (int i=0; i<8; i++) tab_x0x1x2[i] = malloc (3 * sizeof (int) );
       
       //On test si l'allocation a bien été effectué
       if(tab_x0x1x2== NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
    
        // Remplissage du tableau, car si on fait avec les modulo il ne seront pas dans le même ordre pour chaque valeurs possibles fi de la fonction de filtrage F
         tab_x0x1x2[0][0]=0;  tab_x0x1x2[0][1]=0;  tab_x0x1x2[0][2]=0;
         tab_x0x1x2[1][0]=1;  tab_x0x1x2[1][1]=0;  tab_x0x1x2[1][2]=0;
         tab_x0x1x2[2][0]=0;  tab_x0x1x2[2][1]=1;  tab_x0x1x2[2][2]=0;
         tab_x0x1x2[3][0]=1;  tab_x0x1x2[3][1]=1;  tab_x0x1x2[3][2]=0;
         tab_x0x1x2[4][0]=0;  tab_x0x1x2[4][1]=0;  tab_x0x1x2[4][2]=1;
         tab_x0x1x2[5][0]=1;  tab_x0x1x2[5][1]=0;  tab_x0x1x2[5][2]=1;
         tab_x0x1x2[6][0]=0;  tab_x0x1x2[6][1]=1;  tab_x0x1x2[6][2]=1;
         tab_x0x1x2[7][0]=1;  tab_x0x1x2[7][1]=1;  tab_x0x1x2[7][2]=1;
     
            
        
    return tab_x0x1x2;

}


// Cette fonction permet de retourner la correlation entre x0 , x1, X2 et le bit de filtrage correspondant
int ** correlation( ){
    // Déclaration de Tableau
    int** tab_correlation = malloc(256 * sizeof (int*) );
    for (int i=0; i<256; i++) tab_correlation[i] = malloc (3 * sizeof (int) );
    
    //On test si l'allocation a bien été effectué
    if(tab_correlation == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
    
         //on récupère le tableau des combinaisons de x0x1x2 possibles
          // Déclaration de Tableau
          int** tab_bits = malloc(8 * sizeof (int*) );
          for (int i=0; i<8; i++) tab_bits[i] = malloc (3 * sizeof (int) );
          
          //On test si l'allocation a bien été effectué
          if(tab_bits== NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
           
          tab_bits = init_x0x1x2();
    
    //on récupère le tableau des fonctions de filtrages possibles
           //Déclaration d'un tableau
           int** tab_filtrage = malloc(256 * sizeof (int*) );
           for (int i=0; i<256; i++) tab_filtrage[i] = malloc (8 * sizeof (int) );
          
           //On test si l'allocation a bien été effectuée
           if(tab_filtrage == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
       
           tab_filtrage = init_filtrages_possibles();
    
    //On déclare des compteur à 0 qui vont nous servir à compter le nombre de correlation pour chacun des bits x0, x1, x2
    int cmpt0 = 0;
    int cmpt1 = 0;
    int cmpt2 = 0;
    
    //on boucle sur chaque fonction de filtrage possibles et on compare avec x0, x1, X2
    // S'il y a correlation alors on incremente le compteur
    for (int i=0; i<256; i++) {
        for (int j=0; j<8; j++){
           
            //valeur de x0
            if ( tab_bits[j][0] == tab_filtrage[i][j] ){ cmpt0++; }
            //valeur de x1
            if ( tab_bits[j][1] == tab_filtrage[i][j] ) {cmpt1++; }
            //valeur de x2
            if ( tab_bits[j][2] == tab_filtrage[i][j] ) {cmpt2++; }
            
            tab_correlation[i][0] = ( (cmpt0*100) / (8*256) );
            tab_correlation[i][1] = ( (cmpt1*100) / (8*256) );
            tab_correlation[i][2] = ( (cmpt2*100) / (8*256) );
            
        }

    }

    
    //on desaloue la mémoire pour les tab_bits
     for (int i=0; i<8; i++) free (tab_bits[i]);
     free(tab_bits);
        
    //on desaloue la mémoire poru tab_filtrage
    for (int i=0; i<256; i++) free (tab_filtrage[i]);
    free(tab_filtrage);
       
    return tab_correlation;
}


void affichage(){
  /*  F = 0 0 0 0 0 0 0 0
    cX1 = 50%
    cX2 = 25%
    cX3 = 10%

    F = 0 0 0 0 0 0 0 1
    cX1 = 20%
    cX2 = 30%
    cX3 = 10%
    */
    
    // Déclaration de Tableau F
     int** F = malloc(256 * sizeof (int*) );
     for (int i=0; i<256; i++) F[i] = malloc (8 * sizeof (int) );
    //On test si l'allocation a bien été effectué
    if(F== NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
    
    // Déclaration de Tableau à deux dimensions des correlations
    int** tab = malloc(256 * sizeof (int*) );
    for (int i=0; i<256; i++) tab[i] = malloc (3 * sizeof (int) );
    // test si le tableau à bien été alloué
    if(tab == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
   
    //appel des fonctions
    tab = correlation();
    F = init_filtrages_possibles();
    
    printf("\n Corrélation de x0 x1 x2 : \n");
    
    // parcours du tableau pour afficher les éléments de ce dernier
    for (int j=0; j<256; j++){

            printf("\n\nF = (%d, %d, %d, %d, %d, %d, %d, %d )",F[j][0],F[j][1],F[j][2],F[j][3],F[j][4],F[j][5],F[j][6],F[j][7] );
            for (int i=0; i<3; i++) printf("\nCx%d = %d \n",i, tab[j][i]);
        
    }
    // desaloue la mémoire du tableau
    free(tab);
    free(F);
    
    
}


int main(){

affichage();

return 0;
}

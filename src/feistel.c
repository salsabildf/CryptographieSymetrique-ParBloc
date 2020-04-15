#include "../lib/feistel.h"

int* dec_to_binaire(int nbr){
     int * binaire =  malloc(32*sizeof(int));
     if(binaire == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
    
     for(int i=31; i>=0; i-- ){
         binaire[i] = nbr%2;
         nbr = nbr/2;
     }
    return binaire;
}
int binaire_to_dec(int * dec){
    int T = 0;
   
    for(int i=0; i<32; i++){
       if ( dec[i] )  T += (int) (pow(2.0,(31-i) ) ) ;
      
    }
    return T;
}

int rotation_gauche_7bits( int T ){
    int * tab = malloc(32*sizeof(int));
    if(tab == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
   
    tab = dec_to_binaire(T);
 
    int save[7] = {tab[0], tab[1], tab[2], tab[3], tab[4], tab[5], tab[6]};
 
    for (int i=0 ;i<25; i++) tab[i] = tab[i+7];
    for (int i=25; i<32 ; i++) tab[i] = save[i-25];
  
    return binaire_to_dec(tab);
}

int * chiffrement_par_bloc(int x0L, int x0R, int k0, int k1, int * chiffree){
      
    int T0,T1;
      for(int i=0; i<12; i++){
      printf("-----------------------------\nAffichage %d tour de Feistel \n",i+1);
       
        T0 = rotation_gauche_7bits(x0L ^ x0R) ^k0;
        
        T1 = rotation_gauche_7bits(x0R ^ T0) ^ k1;
        
        //permet d'attribuer les valeurs chiffré intermedaire calculées et
        //les remmettre en tabt que clairLeft et clairRight pour refaire un autre tour de feistel
         x0L = T0;
         x0R = T1;
   
          printf("couple message chiffré : ( 0x%X, 0x%X) \n\n\n", T0,T1);
        
   }
  
    chiffree[0] = T0;
    chiffree[1] = T1;
   
    return chiffree;
}
int * cryptanalyse_1_tour(int clairL, int clairR, int chiffreL, int chiffreR, int * keys){
    
 //  k0 = x1L ⊕ ( ( x0L ⊕  x0R) <<< 7 )
 //  k1 = x1R ⊕ (( x0R ⊕  x1L) <<< 7 )

    keys[0] = chiffreL ^ rotation_gauche_7bits(clairL ^ clairR);
  
    keys[1] = chiffreR ^ rotation_gauche_7bits(clairR ^ chiffreL);
    
    return keys;
}
int * cryptanalyse_12_tours(int clairL, int clairR, int chiffreL, int chiffreR, int * keys){
    
    //déclaration de variable temporaire pour faire les calcul
    int k0, k1;
    
    k0 = rotation_gauche_7bits(clairL ^clairR) ^ chiffreL;
    
    k1 = ( rotation_gauche_7bits(clairL^clairR) ^k0 ) ^ clairR;
    k1 = rotation_gauche_7bits(k1) ^ chiffreR;
    
    //tableau retournant la clef(k0,k1)
    keys[0] = k0;
    keys[1] = k1;
    return keys;
}


void Affichage(int x0L, int x0R, int k0, int k1){
      printf("-----------------------------\nDONNÉES QUE NOUS POSSÉDONS POUR CHIFFRER : \n(clairL,clairR) : (0x%X,0x%X) - (k0,k1) : (0x%X,0x%X)  \n\n\n",x0L,x0R, k0, k1);
      
      //Un tableau pour récupérer le chiffré produit
      int * crypter = malloc(2*sizeof(int));
      if(crypter == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
      
      crypter = chiffrement_par_bloc(x0L,x0R,k0,k1, crypter);
      
      //afficher le couple crypté
      printf("-----------------------------\nCRYPTAGE RÉUSSI.\ncouple message chiffré : ( 0x%X, 0x%X) \n\n", crypter[0], crypter[1]);

      free(crypter);

}
void Affichage_cryptanalyse_1_tour(int clairL, int clairR, int chiffreL, int chiffreR){
    
      printf("\n-----------------------------\nLes données que l'attaquant possède  : \n(clairL,clairR) : (0x%X,0x%X) - (chiffreL,chiffreR) : (0x%X,0x%X)  \n\n",clairL,clairR, chiffreL, chiffreR);
        
       //Un tableau pour récupérer le chiffré produit
       int * keys = malloc(2*sizeof(int));
       if(keys == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
       
       keys = cryptanalyse_1_tour(clairL,clairR,chiffreL,chiffreR, keys);
       
       //afficher la clef k0 et k1
     printf("-----------------------------\nATTAQUE RÉUSSIE.\nLa clé trouvé est (k0,k1) = (0x%X,0x%X) \n\n", keys[0], keys[1]);

       free(keys);
}

void Affichage_cryptanalyse_12_tours(int clairL, int clairR, int chiffreL, int chiffreR){
      
      printf("-----------------------------\nLes données que l'attaquant possède  : \n(clairL,clairR) : (0x%X,0x%X) - (chiffreL,chiffreR) : (0x%X,0x%X)  \n\n",clairL,clairR, chiffreL, chiffreR);

        printf("-----------------------------\nCryptanalyse 12 tour de Feistel \n");
       //Un tableau pour récupérer le chiffré produit
       int * keys = malloc(2*sizeof(int));
       if(keys == NULL) { printf("Reservation mémoire impossible !\n"); exit(-1); }
       
       keys = cryptanalyse_12_tours(clairL,clairR,chiffreL,chiffreR, keys);
       
       //afficher la clef k0 et k1
     printf("-----------------------------\nATTAQUE RÉUSSIE.\nLa clé trouvé est (k0,k1) = (0x%d,0x%d) \n\n", keys[0], keys[1]);

       free(keys);
    
}
int main(){
    
    /* donnée en entré que nous possédons */
    int x0L =  0x45019824, x0R = 0x51023321, k0 = 0x01020304, k1 = 0x98765432;
    printf(" CHIFFREMENT   \n\n ");
    /* affiche les messages cryptés */
    Affichage(x0L, x0R, k0, k1);
    
    
   /*---------------------------------- */
    
    printf("\n\n\n\n----------------------------------\n----------------------------------\n");
      printf(" CRYPTANALYSE 1 TOUR \n \n");
   /* donnée en entrée que l'attaquant possède
    On cherche cette clef(k0,k1) = (0x01020304,0x98765432) */
    int clairL = 0x45019824, clairR = 0x51023321, chiffreL = 0xD7818E, chiffreR = 0x72AF039A;
    
    //affiche la clef (k0,k1)
    Affichage_cryptanalyse_1_tour(clairL, clairR, chiffreL, chiffreR );
    
    
      printf("\n\n\n\n ----------------------------------\n----------------------------------\n");
       printf(" CRYPTANALYSE 12 TOURS \n\n ");
       /*  On cherche cette clef(k0,k1) = (0x01020304,0x98765432)
       donnée en entrée que l'attaquant possède sur 12 TOURS */
    clairL = 0x45019824; clairR = 0x51023321; chiffreL = 0x1F8861AE; chiffreR = 0x3C94126;
       
       Affichage_cryptanalyse_12_tours(clairL, clairR, chiffreL, chiffreR );
    
    return 0;
}

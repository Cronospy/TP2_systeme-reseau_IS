/**
 * TP pour évelopper un allocateur de mémoire
 *  - L'objectif est de réecrire malloc et free
 */

#include <stdlib.h>
#include <stdio.h>

#define MAGIC_NUMBER 0xBADA110CBADA110CL;

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next;   /* pointe sur le prochain bloc libre */
    size_t bloc_size;               /* taille du memory bloc en octets*/
    long magic_number;              /* 0xBADA110CBADA110CL */
} HEADER;


HEADER* malloc_3is(){
    /**
     * - Si aucun bloc disponible :
     *      -> On agrandit le tas avec la fonction sbrk()
     * - Recherche d'un bloc de taille suffisante et disponible dans la liste chaînée
     *      -> retourne le bloc dispo ou une partie du bloc (division du bloc) 
     */
    return NULL;
}

void fusion(HEADER* blocA, HEADER* blocB){
    /**
     * Fusionne deux bloc mémoire dans la liste des blocs libres 
     *  - ptr_next : pointeur du deuxième pointeur 
     *  - bloc_size : sommme des bloc_size 
     *  - magic_number : MAGIC_NUMBER
     */
    return;
}

void insert(HEADER* bloc){
    /**
     * Insert le bloc dans la liste des blocs libres en gardant la propriété :
     *   -> la liste est classée par ordre croissant des adresses
     */
    return;
}

void free_3is(HEADER* bloc){
    /**
     * - Contrôle des débordements de mémoire avec le MAGIC_NUMBER 
     *      -> retour d'une erreur de débordement 
     * - Le bloc est réinsérer dans le liste des blocs libres (classée par ordre croissant des adresses des blocs de mémoire disponibles) 
     *      -> Si le bloc mémoire libéré est contigu avec un bloc mémoire déjà libre
     *              -> Fusion des Blocs
     */
    return;
}



int main(){
    printf("TP n°2 : Allocateur de Mémoire \n");

    return 0;
}
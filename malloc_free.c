/**
 * TP pour évelopper un allocateur de mémoire
 *  - L'objectif est de réecrire malloc et free
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAGIC_NUMBER 0xBADA110CBADA110CL

typedef struct HEADER_TAG {
    struct HEADER_TAG * ptr_next;   /* pointe sur le prochain bloc libre */
    size_t bloc_size;               /* taille du memory bloc en octets*/
    long magic_number;              /* 0xBADA110CBADA110CL */
} HEADER;

HEADER* free_list = NULL;

void* user_ptr(HEADER* bloc){
    return (char*)bloc + sizeof(HEADER);
}

HEADER* header_from_user_ptr(void* ptr){
    return (HEADER*)((char*)ptr - sizeof(HEADER));
}

HEADER* request_memory(size_t size){
    HEADER* block = sbrk(sizeof(HEADER) + size + sizeof(long));

    if (block == (void*)-1){
        return NULL;
    }

    block->ptr_next = NULL;
    block->bloc_size = size;
    block->magic_number = MAGIC_NUMBER;

    //footer 
    long* footer = (long*)(char*)block + sizeof(HEADER) + size ;
    *footer = MAGIC_NUMBER;

    return block;
}

void* malloc_3is(size_t size){
    /**
     * - Si aucun bloc disponible :
     *      -> On agrandit le tas avec la fonction sbrk()
     * - Recherche d'un bloc de taille suffisante et disponible dans la liste chaînée
     *      -> retourne le bloc dispo ou une partie du bloc (division du bloc) 
     */

    if(size <= 0) return NULL;

    HEADER* previous_block = NULL;
    HEADER* current_block = free_list;

    while(current_block!=NULL){
        if(current_block->bloc_size >= size){
            break;
        }
        previous_block = current_block;
        current_block = current_block->ptr_next;
    }
    
    // Un bloc est disponible  (division de bloc si taille plus grande à faire ;-;)
    if(current_block != NULL){
        if (previous_block == NULL){
            free_list = current_block->ptr_next;
        }
        else{
            previous_block->ptr_next = current_block->ptr_next;
        }

        current_block->ptr_next = NULL;

        return user_ptr(current_block);
    }

    // Aucun bloc disponible 
    HEADER* new_block = request_memory(size);
    return user_ptr(new_block);
}

void fusion(HEADER* blocA, HEADER* blocB){
    /**
     * Fusionne deux bloc mémoire dans la liste des blocs libres 
     *  - ptr_next : pointeur du deuxième pointeur 
     *  - bloc_size : sommme des bloc_size 
     *  - magic_number : MAGIC_NUMBER
     */

    HEADER* new_ptr_next = blocA->ptr_next>blocB->ptr_next ? blocA->ptr_next : blocB->ptr_next;
    size_t new_bloc_size = blocA->bloc_size + blocB->bloc_size;
    return;
}

void insert(HEADER* block){
    /**
     * Insert le bloc dans la liste des blocs libres en gardant la propriété :
     *   -> la liste est classée par ordre croissant des adresses
     */
    HEADER* previous_block = NULL;
    HEADER* current_block = free_list;

    while (current_block != NULL) {
        if (current_block >= block) {
            if (previous_block == NULL) {
                free_list = block;
                block->ptr_next = current_block;

            }
            else{
                previous_block->ptr_next = block;
                block->ptr_next = current_block;

            }
            break;
        }
        previous_block=current_block;
        current_block = current_block->ptr_next;
    }
    free_list = block;
    block->ptr_next = NULL;
}

void free_3is(void* ptr){
    /**
     * - Contrôle des débordements de mémoire avec le MAGIC_NUMBER 
     *      -> retour d'une erreur de débordement 
     * - Le bloc est réinsérer dans le liste des blocs libres (classée par ordre croissant des adresses des blocs de mémoire disponibles) 
     *      -> Si le bloc mémoire libéré est contigu avec un bloc mémoire déjà libre
     *              -> Fusion des Blocs
     */
    HEADER* header_ptr = header_from_user_ptr(ptr);
    // insertion du bloc dans la liste

    // Check débordement

    // check bloc de gauche et de droite

    return;
}



int main(){
    printf("TP n°2 : Allocateur de Mémoire \n");

    return 0;
}
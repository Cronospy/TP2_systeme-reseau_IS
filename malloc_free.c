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

void print_free_list(){
    printf("free_list :");
    HEADER* current_block = free_list;
    while(current_block != NULL){
        printf("\n-> %p | %ld ",current_block,current_block->bloc_size);
        current_block = current_block->ptr_next;
    }
    printf("\n-> NULL\n");
}

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
    long* footer = (long*) ((char*)block + sizeof(HEADER) + size);
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

    if (new_block == NULL){
        return NULL;
    }

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
                //insertion en début de liste
                block->ptr_next = free_list;
                free_list = block;
            }
            else{
                // insertion en milieu de liste
                block->ptr_next = current_block;
                previous_block->ptr_next = block;
            }
            return;
        }
        previous_block = current_block;
        current_block = current_block->ptr_next;
    }

    block->ptr_next = NULL;
    if (previous_block == NULL){
        free_list=block; // liste totalement vide
    }
    else {
        previous_block->ptr_next = block;
    }
    
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

    long* footer = (long*) ((char*)header_ptr + sizeof(HEADER) + header_ptr->bloc_size) ;
    // Check débordement
    if (header_ptr->magic_number != MAGIC_NUMBER || *footer != MAGIC_NUMBER){
        printf("Erreur de dépassement !\n");
        return;
    }

    // Insertion du bloc dans la liste
    insert(header_ptr);

    // check bloc de gauche et de droite

    return;
}

void check_displaying(void* ptr){
    HEADER* header_ptr = header_from_user_ptr(ptr);

    long* footer = (long*) ((char*)header_ptr + sizeof(HEADER) + header_ptr->bloc_size) ;
    if (header_ptr->magic_number != MAGIC_NUMBER || *footer != MAGIC_NUMBER){
        printf("Erreur de dépassement !\n");
        return;
    }
}


int main(){
    printf("TP n°2 : Allocateur de Mémoire \n");

    print_free_list();

    // Allocation multiple 
    printf("\n*--------------- Allocation multiple ---------------*\n");
    printf("sbrk(0):%p\n",sbrk(0));
    void* ptr1 = malloc_3is(100);
    printf("sbrk(0):%p\n",sbrk(0));
    void* ptr2 = malloc_3is(100);
    printf("sbrk(0):%p\n",sbrk(0));
    void* ptr3 = malloc_3is(100);
    printf("sbrk(0):%p\n",sbrk(0));
    void* ptr4 = malloc_3is(100);
    printf("sbrk(0):%p\n",sbrk(0));

    printf("\n*--------------- Libération d'un bloc ---------------*\n");
    free_3is(ptr2);
    print_free_list();
    free_3is(ptr1);
    print_free_list();

    printf("\n*--------------- Débordement de mémoire ---------------*\n");
    //écriture dans le footer
    long* footer = (long*)((char*)ptr3 + 100);
    *footer = 42;
    //écriture dans le header
    HEADER* header = header_from_user_ptr(ptr4);
    header->magic_number = 42;

    check_displaying(ptr3);
    check_displaying(ptr4);

    printf("Fin du TP\n");
    return 0;
}
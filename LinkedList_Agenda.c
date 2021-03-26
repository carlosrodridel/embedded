#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data
{
    char name[30];
    char phone[11];
}data;


typedef struct node{
    data credentials;
    struct node *ptrNext;  
}node;

node *head=NULL; 
node *footer=NULL;  

node* createNode(){
    node *newNode =(node*)malloc(sizeof(node));
    if(newNode==NULL) printf("Espacio insuficiente\r\n");

    return(newNode); 
} 

void addNode(){
    node *newnode=createNode();

    if (newnode!=NULL){
        fflush(stdin);
        printf("Ingrese nombre: ");
        gets(newnode->credentials.name);
        fflush(stdin);
        printf("Ingrese numero: ");
        gets(newnode->credentials.phone);
        
        
        if(head==NULL){ 
            newnode->ptrNext=NULL; 
            head=newnode; 
            footer=newnode;
            return; 
        }
        newnode->ptrNext=NULL;
        footer->ptrNext=newnode;     
        footer=newnode;
    }
}

void walkList(){
     node *NODO=head; 
  while(NODO!=NULL){
       printf("persona: %s\r\n",NODO->credentials.name);
       printf("telefono: %s\r\n",NODO->credentials.phone);
       NODO=NODO->ptrNext; 
       return;
       }
   printf("Lista vacía\r\n");
}


void freeList(){
     node* aux = head;
     if(aux == NULL){
        printf("Lista Vacía\r\n");
        return;
    }

  while(aux!=NULL){
       head=aux->ptrNext;
       free(aux); 
       aux=head; 
    }
    head = NULL;
    footer = NULL;
    printf("Memoria Liberada\r\n");
}
    
int main(){
    char sel;
    printf("/////////bienvenido a su agenda telefónica/////////\r\n");
    


    while (1)
    {
        printf("\r\n\tseleccione una opción:\r\n");
        printf("1. añadir un contacto \r\n2. ver todos los contactos\r\n3. eliminar lista de contactos\r\n4. salir\r\n");
        scanf("%d", &sel);
        getchar();
        switch (sel)
        {
        case 1:
            addNode();
            break;
        
        case 2:
            walkList();
            break;
        case 3:
            freeList();
            break;
        case 4:
            freeList();
            return -1;
        default:
            printf("opción invalida, intente nuevamente");
            break;
        }
    }
    return 0;
}
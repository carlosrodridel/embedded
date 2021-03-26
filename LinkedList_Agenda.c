#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct data
{
    char name[30];
    unsigned long phone;
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
     unsigned long num;
    if (newnode!=NULL){
        fflush(stdin);
        printf("Ingrese nombre: ");
        gets(newnode->credentials.name);
        fflush(stdin);
        printf("Ingrese numero: ");
        scanf("%ld", &num);
        newnode->credentials.phone=num;

        
        
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
       printf("telefono: %ld\r\n\r\n",NODO->credentials.phone);
       NODO=NODO->ptrNext; 
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

void deleteNumber(){
    char flag=1;
    unsigned long x;
    node *q= head;
    node *t;
    printf("ingrese el numero a eliminar: ");
    scanf("%ld",&x);
    while ((q->credentials.phone!=x) && (flag==1))
    {
        if (q->ptrNext!=NULL)
        {
            t=q;
            q=q->ptrNext;
        }else{
         flag=0;
        }
    }
    if  (flag==0){
        printf("\r\nno se encontró el numero en la agenda");

    }else{ 
        
        if(head==q)
            head=q->ptrNext;
        else
            t->ptrNext = q->ptrNext;
        free(q);
    }
    
}

int main(){
    char sel;
    printf("/////////bienvenido a su agenda telefónica/////////\r\n");
    


    while (1)
    {
        printf("\r\n\tseleccione una opción:\r\n");
        printf("1. añadir un contacto \r\n2. ver todos los contactos\r\n3. eliminar lista de contactos\r\n4. eliminar un numero\r\n5. salir\r\n");
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
            deleteNumber();
            break;
            
        case 5:
            freeList();
            return -1;
            break;
        default:
            printf("opción invalida, intente nuevamente");
            break;
        }
    }
    return 0;
}

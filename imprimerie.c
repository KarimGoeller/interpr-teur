#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include "imprimerie.h"




/*
requires: nothing
assigns: nothing
ensures: return an empty stack
*/
pill initSTACK() {
    pill t;
    t.top = -1;
    return t;
}
/*
requires: nothing
assigns: t.content, t.top
ensures: put a on the top of t
*/
void push(pill *t, pixel **a) {
    if(t->top<STACK_SIZE){
        t->top++;
        t->content[t->top] = a;
    }
}
/*
requires: nothing
assigns: t.top
ensures: return and remove the last element of t
*/
pixel **pop(pill *t) {
    if (t->top==-1) {
        fprintf(stderr,"Stack is empty, cannot pop last element");
        exit(0);
    }
    pixel **a = t->content[t->top];
    t->top--;
    return a;
}

/*Les fonctions de base pour coder les liste*/
/*
  requires:a valide liste and a position
  assigns:list
  ensures:put a new element at the begining of the list
  */
void add(list *l,pos e) {
    list new = malloc(sizeof(struct cell));
    new->position[0] = e[0];
    new->position[1] = e[1];
    new->next = *l;
    *l = new;
}

/*
  requires:a volid list
  assigns:list
  ensures:free all the cells of the list to delet it
  */
void delet_list(list l){
    list tmp;
    while (l != NULL) //la list n'est pas infinie donc le dernier element à forcément NULL comme prochain maillon
    {
       tmp=l;
       l=l->next;
       free(tmp);
    }
}

void print_list(list l) {
    while (NULL != l)//la list n'est pas infinie donc le dernier element à forcément NULL comme prochain maillon
    {
        printf("[%d,%d] -> ", l->position[0],l->position[1]);
        l=l->next;
    }
    printf("[]\n");
}


/*fonction pour l'état de la machine*/
/*
 requires:A positiv size, a not-NULL layer
 assigns:layer
 ensures:free the 2D layer
 */
void delet_layer(int size, pixel **layer){
    for(int i=0;i<size;i++){
        free(layer[i]);
    }
    free(layer);
}

/*requires:an initialise machine state
 * assigns:the color bucket of the machine state
 *ensures:a new bucket, with color c in it
 * */

void add_color(machine_state *A,rgb c){
    if(A->color_b.len==-1){//on est obliger de faire un cas à part sinon on va free un pointeur NULL ce qui cause un segmentation fault
        rgb *new_bucket=malloc(sizeof(rgb));
        memset(new_bucket,0,sizeof(rgb));
        A->color_b.bucket=new_bucket;
    }
    else{
        rgb *new_bucket=malloc((A->color_b.len+2) * sizeof(rgb));
        memset(new_bucket,0,(A->color_b.len+2)*sizeof(rgb));
        for(int i=0;i<A->color_b.len+1;i++){
            new_bucket[i][0]=A->color_b.bucket[i][0];
            new_bucket[i][1]=A->color_b.bucket[i][1];
            new_bucket[i][2]=A->color_b.bucket[i][2];
        }
        free(A->color_b.bucket);
        A->color_b.bucket=new_bucket;
    }
    A->color_b.len++;
    A->color_b.bucket[A->color_b.len][0]=c[0];
    A->color_b.bucket[A->color_b.len][1]=c[1];
    A->color_b.bucket[A->color_b.len][2]=c[2];
}
/*requires:an initialise machine state
 * assigns:the opacity bucket of the machine state
 *ensures:a new bucket, with opacity opac =at the end
 * */
void add_opac(machine_state *A,int opac){
    if(A->opac_b.len==-1){//on est obliger de faire un cas à part sinon on va free un pointeur NULL ce qui cause un segmentation fault
        int *new_bucket=malloc(sizeof(int));
        memset(new_bucket,0,sizeof(int));
        A->opac_b.bucket=new_bucket;
    }
    else{
        int *new_bucket=malloc((A->opac_b.len+2) * sizeof(int));
        memset(new_bucket,0,(A->opac_b.len+2)*sizeof(int));
        for(int i=0;i<A->opac_b.len+1;i++){
            new_bucket[i]=A->opac_b.bucket[i];
        }
        free(A->opac_b.bucket);
        A->opac_b.bucket=new_bucket;
    }
    A->opac_b.len++;
    A->opac_b.bucket[A->opac_b.len]=opac;
}
/*@requires:an initialise machine state
 *@assigns:the opaciry and color bucket  of the machine state
 *@ensures:to free both bucket and put the lenght of the bucket at -1
 */
void empty_bucket(machine_state *A){
    if(A->color_b.len > -1){  //si les sceaus sont vide inutile de les free
        free(A->color_b.bucket);
        A->color_b.len=-1;
    }
    if(A->opac_b.len > -1){
        free(A->opac_b.bucket);
        A->opac_b.len=-1;
    }
}

/*requires:an initialise machine state
 * assigns:A->stack_layer
 *ensures:put a layer with 0 everywhere on the top of A->stack_layer
 * */
void add_layer(machine_state *A,int size){
    if(A->stack_layer.top<STACK_SIZE){
        pixel **res=malloc(size* sizeof(pixel*));
        for(int i=0;i<size;i++){
            res[i]=malloc(size * sizeof(pixel));
            memset(res[i],0,size*sizeof(pixel));
        }
        push(&A->stack_layer,res);
    }
}
/* requires:A positive size
 * assigns: nothing
 * ensures:an initialise machine state
 */
machine_state init_machine_state(int s){
    machine_state res;
    res.position[0]=0;
    res.position[1]=0;
    res.mark_position[0]=0;
    res.mark_position[1]=0;
    res.direction='e';
    res.color_b.len = -1;
    res.opac_b.len = -1;
    res.stack_layer=initSTACK();
    add_layer(&res,s);
    return res;
}

/*
  requires:an initialise machine state
  assigns:assigns:the machine state
  ensures:free all the memory alocated for the machine state
  */

void delet_machine_state(machine_state *A,int size){
    while(A->stack_layer.top!=-1){
        pixel **d=pop(&A->stack_layer);
        delet_layer(size,d);
    }
    empty_bucket(A);
}




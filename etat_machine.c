#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include "imprimerie.h"
#include "etat_machine.h"

/* requires:an initalise machine state
 * assigns:return a pixel
 * ensures:a current pixel make by a mix of opacity and color bucket
 */
pixel current_pixel(machine_state A){
    pixel res;
    int tmp;
    unsigned int tmp2[3]; //on fait les calcules sur des int car quand on fait des addition sur des unsigned chars leur valeur maximal est 255
    if(A.opac_b.len==-1){
        res.opac=255;
    }
    else{
        tmp=0;
        for(int i=0;i<=A.   opac_b.len;i++) {
            tmp += A.opac_b.bucket[i];
        }
        tmp=tmp/(A.opac_b.len+1);
        res.opac=tmp;
    }

    if(A.color_b.len==-1){
        res.color[0]=0;
        res.color[1]=0;
        res.color[2]=0;
    }
    else{
        tmp2[0]=0;
        tmp2[1]=0;
        tmp2[2]=0;
        for(int i=0;i<=A.color_b.len;i++){
            tmp2[0]+=A.color_b.bucket[i][0];
            tmp2[1]+=A.color_b.bucket[i][1];
            tmp2[2]+=A.color_b.bucket[i][2];
        }
        tmp2[0]=tmp2[0]/(A.color_b.len+1);
        tmp2[1]=tmp2[1]/(A.color_b.len+1);
        tmp2[2]=tmp2[2]/(A.color_b.len+1);
        tmp2[0]=tmp2[0]*res.opac/255;
        tmp2[1]=tmp2[1]*res.opac/255;
        tmp2[2]=tmp2[2]*res.opac/255;
        res.color[0]=tmp2[0];
        res.color[1]=tmp2[1];
        res.color[2]=tmp2[2];
    }
    return res;
}

/* requires:an initialise machine state
 * assigns:A->mark_position
 * ensures:change the mark position and the actual position
 */
void change_mark_position(machine_state *A){
    A->mark_position[0]=A->position[0];
    A->mark_position[1]=A->position[1];
}

/* requires:an initialise machine state
 * assigns: A->position
 * ensures:move the cursor to A->direction
 */
void move(machine_state *A,int size){
    if(A->direction=='e'){
        if(A->position[1]==size-1){
            A->position[1]=0;
        }
        else{
            A->position[1]++;
        }
    }
    else if(A->direction=='n'){
        if(A->position[0]==0){
            A->position[0]=size-1;
        }
        else{
            A->position[0]--;
        }
    }
    else if(A->direction=='s'){
        if(A->position[0]==size-1){
            A->position[0]=0;
        }
        else{
            A->position[0]++;
        }
    }
    else if(A->direction=='w'){
        if(A->position[1]==0){
            A->position[1]=size-1;
        }
        else{
            A->position[1]--;
        }
    }
}
/* requires:an initialise machine state
 * assigns:A->direction
 * ensures:change the direction by the "sense horraire"
 */
void change_direction_horraire(machine_state *A){
    if(A->direction=='e'){
        A->direction='s';
    }
    else if(A->direction=='s'){
        A->direction='w';
    }
    else if(A->direction=='w'){
        A->direction='n';
    }
    else if(A->direction=='n'){
        A->direction='e';
    }
}
/* requires:an initialise machine state
 * assigns:A->direction
 * ensures:change the direction by the "sens anti-horraire"
 */
void change_direction_antihorraire(machine_state *A){
    if(A->direction=='e'){
        A->direction='n';
    }
    else if(A->direction=='n'){
        A->direction='w';
    }
    else if(A->direction=='w'){
        A->direction='s';
    }
    else if(A->direction=='s'){
        A->direction='e';
    }
}
/* requires:nothing
 * assigns:nothing
 * ensures:return the biggest absolute value value between a and b
 */
int max(int a, int b){
    int res;
    if(a<0) a=-a;
    if(b<0) b=-b;
    if (a>b){
        res=a;
    }
    else{
        res=b;
    }
    return res;
}
/* requires:an initialise machine state
 * assigns:A->stack_layer.content[A->stack_layer.top]
 * ensures:print a line with the current pixel form the mark_position to the position on the top layer
 */
void print_line(machine_state *A){
    int delta_i=A->position[0]-A->mark_position[0];
    int delta_j=A->position[1]-A->mark_position[1];
    int d=max(delta_i,delta_j);
    int s;
    if((0<delta_i && 0<delta_j) || (0>delta_i && 0>delta_j)){
        s=0;
    }
    else{
        s=1;
    }
    int i=A->mark_position[0]*d + (d-s)/2;
    int j=A->mark_position[1]*d + (d-s)/2;
    pixel current_pix= current_pixel(*A);
    pixel **layer=pop(&A->stack_layer);
    for(int k=0;k<d;k++){
        layer[i/d][j/d].color[0]=current_pix.color[0];
        layer[i/d][j/d].color[1]=current_pix.color[1];
        layer[i/d][j/d].color[2]=current_pix.color[2];
        layer[i/d][j/d].opac=current_pix.opac;
        i+=delta_i;
        j+=delta_j;
    }
    layer[A->position[0]][A->position[1]].color[0]=current_pix.color[0];
    layer[A->position[0]][A->position[1]].color[1]=current_pix.color[1];
    layer[A->position[0]][A->position[1]].color[2]=current_pix.color[2];
    layer[A->position[0]][A->position[1]].opac=current_pix.opac;
    push(&A->stack_layer,layer);

}



/*
  requires:an initialise machine state
  assigns:assigns:A->stack_layer.content[A->stack_layer.top]
  ensures:fill the zone of the same color where the position is with the new color and opacity current pixel
  */

void fill_machine_state(machine_state *A,int size){
    list passage;
    int *test;//tableau avec des 0 pour savoir par quel endroit on est déjà aller
    test=malloc(size*size*sizeof(int));
    memset(test,0,size*size*sizeof(int));
    test[size*A->position[0]+A->position[1]]=1;
    passage=malloc(sizeof(struct cell));
    pixel **layer=pop(&A->stack_layer);
    passage->next=NULL;
    passage->position[0]=A->position[0];
    passage->position[1]=A->position[1];
    pixel old;
    old.color[0]=layer[A->position[0]][A->position[1]].color[0];
    old.color[1]=layer[A->position[0]][A->position[1]].color[1];
    old.color[2]=layer[A->position[0]][A->position[1]].color[2];
    old.opac=layer[A->position[0]][A->position[1]].opac;
    pixel new= current_pixel(*A);
    while(passage!=NULL){ //il y a un nombre finis de case dans le calque et on teste si chaque position à déjà été dans la pill donc impossible d'avoir une boucle infinie
        int x=passage->position[0];
        int y=passage->position[1];
        list tmp;
        tmp=passage;
        passage=passage->next;
        free(tmp);
        if(layer[x][y].color[0]==old.color[0] && layer[x][y].color[1]==old.color[1] && layer[x][y].color[2]==old.color[2] && layer[x][y].opac==old.opac ) {
            layer[x][y].color[0] = new.color[0];
            layer[x][y].color[1] = new.color[1];
            layer[x][y].color[2] = new.color[2];
            layer[x][y].opac = new.opac;
            if(x>0){
                pos tmp;
                tmp[0]=x-1;
                tmp[1]=y;
                if(layer[tmp[0]][tmp[1]].color[0]==old.color[0] && layer[tmp[0]][tmp[1]].color[1]==old.color[1] && layer[tmp[0]][tmp[1]].color[2]==old.color[2] && layer[tmp[0]][tmp[1]].opac==old.opac){
                    if(test[size*tmp[0]+tmp[1]]!=1){
                        test[size*tmp[0]+tmp[1]]=1;
                        add(&passage,tmp);
                    }
                }

            }
            if(y>0){
                pos tmp;
                tmp[0]=x;
                tmp[1]=y-1;
                if(layer[tmp[0]][tmp[1]].color[0]==old.color[0] && layer[tmp[0]][tmp[1]].color[1]==old.color[1] && layer[tmp[0]][tmp[1]].color[2]==old.color[2] && layer[tmp[0]][tmp[1]].opac==old.opac){
                    if(test[size*tmp[0]+tmp[1]]!=1){
                        test[size*tmp[0]+tmp[1]]=1;
                        add(&passage,tmp);
                    }
                }
            }
            if(x<size-1){
                pos tmp;
                tmp[0]=x+1;
                tmp[1]=y;
                if(layer[tmp[0]][tmp[1]].color[0]==old.color[0] && layer[tmp[0]][tmp[1]].color[1]==old.color[1] && layer[tmp[0]][tmp[1]].color[2]==old.color[2] && layer[tmp[0]][tmp[1]].opac==old.opac){
                    if(test[size*tmp[0]+tmp[1]]!=1){
                        test[size*tmp[0]+tmp[1]]=1;
                        add(&passage,tmp);
                    }
                }
            }
            if(y<size-1){
                pos tmp;
                tmp[0]=x;
                tmp[1]=y+1;
                if(layer[tmp[0]][tmp[1]].color[0]==old.color[0] && layer[tmp[0]][tmp[1]].color[1]==old.color[1] && layer[tmp[0]][tmp[1]].color[2]==old.color[2] && layer[tmp[0]][tmp[1]].opac==old.opac){
                    if(test[size*tmp[0]+tmp[1]]!=1){
                        test[size*tmp[0]+tmp[1]]=1;
                        add(&passage,tmp);
                    }
                }
            }
        }
    }
    delet_list(passage);
    push(&A->stack_layer,layer);
    free(test);
}

/*
  requires:an initialise machine state
  assigns:assigns:A->stack_layer.content[A->stack_layer.top],A->stack_layer.content[A->stack_layer.top-1]
  ensures:mix the two highest layer ont the stack
  */

void mix_layer(machine_state *A,int size){
    if(A->stack_layer.top>0){
        pixel **c0=pop(&A->stack_layer);
        pixel **c1=pop(&A->stack_layer);
        int alpha0;
        for(int i=0;i<size;i++){
            for(int j=0;j<size;j++){
                alpha0=c0[i][j].opac;
                c1[i][j].opac=alpha0+c1[i][j].opac*(255-alpha0)/255;
                c1[i][j].color[0]=c0[i][j].color[0]+c1[i][j].color[0]*(255-alpha0)/255;
                c1[i][j].color[1]=c0[i][j].color[1]+c1[i][j].color[1]*(255-alpha0)/255;
                c1[i][j].color[2]=c0[i][j].color[2]+c1[i][j].color[2]*(255-alpha0)/255;

            }
        }
        push(&A->stack_layer,c1);
        delet_layer(size,c0);
    }
}

/*
  requires:an initialise machine state
  assigns:assigns:A->stack_layer.content[A->stack_layer.top],A->stack_layer.content[A->stack_layer.top-1]
  ensures:delet the layer at the top of the stack and use opacity of each pixel on top to modify the layer bellow
  */


void cut_layer(machine_state *A,int size){
    if(A->stack_layer.top>0){
        pixel **layer0=pop(&A->stack_layer);
        pixel **layer1=pop(&A->stack_layer);
        for(int i=0;i<size;i++){
            for (int j=0;j<size;j++){
                layer1[i][j].color[0]=layer1[i][j].color[0]*layer0[i][j].opac/255;
                layer1[i][j].color[1]=layer1[i][j].color[1]*layer0[i][j].opac/255;
                layer1[i][j].color[2]=layer1[i][j].color[2]*layer0[i][j].opac/255;
                layer1[i][j].opac=layer1[i][j].opac*layer0[i][j].opac/255;
            }
        }
        delet_layer(size,layer0);
        push(&A->stack_layer,layer1);
    }
}

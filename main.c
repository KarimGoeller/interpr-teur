    #include <stdio.h>
#include <stdlib.h>
#include<string.h>

#include "etat_machine.h"
#include "imprimerie.h"
/*
  requires:the first caracteres you enter must be a positive int
  assigns:nothing
  ensures:creat a bitmap tha represent the matrix of the highest layer
  */
rgb *construction(FILE *f,machine_state A,int size){
    char l;
    rgb black={0,0,0};
    rgb red={255,0,0};
    rgb green={0,255,0};
    rgb blue={0,0,255};
    rgb yellow={255,255,0};
    rgb magenta={255,0,255};
    rgb cyan={0,255,255};
    rgb white={255,255,255};
    while((l = getc(f))!=EOF){ //il y a forcément un nombre finis d'instruction donc il finis toujour de lire
        switch(l){
            case 'n':
                add_color(&A,black);
                break;
            case 'r':
                add_color(&A,red);
                break;
            case 'g':
                add_color(&A,green);
                break;
            case 'b':
                add_color(&A,blue);
                break;
            case 'y':
                add_color(&A,yellow);
                break;
            case 'm':
                add_color(&A,magenta);
                break;
            case 'c':
                add_color(&A,cyan);
                break;
            case 'w':
                add_color(&A,white);
                break;
            case 't':
                add_opac(&A,0);
                break;
            case 'o':
                add_opac(&A,255);
                break;
            case 'i':
                empty_bucket(&A);
                break;
            case 'v':
                move(&A,size);
                break;
            case 'h':
                change_direction_horraire(&A);
                break;
            case 'a':
                change_direction_antihorraire(&A);

                break;
            case 'p':
                change_mark_position(&A);
                break;
            case 'l':
                print_line(&A);
                break;
            case 'f':
                fill_machine_state(&A,size);
                break;
            case 's':
                add_layer(&A,size);
                break;
            case 'e':
                mix_layer(&A,size);
                break;
            case 'j':
                cut_layer(&A,size);
                break;
        }

    }
    rgb *btm = malloc(size*size*sizeof(rgb));
    memset(btm,0,size*size*sizeof(rgb));
    pixel **d=pop(&A.stack_layer);
    for(int i=0;i<size-1;i++){
        for(int j=0;j<size;j++){
            btm[size*i+j][0]=d[i][j].color[0];
            btm[size*i+j][1]=d[i][j].color[1];
            btm[size*i+j][2]=d[i][j].color[2];
        }
    }
    push(&A.stack_layer,d);
    delet_machine_state(&A,size);
    return btm;
}


/*
  requires:the first caracteres you enter must be a positive int
  assigns:nothing
  ensures:creat an image with the instruction you give
  */

int main(int  argc,char **argv){
    int size;
    char buf[256];
    if(argc==1){
        fgets(buf,256,stdin);
        sscanf(buf,"%d",&size);
        machine_state A= init_machine_state(size);
        rgb *btm=construction(stdin,A,size);
        fprintf(stdout,"P6\n%d %d\n255\n", size, size);
        fwrite(btm,sizeof(rgb),size*size,stdout);
        free(btm);
    }
    if(argc==2){
        FILE *source=fopen(argv[1],"r");
        if(source==NULL){
            fprintf(stderr,"can't open the source file\n");
            exit(1);
        }
        fgets(buf,256,source);
        sscanf(buf,"%d",&size);
        machine_state A= init_machine_state(size);
        rgb *btm=construction(source,A,size);
        fclose(source);
        fprintf(stdout,"P6\n%d %d\n255\n", size, size);
        fwrite(btm,sizeof(rgb),size*size,stdout);
        free(btm);
    }
    if(argc==3){
        FILE *source=fopen(argv[1],"r");
        if(source==NULL){
            fprintf(stderr,"can't open the source file\n");
            exit(2);
        }
        fgets(buf,256,source);
        sscanf(buf,"%d",&size);
        machine_state A= init_machine_state(size);
        rgb *btm=construction(source,A,size);
        fclose(source);
        FILE *destination=fopen(argv[2],"wb");
        if(destination==NULL){
            fprintf(stderr,"can't open the destination file\n");
            exit(3);
        }
        fprintf(destination,"P6\n%d %d\n255\n", size, size);
        fwrite(btm,sizeof(rgb),size*size,destination);
        fclose(destination);
        free(btm);
    }
    if(argc>3){
        fprintf(stderr,"too much argument\n");
        FILE *source=fopen(argv[1],"r");
        if(source==NULL){
            fprintf(stderr,"can't open the destination file\n");
            exit(4);
        }
        fgets(buf,256,source);
        sscanf(buf,"%d",&size);
        machine_state A= init_machine_state(size);
        rgb *btm=construction(source,A,size);
        fclose(source);
        FILE *destination=fopen(argv[2],"wb");
        if(destination==NULL){
            fprintf(stderr,"can't open the source file\n");
            exit(5);
        }
        fprintf(destination,"P6\n%d %d\n255\n", size, size);
        fwrite(btm,sizeof(rgb),size*size,destination);
        fclose(destination);
        free(btm);
    }
    return 0;
}
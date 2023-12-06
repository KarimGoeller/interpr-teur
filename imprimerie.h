#ifndef IMPRIMERIE_H
#define IMPRIMERIE_H

#define STACK_SIZE 10

/*les pixels*/
typedef unsigned char rgb[3];
struct pixel{ //un pixel est définie par une couleur (rgb) et une opacité
    rgb color;
    unsigned int opac;
};
typedef struct pixel pixel;
/*-----------------------------------------------------------------------------------------------------------*/
/*la pille de calque pour l'état de la machinne*/
struct pill {
    int top;
    pixel **content[STACK_SIZE];
};
typedef struct pill pill;
/*---------------------------------------------------------------------------------------------------------------*/

/*les sceau de couleur et d'opacité*/
struct color_bucket{
    rgb *bucket; //le tableau avec les différente couleur (rgb)
    int len;     //combien de couleur il y a dans le scean
};
typedef struct color_bucket color_bucket;

struct opac_bucket{  //idem que color bucket mais avec des int pour l'opacité
    int *bucket;
    int len;
};
typedef struct opac_bucket opac_bucket;
/*-------------------------------------------------------------------------------------------------------------------*/

/*un struct afin de stocker tout les états de la machine*/
struct machine_state{  //struct où il y a toute les données de l'état de la machine
    int position[2];   //[0] ligne [1] les colonnes
    int mark_position[2];
    char direction;
    color_bucket color_b;
    opac_bucket opac_b;
    pill stack_layer;
};

typedef struct machine_state machine_state;
/*--------------------------------------------------------*/
/*la liste chainé pour faire une pill des positions à traité pour la fonction de remplissage*/
typedef int pos[2];
struct cell{
    pos position;
    struct cell *next;
};
typedef struct cell* list;

/*----------------------------------------------------------------------------------------------*/

/*
requires: nothing
assigns: nothing
ensures: return an empty stack
*/
pill initSTACK();

/*
requires: nothing
assigns: t.content, t.top
ensures: put the layer a on the top of t
*/
void push(pill *t, pixel **a);

/*
requires: nothing
assigns: t.top
ensures: return and remove the last element of t
*/
pixel **pop(pill *t);

/*
  requires:a valide liste and a position
  assigns:list l
  ensures:put a new element at the begining of the list
  */
void add(list *l,pos e);

/*
  requires:a volid list
  assigns:list
  ensures:free all the cells of the list to delet it
  */
void delet_list(list l);

/*
 requires:A positiv size, a not-NULL layer
 assigns:layer
 ensures:free the 2D layer
 */
void delet_layer(int size, pixel **layer);

/*requires:an initialise machine state
 * assigns:the color bucket of the machine state
 *ensures:a new bucket, with color c in it
 * */

void add_color(machine_state *A,rgb c);

/*requires:an initialise machine state
 * assigns:the opacity bucket of the machine state
 *ensures:a new bucket, with opacity opac =at the end
 * */
void add_opac(machine_state *A,int opac);

/*@requires:an initialise machine state
 *@assigns:the opaciry and color bucket  of the machine state
 *@ensures:to free both bucket and put the lenght of the bucket at -1
 */
void empty_bucket(machine_state *A);

/*requires:an initialise machine state
 * assigns:A->stack_layer
 *ensures:put a layer with 0 everywhere on the top of A->stack_layer
 * */
void add_layer(machine_state *A,int size);

/* requires:A positive size
 * assigns: nothing
 * ensures:an initialise machine state
 */
machine_state init_machine_state(int s);

/*
  requires:an initialise machine state
  assigns:assigns:the machine state
  ensures:free all the memory alocated for the machine state
  */

void delet_machine_state(machine_state *A,int size);

#endif /* IMRPIMERIE_H */
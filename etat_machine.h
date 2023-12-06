#ifndef ETAT_MACHINE_H
#define ETAT_MACHINNE_H

#include "imprimerie.h"
/* requires:an initalise machine state
 * assigns:return a pixel
 * ensures:a current pixel make by a mix of opacity and color bucket
 */
pixel current_pixel(machine_state A);

/* requires:an initialise machine state
 * assigns:A->mark_position
 * ensures:change the mark position by the actual position
 */
void change_mark_position(machine_state *A);

/* requires:an initialise machine state
 * assigns: A->position
 * ensures:move the cursor A->direction
 */
void move(machine_state *A,int size);

/* requires:an initialise machine state
 * assigns:A->direction
 * ensures:change the direction by the "sense horraire"
 */
void change_direction_horraire(machine_state *A);

/* requires:an initialise machine state
 * assigns:A->direction
 * ensures:change the direction by the "sens anti-horraire"
 */
void change_direction_antihorraire(machine_state *A);

/* requires:nothing
 * assigns:nothing
 * ensures:return the biggest absolute value value between a and b
 */
int max(int a, int b);

/* requires:an initialise machine state
 * assigns:A->stack_layer.content[A->stack_layer.top]
 * ensures:print a line with the current pixel form the mark_position to the position on the top layer
 */
void print_line(machine_state *A);

/*
  requires:an initialise machine state
  assigns:assigns:A->stack_layer.content[A->stack_layer.top]
  ensures:fill the zone of the same color where the position is with the new color and opacity current pixel
  */

void fill_machine_state(machine_state *A,int size);

/*
  requires:an initialise machine state
  assigns:assigns:A->stack_layer.content[A->stack_layer.top],A->stack_layer.content[A->stack_layer.top-1]
  ensures:mix the two highest layer ont the stack
  */

void mix_layer(machine_state *A,int size);

/*
  requires:an initialise machine state
  assigns:assigns:A->stack_layer.content[A->stack_layer.top],A->stack_layer.content[A->stack_layer.top-1]
  ensures:delet the layer at the top of the stack and use opacity of each pixel on top to modify the layer bellow
  */
void cut_layer(machine_state *A,int size);



#endif
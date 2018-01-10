 /**
 * Machine Problem 1
 * CS 241 - Spring 2016
 */

/* An automatically-expanding array of strings. */
#include "vector.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define INITIAL_CAPACITY 10

#define max(a,b) (((a) > (b)) ? (a) : (b))

Vector *Vector_create() {

  Vector * ret = malloc(sizeof(Vector));
  if(!ret){
    fprintf(stderr, "Malloc failed!\n");
    exit(1);
  }

  ret->array = malloc(INITIAL_CAPACITY*sizeof(char*)); 
  ret->size = 0;
  ret->capacity = INITIAL_CAPACITY;
  
  return ret;
}

void Vector_destroy(Vector *vector) {
  assert(vector);
  size_t i;
  for(i = 0; i < (vector->size); i++){
    if((vector->array)[i]) free((vector->array)[i]);
  }
  free(vector->array);
  free(vector);
}

size_t Vector_size(Vector *vector) {
  assert(vector);
  return vector->size;
}

void Vector_resize(Vector *vector, size_t new_size) {
  assert(vector);
  assert(new_size > 0);

  // CASE ONE: Same size
  if(new_size == vector->size){
    return; // nothing to be done
  }

  // CASE TWO: New size less than orig size and capacity
  size_t old_capacity = vector->capacity;
  if((new_size < vector->size) && (new_size < old_capacity)){
    size_t i;
    for(i = new_size; i < vector->size; i++){
      if((vector->array)[i]) free((vector->array)[i]); // free data outside of size range
    }
    vector->size = new_size;
    return;
  }
  // CASE THREE: New size greater than old size but less or equal to old capacity
  else if((new_size > vector->size) && (new_size <= vector->capacity)){
    size_t i;
    for(i = vector->size; i < new_size; i++) (vector->array)[i] = NULL; // NULL padding
    vector->size = new_size;
    return;
  }

  // CASE FOUR: New size greater than old size and old capacity
  else if(new_size > old_capacity){
    size_t new_capacity = max(old_capacity * 2, new_size);
    char ** new = malloc(new_capacity * sizeof(char*)); //allocate new array
    if(!new){
      fprintf(stderr, "Malloc failed!\n");
      exit(1);
    }
    size_t i;
    for(i = vector->size; i < new_size; i++) new[i] = NULL; // NULL padding
    for(i = 0; i < vector->size; i++) new[i] = (vector->array)[i]; // copy pointers to data
    free(vector->array);    
    vector->capacity = new_capacity;
    vector->size = new_size;
    vector->array = new;
    return;
  }
}


void Vector_set(Vector *vector, size_t index, const char *str) {
  assert(vector);
  assert(index >=  0);
  assert(index < vector->size);

  if((vector->array)[index]) free((vector->array)[index]);

  if(!str){
    (vector->array)[index] = NULL;
    return;
  }

  (vector->array)[index] = malloc(strlen(str)+1);
  strcpy((vector->array)[index],str);
  return;
}

const char *Vector_get(Vector *vector, size_t index) {
  assert(vector);
  assert(index >= 0);
  assert(index < vector->size);

  return (vector->array)[index];
}

void Vector_insert(Vector *vector, size_t index, const char *str) {
  assert(vector);
  assert(index >= 0);
  
  size_t resizeTo;
  if(index < vector->size) resizeTo = (vector->size)+1; // calculate new vector size
  else resizeTo = index+1;

  Vector_resize(vector, resizeTo); // resize

  if(resizeTo == (index+1)){
    if(!str) (vector->array)[index] = NULL;
    else {
      (vector->array)[index] = malloc(strlen(str)+1);
      strcpy((vector->array)[index],str); // if index is being inserted at index >= size, no shifting should need to take place
    }
    return;
  }

  // in the case we do need to shift:
  char * temp = (vector->array)[index];
  char * temp2;
  if(!str){
    (vector->array)[index] = NULL;
  }
  else{
    (vector->array)[index] = malloc(strlen(str)+1);
    strcpy((vector->array)[index],str);
  }

  size_t i;
  for(i = index+1; i < vector->size; i++){
    temp2 = (vector->array)[i];
    (vector->array)[i] = temp;
    temp = temp2;
  }
  
  return;
}

void Vector_delete(Vector *vector, size_t index) {
  assert(vector);
  assert(index >= 0);
  assert(index < vector->size); // can't delete something outside of the vector

  if(index == ((vector->size)-1)){ // simplest case: no shifting needed.
      Vector_resize(vector, index); // just resize so the last element is deallocated
      return;
    }

  // otherwise, we need to manually dealloc and shift
  if((vector->array)[index]) free((vector->array)[index]);
  
  char * temp;
  size_t i;
  for(i = index; i < ((vector->size)-1); i++){  // shifting
    temp = (vector->array)[i+1];
    (vector->array)[i] = temp;
  }
  (vector->array)[(vector->size)-1] = NULL; //remember to set last entry to NULL so we don't accidentally free data in resize()
  // finally, resize to correct size
  Vector_resize(vector, ((vector->size)-1));

  return;
}

void Vector_append(Vector *vector, const char *str) {
  assert(vector);
  
  Vector_resize(vector, ((vector->size)+1)); // grow vector by 1
  
  if(!str){
      (vector->array)[(vector->size)-1] = NULL;
    }
  else{
    (vector->array)[(vector->size)-1] = malloc(strlen(str)+1);
    strcpy((vector->array)[(vector->size)-1], str); // copy str into last cell;
  }
  return;
}
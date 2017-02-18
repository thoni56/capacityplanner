#ifndef VECTOR_H
#define VECTOR_H

typedef void (*GenericDestructor)(void *);
typedef struct Vector_ Vector;

Vector *create_vector(GenericDestructor destructor);
void destroy_vector(Vector *vector);
void add_item_to_vector(Vector *vector, void *item);
void *vector_remove(Vector *vector, int position);
void *get_item_from_vector(const Vector *vector, int position);
int size_of_vector(const Vector *vector);

#endif

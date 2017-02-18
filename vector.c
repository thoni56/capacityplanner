#include "vector.h"

#include <stdio.h>
#include <stdlib.h>

struct Vector_ {
    int size;
    void (*destructor)(void *);
    int space;
    void **items;
};

static void increase_space(Vector *vector);

Vector *create_vector(void (*destructor)(void *)) {
    Vector *vector = (Vector *)malloc(sizeof(Vector));
    vector->size = 0;
    vector->destructor = destructor;
    vector->space = 0;
    vector->items = NULL;
    return vector;
}

void destroy_vector(Vector *vector) {
    int i;
    if (vector->destructor != NULL) {
        for (i = 0; i < vector->size; i++) {
            (*vector->destructor)(vector->items[i]);
        }
    }

    free(vector->items);
    vector->items = NULL;
    vector->destructor = NULL;
    vector->size = 0;
    vector->space = 0;

    free(vector);
}

void add_item_to_vector(Vector *vector, void *item) {
    if (vector->size == vector->space) {
        increase_space(vector);
    }
    vector->items[vector->size] = item;
    vector->size++;
}

void *vector_remove(Vector *vector, int position) {
    void *item;
    int i;

    if (position < 0 || position > size_of_vector(vector)) {
        fprintf(stderr, "\tCGREEN INTERNAL ERROR: illegal position (%d) in vector operation\n", position);
        return NULL;
    }

    item = vector->items[position];

    for (i = position; i < vector->size; i++) {
        vector->items[i] = vector->items[i + 1];
    }

    vector->items[vector->size] = NULL;

    vector->size--;
    return item;
}

void *get_item_from_vector(const Vector *vector, int position) {
    if (position < 0 || position > size_of_vector(vector)) {
        return NULL;
    }
    return vector->items[position];
}

int size_of_vector(const Vector *vector) {
    return (vector == NULL ? 0 : vector->size);
}

static void increase_space(Vector *vector) {
    vector->space += 100;
    vector->items = (void**)realloc(vector->items, sizeof(void *) * vector->space);
}

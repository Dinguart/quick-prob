#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

typedef struct {
    int x;
    int y;
} Pair;

typedef struct Event;

typedef struct {
    bool (*condition)(int);
    int *data; // for now use int rather than a generic structure
    Pair bounds;
    size_t size;
} Event;

typedef struct {
    Event sample;
    Event *partitions;
    size_t part_size;
} SampleSpace;

bool equal_two(int num) {
    return num > 1;
}

/*
void fill_event(Event **e, bool (*condition)) {}

-> Allocates and sets e->data.
-> sets e->size.
*/
void fill_event(Event **e, bool (*condition)(int)) { // use a function pointer inside of the parameter list which is a bool function (describing the condition)
    bool increasing = (*e)->bounds.x < (*e)->bounds.y;

    int num = increasing ? (*e)->bounds.x : (*e)->bounds.y;
    int inc = increasing ? 1 : -1;
    bool cond = increasing ? num < (*e)->bounds.y : num >= (*e)->bounds.x;

    size_t bound_size = increasing ? fabs((*e)->bounds.y - (*e)->bounds.x) : fabs((*e)->bounds.x - (*e)->bounds.y);
    (*e)->size = bound_size; // initially we set the size to the range of the bounds (as we do not know the result of the condition yet.
    size_t bound_idx=0;
    int bound_arr[bound_size];
    memset(bound_arr, 0, sizeof(bound_arr));

    for (num; cond; num += inc) {
        if (condition(num)) bound_arr[bound_idx++] = num; // condition used arbitrarily by the caller.
        cond = increasing ? num < (*e)->bounds.y : num >= (*e)->bounds.x;
    }
    if (!bound_idx) {
        (*e)->size = 0;
        return;
    }
    (*e)->size = bound_idx; // set the new bound size after the condition modification
    (*e)->data = malloc((*e)->size * sizeof(int));
    memcpy((*e)->data, bound_arr, (*e)->size * sizeof(int));
}

Event *define_event(bool (*condition)(int), Pair bounds) {
    Event *e = malloc(sizeof(Event));
    e->condition = condition;
    e->bounds = bounds;
    fill_event(&e, condition);
    return e;
}

int main(void) {
    Pair bound = { 1, 4 };
    Event *test = define_event(equal_two, bound);
    for (size_t i=0; i<test->size; ++i) {
        printf("%d\n", test->data[i]);
    }
    free(test->data);
}

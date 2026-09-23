#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#include "prob.h"

bool greater_two(int num) {
    return num > 2;
}

bool less_four(int num) {
    return num < 4;
}

/*
void fill_event(Event **e, bool (*condition)) {}

-> Allocates and sets e->data.
-> sets e->size.
*/
void fill_event(struct Event **e, bool (*condition)(int), const char *name) { // use a function pointer inside of the parameter list which is a bool function (describing the condition)
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

    // set the name that user provided (if exists)
    if (!name) return;
    size_t name_len = strlen(name);
    (*e)->name = malloc(name_len);
    memcpy((*e)->name, name, name_len);
}

struct Event *define_event(bool (*condition)(int), Pair bounds, const char *name) {
    struct Event *e = malloc(sizeof(struct Event));
    e->condition = condition;
    e->bounds = bounds;
    fill_event(&e, condition, name);
    return e;
}

struct SampleSpace *define_sample_space(bool (*sample_condition)(int), Pair sample_bounds, const char *sample_name) {
    struct SampleSpace *s = malloc(sizeof(struct SampleSpace));
    s->sample_event = define_event(sample_condition, sample_bounds, sample_name);
    s->part_cap = PARTITION_CAP_DEFAULT;
    s->part_size = 0;
    s->partitions->next = malloc(sizeof(struct EventNode));
    s->partitions->prev = malloc(sizeof(struct EventNode));
    s->partitions->next = s->partitions->prev;
    s->partitions->prev = s->partitions->next;
    return s;
}

void sample_push_event(struct SampleSpace **s, bool (*condition)(int), Pair bounds, const char *name) {
    if ((*s)->part_size > (*s)->part_cap) return;

    struct EventNode *node = malloc(sizeof(struct EventNode));
    node->event = define_event(condition, bounds, name);
    
    struct EventNode *tmp = (*s)->partitions;
    node->prev = (*s)->partitions->next;
    node->next = NULL;
    (*s)->partitions->next = node;
    (*s)->partitions->prev = tmp;
}

void print_event_space(const struct Event *e) {
    if (e->name) printf("%s ", e->name);
    printf("{");  
    for (size_t i = 0; i < e->size; ++i) {
        if (i < e->size - 1)
        printf("%d, ", e->data[i]);
        else printf("%d}\n", e->data[i]);
    }  
}

void print_sample_space(const struct SampleSpace **s) {
    printf("Sample Space: ");
    print_event_space((*s)->sample_event);
    printf("\n");
    struct EventNode *tmp = (*s)->partitions;
    while (tmp) {
        print_event_space(tmp->event);
        tmp = tmp->next;
    }
}

void test_sample_space() {
    Pair bound = { 1, 10 };
    const char *name = "Integers greater than 2.";
    struct SampleSpace *s = define_sample_space(greater_two, bound, name);

    Pair test_bound = { 1, 4 };
    const char *test_name = "Integers greater than 2, and less than 4."; // each event must follow the condition of the sample space.
    sample_push_event(&s, less_four, bound, test_name);

    print_sample_space(&s);
}

int main(void) {
    /*Pair bound = { 1, 4 };
    Event *test = define_event(equal_two, bound);
    print_event_space(test);
    free(test->data);
    */
    test_sample_space();
}

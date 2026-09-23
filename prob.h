#ifndef PROB_H_
#define PROB_H_
// general
typedef struct {
    int x;
    int y;
} Pair;

// specific
typedef struct Event {
    const char *name;
    bool (*condition)(int);
    int *data;
    Pair bounds;
    size_t size;
}; 

typedef struct EventNode {
    struct Event *event;
    struct EventNode *next;
    struct EventNode *prev;
};

typedef struct SampleSpace {
    struct Event *sample_event;
    struct EventNode *partitions;
    size_t part_size;
    size_t part_cap;
};

#define PARTITION_CAP_DEFAULT (size_t)2

#endif

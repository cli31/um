/* all the includes */
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/* Seq ADT */
#define T Seq_T

typedef struct T {
        /* index is uint32_t */
        uint32_t length;
        uint32_t capacity;
        void **p; /* used in my um, p will always be uint32_t ** */
} * T;

static inline T Seq_new(int HINT) {
        T new_Seq = (T)malloc(sizeof(struct T));
        new_Seq->p = (void **)malloc(sizeof(uint32_t *) * HINT);
        new_Seq->length = 0;
        new_Seq->capacity = HINT;
        return new_Seq;
}

static inline uint32_t Seq_length(T Seq) {
        return Seq->length;
}

static inline void * Seq_get(T Seq, uint32_t idx) {
        return *(Seq->p + idx);
}

/* change the return from original ith index to void return since we
   did not use this feature in um */
static inline void Seq_put(T Seq, uint32_t idx, void *elem) {
        *(Seq->p + idx) = elem;
}

/* samely void as Seq_put */
static inline void Seq_addhi(T Seq, void *elem) {
        /* enlarge capacity */
        if (Seq->length == Seq->capacity - 1) {
                if (Seq->capacity >= (1UL << 31)) {
                        Seq->capacity = -1; /* set capacity to MAX */
                }
                else {
                        Seq->capacity = Seq->capacity << 1;
                }
                void **t = (void **)malloc(sizeof(uint32_t *) * Seq->capacity);
                assert(t); /* memory exhaust */
                memcpy(t, Seq->p, Seq->length * sizeof(uint32_t *));
                free(Seq->p);
                Seq->p = t;
        }
        *(Seq->p + Seq->length) = elem;
        Seq->length++;
}

/* do return the original elem here */
static inline void * Seq_remhi(T Seq) {
        /* do not delete it but just shrink the length */
        Seq->length--;
        return *(Seq->p + Seq->length);
}

static inline void Seq_free(T Seq) {
        free(Seq->p);
        free(Seq);
}

/* um */
typedef struct UM {
        uint32_t r[8]; /* 8 general-purpose registers, 32-bit each.*/
        T mem_space;
        T unmapped_ids;
        uint32_t counter;
} UM;

#undef T

/* macros used to parse inst */
#define opcode (N >> 28)
#define A (((N) >> 6) & 0x7)
#define B (((N) >> 3) & 0x7)
#define C ((N)        & 0x7)
#define RA (um.r[A])
#define RB (um.r[B])
#define RC (um.r[C])
#define value ((N << 7) >> 7)
#define Rvalue (um.r[(N >> 25) & 0x7])

/* main */

int main(int argc, char *argv[]) 
{
        /* Usage: ./um [file.um] */
        if (argc != 2) {
        fprintf(stderr, "Usage: ./um [file.um]\n");
        exit(EXIT_FAILURE);
        }
        FILE *fp = fopen(argv[1], "rb");
        if (!fp) {
        fprintf(stderr, 
                "Error: Cannot open file '%s' for reading.\n", 
                argv[argc - 1]);
        exit(EXIT_FAILURE);
        }
        /* template done, code starts here */
        /* it is an oop idea, code inside of this block is equivalent to 
        um.run() */
        /* Step1: read in instructions from fp */
        /* get the size of file and see if it has complete instructions */
        fseek(fp, 0, SEEK_END);
        uint32_t num_of_inst = ftell(fp);
        num_of_inst /= 4;
        fseek(fp, 0, SEEK_SET);
        uint32_t *buffer = (uint32_t *)malloc(sizeof(uint32_t) * (num_of_inst + 1));
        buffer[0] = num_of_inst;
        for (uint32_t i = 1; i <= num_of_inst; i++) {
                uint32_t a, b, c, d;
                a = getc(fp);
                b = getc(fp);
                c = getc(fp);
                d = getc(fp);
                buffer[i] = (a << 24) | (b << 16) | (c << 8) | d;
        }
        fclose(fp);
        /* Step2: initialize um */
        UM um;
        for (int i = 0; i < 8; i++) {
                um.r[i] = 0;
        }
        um.counter = 1; /* since idx 0 is always the size */
        um.mem_space    = Seq_new(1UL << 10);
        um.unmapped_ids = Seq_new(1UL << 10);
        /* $m[0] is first loaded with program */
        Seq_addhi(um.mem_space, buffer);

        /* Step3: execution cycle */
        uint32_t N;
        for ( ;; um.counter++) {
                /* use buffer instead of Seq_get for a simpler expression */
                /* reset of program ptr was done in load program block */
                N = buffer[um.counter];
                switch (opcode) {
                case 0:
                        if (RC != 0) {
                                RA = RB;
                        }
                        break;
                case 1: {
                        /* retrieve the segment with id $r[b] */
                        uint32_t *curr_seg = (uint32_t *)Seq_get(um.mem_space, RB);
                        RA = curr_seg[RC + 1]; /* size takes the 0 idx */
                        break;
                }
                case 2: {
                        uint32_t *curr_seg = (uint32_t *)Seq_get(um.mem_space, RA);
                        curr_seg[RB + 1] = RC;
                        break;
                }
                case 3:
                        RA = RB + RC;
                        break;
                case 4:
                        RA = RB * RC;
                        break;
                case 5:
                        RA = RB / RC;
                        break;
                case 6:
                        RA = ~(RB & RC);
                        break;
                case 7: {
                        uint32_t length = Seq_length(um.mem_space);
                        for (uint32_t i = 0; i < length; i++) {
                                free(Seq_get(um.mem_space, i));
                        }
                        Seq_free(um.mem_space);
                        Seq_free(um.unmapped_ids);
                        return 0;
                }
                case 8: {
                        uint32_t *seg = NULL;
                        uint32_t id = 0; /* invalid */
                        uint32_t size = RC;
                        /* if there is no available id, alloc a new memory segment */
                        if (Seq_length(um.unmapped_ids) == 0) {
                                seg = (uint32_t *)malloc(sizeof(uint32_t) * (size + 1));
                        }
                        else {
                                /* use from high end like a stack */
                                id = (uint32_t)(uintptr_t)Seq_remhi(um.unmapped_ids);
                                seg = (uint32_t *)Seq_get(um.mem_space, id);
                                /* seg is NULL */
                                seg = (uint32_t *)malloc(sizeof(uint32_t) * (size + 1));
                        }

                        seg[0] = size; /* set the first word as size */
                        /* initialize each word to 0 */
                        for (uint32_t i = 1; i <= size; i++) {
                                seg[i] = 0;
                        }

                        if (id == 0) { /* if id is new, push to the high end */
                                Seq_addhi(um.mem_space, seg);
                                RB = Seq_length(um.mem_space) - 1; 
                        }
                        else { /* if id is recycled, update mem_space (id has popped when 
                                        called) */
                                Seq_put(um.mem_space, id, seg);
                                RB = id;
                        }
                        break;
                }
                case 9: {
                        uint32_t id = RC;
                        uint32_t *seg = (uint32_t *)Seq_get(um.mem_space, id);
                        /* free that segment and put NULL onto the seq */
                        free(seg);
                        seg = NULL; /* protective coding: in case double free */
                        Seq_put(um.mem_space, id, seg);
                        /* push the id to reusable seq */
                        Seq_addhi(um.unmapped_ids, (void *)(uintptr_t)id);
                        break;
                }
                case 10:
                        putchar((char)RC);
                        break;
                case 11: {
                        int input = fgetc(stdin);
                        if (input != EOF) {
                                RC = input;
                        } else {
                                RC = -1;
                        }
                        break;
                }
                case 12: {
                        um.counter = RC;
                        if (RB == 0) {
                                break; /* do nothing == extremely quick */
                        }
                        /* get $m($r[b]) */
                        uint32_t *this_seg = (uint32_t *)Seq_get(um.mem_space, RB);
                        /* free $m[0] */
                        free(Seq_get(um.mem_space, 0));
                        /* duplicate */
                        uint32_t size = this_seg[0];
                        uint32_t *new_seg = (uint32_t *)malloc(sizeof(uint32_t) * (size + 1));
                        memcpy(new_seg, this_seg, sizeof(uint32_t) * (size + 1));
                        /* the duplicate replace $m[0] */
                        Seq_put(um.mem_space, 0, new_seg);
                        buffer = new_seg;
                        break;
                }
                case 13: {
                        Rvalue = value;
                        break;
                }
                default:
                        assert(false);
                        break;
                }
        }
        return 0;
}
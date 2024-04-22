#include "my_seq.h"
#include <stdio.h>
#include <stdbool.h>

/* the compile path was updated in Makefile by using make Seq_test */
int main() {
        Seq_T t = Seq_new(1);
        printf("test Seq_new: %s\n", Seq_length(t) == 0 ? "good" : "fail");
        uint32_t arr[5] = { 0, 1, 2, 3, 4 };
        uint32_t *p = &arr[0];
        bool b = true;
        for (uint32_t i = 0; i < 5; i++) {
                Seq_addhi(t, p + i);
                b = b && *(uint32_t *)Seq_get(t, i) == i;
        }
        printf("test Seq_addhi: %s\n", Seq_length(t) == 5 ? "good" : "fail");
        printf("test Seq_get: %s\n", b ? "good" : "fail");

        for (uint32_t i = 5; i != 0; i--) {
                b = b && (i - 1) == *(uint32_t *)Seq_remhi(t);
        }
        printf("test Seq_remhi: %s\n", Seq_length(t) == 0 && b ? "good" : "fail");

        Seq_free(&t);
}
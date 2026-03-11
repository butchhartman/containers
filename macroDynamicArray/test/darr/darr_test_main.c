#include "darr/darr.h"
#include <stdio.h>
#include <assert.h>
#include <time.h>

#define GROW_NUM_TRIALS 50000

struct dynArrStruct {
        float* darr;
};

float* grow_method_1(float* arr) {
        for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                DARRAY_PUSH(arr, i);
        }

        return arr;
}

void grow_method_2(float** arr) {
        for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                DARRAY_PUSH_ESF(arr, i);
        }
}

float* struct_grow_method_1(struct dynArrStruct* stru) {
        for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                DARRAY_PUSH(stru->darr, i);
        }
        
        return stru->darr;
}

void struct_grow_method_2(struct dynArrStruct* stru) {
        for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                DARRAY_PUSH(stru->darr, i); // If ptr is in struct, you dont have to use ESF push. Do I want to enforce a darray struct type beyond just a raw ptr?
        }
}

int main(void) {

        // Test that growing an array from the smallest size works fine
        clock_t start = clock();
        for (size_t times = 0; times < GROW_NUM_TRIALS; times++) {
                int* arr = DARRAY_NEW(int, 0);

                for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                        DARRAY_PUSH(arr, i);
                }

                for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                        assert(DARRAY_GET(arr, i) == i);
                }

                DARRAY_FREE(arr);
        }
         
        clock_t final = clock();
        double elapsed = (double)(final - start) / CLOCKS_PER_SEC;
        printf("Time elapsed for basic grow: %f seconds\n", elapsed);

        // Test that array can be grown from a function (method 1)
        start = clock();
        for (size_t times = 0; times < GROW_NUM_TRIALS; times++) {
                float* arr = DARRAY_NEW(float, 0);
                arr = grow_method_1(arr);
                for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                        assert(DARRAY_GET(arr, i) == i);
                }
                DARRAY_FREE(arr);
        }
        final = clock();
        elapsed = (double)(final - start) / CLOCKS_PER_SEC;
        printf("Time elapsed for function grow 1: %f seconds\n", elapsed);

        // Test that array can be grown from a function (method 2)
        start = clock();
        for (size_t times = 0; times < GROW_NUM_TRIALS; times++) {
                float* arr = DARRAY_NEW(float, 0);
                grow_method_2(&arr);
                for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                        assert(DARRAY_GET(arr, i) == i);
                }
                DARRAY_FREE(arr);
        }
        final = clock();
        elapsed = (double)(final - start) / CLOCKS_PER_SEC;
        printf("Time elapsed for function grow 2: %f seconds\n", elapsed);

        // Test array growing in a struct (method 1)
        start = clock();
        for (size_t times = 0; times < GROW_NUM_TRIALS; times++) {
                struct dynArrStruct mys = {.darr = DARRAY_NEW(float, 0)};
                mys.darr = struct_grow_method_1(&mys);
                for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                        assert(DARRAY_GET(mys.darr, i) == i);
                }
                DARRAY_FREE(mys.darr);
        }
        final = clock();
        elapsed = (double)(final - start) / CLOCKS_PER_SEC;
        printf("Time elapsed for function grow in struct 1: %f seconds\n", elapsed);


        // Test array growing in a struct (method 2)
        start = clock();
        for (size_t times = 0; times < GROW_NUM_TRIALS; times++) {
                struct dynArrStruct mys = {.darr = DARRAY_NEW(float, 0)};
                struct_grow_method_2(&mys);
                for (size_t i = 0; i < GROW_NUM_TRIALS; i++) {
                        assert(DARRAY_GET(mys.darr, i) == i);
                }
                DARRAY_FREE(mys.darr);
        }
        final = clock();
        elapsed = (double)(final - start) / CLOCKS_PER_SEC;
        printf("Time elapsed for function grow in struct 2: %f seconds\n", elapsed);
}

#pragma once
#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdbool.h>
#define NOB_STRIP_PREFIX
#include "nob.h"
#include <SDL.h>

// ---------------------
//  Dynamic Arrays
// \/-------------------

/* 
 EXAMPLE:

 dynamic_array(char) xs = {0};
 da_append(&xs, 'a');
 da_append(&xs, 'b');
 da_append(&xs, '7');
 
 printf("%c\n", xs.items[0]);

-----------------------------------

 dynamic_array(char) xs = {0};
 sb_append_cstr(&xs, "abcdeFG"); // YES |SB|_append_cstr

 printf("%s\n", xs.items);
*/
#define dynamic_array(type) \
struct {                    \
    type* items;            \
    int capacity;           \
    int count;              \
}

#define da_contains(da, con_item) ({                                          \
bool yes = false;                                                             \
for (typeof(*((da)->items)) *it = (da)->items; it < (da)->items + (da)->count; ++it) { \
    if (*it == con_item){                                                     \
        yes = true;                                                           \
        break;                                                                \
    }                                                                         \
}                                                                             \
yes;})

#define da_contains_cstr(da, con_item) ({                                     \
bool yes = false;                                                             \
for (typeof(*((da)->items)) *it = (da)->items; it < (da)->items + (da)->count; ++it) { \
    if (strcmp(*it, con_item) == 0){                                                     \
        yes = true;                                                           \
        break;                                                                \
    }                                                                         \
}                                                                             \
yes;})

#define da_pop(popped_da) ({                                \
    typeof(*((popped_da)->items)) val = da_last(popped_da); \
    (popped_da)->count--;                                   \
    val;                                                    \
})

#define da_printf(da, print_string)                                                \
do {                                                                                       \
    for (typeof(*((da)->items)) *it = (da)->items; it < (da)->items + (da)->count; ++it) { \
        printf(print_string, *it);                                                         \
    }                                                                                      \
} while(0)

// ---------------------
//  Logging
// \/-------------------

void error(const char* msg);
void warning(const char* msg);
void todo(const char* msg);

// ---------------------
//  Asserts
// \/-------------------

void errorz(int val, const char* msg);
void errornz(int val, const char* msg);
void errorpz(void* val, const char* msg);
void errorpnz(void* val, const char* msg);

#endif //__UTILS_H__
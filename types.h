#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//#include <assert.h>
#define static_assert _Static_assert
#define static_assert_sizeof(N, S) static_assert(sizeof(N) == S, #N "was expected to be " #S " long")

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

// one of these two must go

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef unsigned int uint;
typedef u8 string11[11];

void *memset(void*, int, size_t);
void *memcpy(void*, void*, size_t);

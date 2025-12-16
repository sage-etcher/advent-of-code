
#ifndef IVEC_H
#define IVEC_H

#include <inttypes.h>
#include <stdint.h>

typedef struct { int x, y, z; } ivec3_t;
typedef struct { int x, y; } ivec2_t;

typedef struct { uint32_t x, y, z; } u32vec3_t;
typedef struct { uint32_t x, y; } u32vec2_t;

typedef struct { uint64_t x, y, z; } u64vec3_t;
typedef struct { uint64_t x, y; } u64vec2_t;

#endif

/* end of file */

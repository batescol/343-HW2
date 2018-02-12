#pragma once

#include <stdint.h>

// The size of the "chunk" we operate on internally
#define TP uint64_t

// The number of cells we can fit in one TP (2 per byte)
#define sTP (sizeof(TP)*2)

// The number of TP high we need to go to have certain
// pixel height. We need a buffer of 1 each for top and
// bottom because we add the row above for the top row,
// and the row below for the bottom row
#define H(pixels) ((pixels)+2)

// The number of TP wide we need to go to have a certain
// pixel width. We need a buffer of 1 for each side for
// similar reasons to above. This is basically
// ceil(pixels/sTP)+2 but with hacky int ceil
#define W(pixels) ((pixels)/sTP+2+((pixels)%sTP?1:0))

// The rule constant. This determines when a cell is
// alive or dead. It is split into two parts. The low
// half's low 9 bits show when it is alive when it was
// dead last generation, and the high half's low 9 bits
// show when it is alive when it was alive last gen.
// EX: Conway's GoL is B3/S23 -> 0000001000 when dead,
// 0000001100 when alive -> 0x000C0008
#define RULE (0x000C0008)
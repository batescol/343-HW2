#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "macros.h"
#include "board.h"
#include "file_utilities.h"

/*
	Author: Colton Bates
	File:   board.c
	Desc.:	GoL implementation and dealings directly with board data
*/

Board* allocB(int wPixels, int hPixels) {
	// The size of the data member. Must be as big as three boards
	// See stepBoard() implementation for why
	int size = H(hPixels)*W(wPixels)*3;

	// Allocating and zeroing board data
	TP* d = (TP*)malloc(size*sizeof(TP));
	memset(d, 0, size*sizeof(TP));

	// Allocating and assigning struct members
	Board* q = (Board*)malloc(sizeof(Board));
	q->data = d;
	q->hPixels = hPixels;
	q->wPixels = wPixels;
	q->bs = 0;

	return q;
}

void deallocB(Board* q) {
	free(q->data);
	free(q);
}

// Because of the way stepBoard is implemented, we need
// this to ensure that the parts that are not supposed
// to exist stay zeros
void cutToSize(Board* q) {
	// These next three lines are practically idiomatic
	// for this project. The third is confusing
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	TP* cboard = q->data+(q->bs&1)*hSize*wSize;

	// Normally all TPs are sTP cells wide, but if the
	// width does not divide evely, the last might be less.
	// We need to set the ones beyond that edge to zero

	// How wide the last TP is (first in cells, then in bits)
	int lastWidth = q->wPixels % sTP;
	lastWidth = (sTP-lastWidth)*4;

	// Preparing mask to zero out beyond the edge
	TP lastMask = (TP)1<<lastWidth;
	lastMask = ~(lastMask-1);

	// Applying mask
	for (int i = 1; i < hSize-1; i++) {
		cboard[i*wSize+wSize-2] &= lastMask;
	}
}

void stepBoard(Board* q, unsigned int count) {
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	// This implementation uses two scratch boards to hold intermediate data, 
	// and because of limitations (and avoiding a copy back), the source board
	// cannot be the end result board. Because of this, the "active" board state
	// swaps between the first and second boards within the Board struct. The "bs"
	// struct member keeps track of which is active and which is scratch
	TP* b1 = q->data+(q->bs&1)*hSize*wSize;		// Current board + next second scratch board
	TP* b2 = q->data+		 2*hSize*wSize;		// Scratch board
	TP* b3 = q->data+(q->bs^1)*hSize*wSize;		// Second scratch board + next current board

	while (count--) {
		// Populate b2 with the number of horizontal neighbors each cell has alive
		for (int i = 1; i < hSize-1; i++) {
			for (int j = 1; j < wSize-1; j++) {
				// Each 4 bits contains a cell, so we can do up to 16 cells at a
				// time on 64-bit machines (if TP = uint64_t)
				b2[i*wSize+j] = (b1[i*wSize+j] << 4 | b1[i*wSize+j+1] >> (sTP-1)*4)		// R
						      + (b1[i*wSize+j] >> 4 | b1[i*wSize+j-1] << (sTP-1)*4);	// L
			}
		}


		for (int i = 1; i < hSize-1; i++) {
			for (int j = 1; j < wSize-1; j++) {
				// Use the data in b2 and b1 to get a grand total of alive
				// neighbors for each cell
				TP q = b1[(i-1)*wSize+j] + b2[(i-1)*wSize+j]		// U, UR, UL
					 + b1[(i+1)*wSize+j] + b2[(i+1)*wSize+j]		// D, DR, DL
					 + b2[i*wSize+j];								// L, R

				// Temp value so we don't do a memory access every loop
				TP t = 0;
				for (int k = 0; k < sTP; k++) {
					// See RULE in macros.h for a description of this bitwise
					// black magic
					t |= (TP)((RULE>>(
							(q >> (4*k) & 15) |							// count of neighbors
							(b1[i*wSize+j] >> (4*k) & 1) << 4)) &		// alive last frame
							1) << (4*k);
				}
				b3[i*wSize+j] = t;
			}
		}
		// Swap b1 and b3 pointers to prepare for another iteration
		TP* tmp = b1;
		b1 = b3;
		b3 = tmp;

		// Swap bs
		q->bs ^= 1;

		// Cull any cells that were off the edge but came to life
		cutToSize(q);
	}
}

// Don't use this. The compacted implementation means the coordinates
// are *all* messed up and I'm too lazy to fix
void setSC(Board* q, int x, int y, int val, int tog) {
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	TP* cboard = q->data+(q->bs&1)*hSize*wSize;

	int xIdx = x/sTP+1;
	int cIdx = x%sTP;
	int yIdx = y+1;
	if (tog) {
		cboard[yIdx*wSize+xIdx] ^= val<<(4*cIdx);
	} else {
		cboard[yIdx*wSize+xIdx] &= ~(7<<(4*cIdx));
		cboard[yIdx*wSize+xIdx] |= val<<(4*cIdx);
	}
}

// Sets a whole TP at a certain coordinate
void setC(Board* q, int x, int y, TP val) {
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	TP* cboard = q->data+(q->bs&1)*hSize*wSize;

	cboard[(y+1)*wSize+(x+1)] = val;
}

// Determines what char should be displayed at a certain coordinate
char gPChar(Board* q,int x, int y, int mode) {
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	TP* cboard = q->data+(q->bs&1)*hSize*wSize;

	if (mode == 0 || mode == 1) {
		int xIdx = x/sTP + 1;		// x coordinate in TPs
		int cIdx = x%sTP;			// x coordinate within the TP
		int yIdx = y+1;				// y coordinate in TPs
		int cellval = cboard[yIdx*wSize+xIdx]>>(4*(sTP-cIdx))&1;

		// " " for 0, "#" for 1
		return " #"[cellval];
	}
}

void printBoard(Board* q) {
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	TP* cboard = q->data+(q->bs&1)*hSize*wSize;

	// Display board size
	printf("%dx%d\n", q->wPixels, q->hPixels);

	for (int i = 1; i < hSize-1; i++) {
	 	for (int j = 1; j < wSize-1; j++) {
	 		// The second part of this loop condition makes it only
	 		// display up to the width, even though the TP width might
	 		// be longer
	 		for (int k = sTP-1; k >= 0 && sTP*j-k <= q->wPixels; k--) {
	 			printf("%c", " #"[cboard[i*wSize+j]>>(4*k)&1]);
	 		}
	 	}
	 	puts("");
	}
}

Board* loadFile(char* filename) {
	int bW = 0, bH = 0, l;
	TP* buf;
	// Get all data from file
	if(read_file(filename, &buf, &bW, &bH, &l) != 0)
		return NULL;

	Board* q = allocB(bW, bH);
	
	TP* cboard = q->data+(q->bs&1)*H(bH)*W(bW);
	memcpy(cboard, buf, l*sizeof(TP));

	free(buf);

	return q;
}

void saveFile(Board* q, char* filename) {
	int hSize = H(q->hPixels);
	int wSize = W(q->wPixels);

	TP* cboard = q->data+(q->bs&1)*hSize*wSize;

	// Don't need to allocate a read buffer, we already have
	// the board data in cboard :P
	write_file(filename, &cboard, q->hPixels, q->wPixels, hSize*wSize);
}
#pragma once

#include "macros.h" // For TP

/*
	Author: Colton Bates
	File:   board.h
	Desc.:	Utilities dealing directly with the board
		or board data
*/

// Simple struct to hold board data
typedef struct {
	// Pointer to actual data, but with extra space for calculation
	TP* data;

	// The size in pixels (cells) of the board
	int hPixels;
	int wPixels;

	// Indicates the current board. See stepBoard() in board.c for why this is needed
	int bs;
} Board;


// Allocates and deallocates the space required for board data
Board* allocB(int, int);
void deallocB(Board*);

// Ensures that extra cells beyond the edge are zero. See steapBoard() in board.c
// for why this is needed
void cutToSize(Board*);

// Steps the board however many iterations. Overly optimized and makes things difficult
void stepBoard(Board*, unsigned int);

// Sets a single cell on the board (or toggles it)
// The coordinates are all messed up, so it's not very useful
void setSC(Board*, int, int, int, int);

// Sets a single group of cells (by 1 TP). Not terribly useful
void setC(Board*, int, int, TP);

// Prints the board
void printBoard(Board*);

// Loads or saves a binary file containing a board setup
Board* loadFile(char*);
void saveFile(Board*, char*);
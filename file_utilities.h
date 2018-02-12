#pragma once

#include "macros.h" // For TP

/*
	Author: Colton Bates
	File:   file_utilities.h
	Desc.:	Reading and writing files containing board data
*/

// Reads the dimensions in pixels, the length in TPs, and the board data
// from a file where it has been saved. Returns 0 on success, -1 on failure
int read_file(char*, TP**, int*, int*, int*);

// Writes the dimensions, length, and board data ro a file such that it can
// be loaded again later. Returns 0 on success, -1 on failure
int write_file(char*, TP**, int, int, int);
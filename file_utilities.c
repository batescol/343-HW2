#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "macros.h" // For TP

/*
	Author: Colton Bates
	File:   file_utilities.c
	Desc.:	Reading and writing files containing board data
*/


int read_file(char* filename, TP **buffer, int* w, int* h, int* l) {

	FILE* f = fopen(filename, "rb");

	if (f == NULL) {
		printf("Open failed.\n");
		return -1;
	}

	// Read dimensions and data length
	fread(h, sizeof(int), 1, f);
	fread(w, sizeof(int), 1, f);
	fread(l, sizeof(int), 1, f);


	*buffer = (TP*)malloc((*h) * (*w) * sizeof(TP));
	if (fread(*buffer, sizeof(TP), *l, f) != *l) {
		printf("Load Failed.\n");
		free(*buffer);
		return -1;
	}

	fclose(f);

	return 0;
}

int write_file(char* filename, TP **buffer, int w, int h, int l) {
	FILE* f = fopen(filename, "wb");

	if (f == NULL) {
		printf("Open failed.\n");
		return -1;
	}

	// Write dimensions and data length
	fwrite(&h, sizeof(int), 1, f);
	fwrite(&w, sizeof(int), 1, f);
	fwrite(&l, sizeof(int), 1, f);

	fwrite(*buffer, sizeof(TP), l, f);

	fclose(f);

	return 0;
}
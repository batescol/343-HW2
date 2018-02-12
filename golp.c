#include <stdio.h>
#include <stdlib.h>

#include "board.h"

/*
	Author: Colton Bates
	File:   golp.c
	Desc.:	Entry point, user input, and main loop
*/

// Keep track of current board, as well as a temporary one
// so that we can check if loading was successful before
// freeing the old one
Board* b, *nb;

// Gets called in a loop
void menu() {
	// Display board and prompt
	printBoard(b);
	printf(">");

	char cmdBuf[50];
	fgets(cmdBuf, 100, stdin);
	char fname[20];
	int t;

	// First char of command determines which command it is
	switch (cmdBuf[0]) {
		// Step frames
		case 'f':;					// this semicolon is not a bug, it's required;
									// C doesn't allow assignments after labels
			int iterati = 1;

			// scanf for arguments to command
			sscanf(cmdBuf, "f %d", &iterati);
			stepBoard(b, iterati);
			break;

		// Quit
		case 'q':
			deallocB(b);
			exit(0);
			break;

		// Toggle a cell
		case 't':;
			int x = 0, y = 0;
			sscanf(cmdBuf, "t %d %d", &x, &y);
			if (x >= b->wPixels || y >= b->hPixels || x < 0 || y < 0) {
				printf("Coordinates out of range\n");
			} else {
				setSC(b, x, y, 1, 1);
			}
			break;

		// Save to file
		case 's':;
			t = sscanf(cmdBuf, "s %s", fname);
			if (t == 1)
				saveFile(b, fname);
			else
				saveFile(b, "h");
			break;
		case 'l':;

		// Load from file
			t = sscanf(cmdBuf, "l %s", fname);
			if (t == 1)
				nb = loadFile(fname);
			else 
				nb = loadFile("h");
			if (nb != NULL) {
				deallocB(b);
				b = nb;
				nb = NULL;
			}
			break;

	}
}

int main(int argc, char** argv) {
	// The board size is dynamic, but there's no way to input it yet,
	// so it's hardcoded to 20 x 20
	b = allocB(20, 20);
	while (1) {
		menu();
	}
}
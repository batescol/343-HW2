# 343-HW2
Project 2 for CIS-343.

Firstly, I went ahead and did it with only two states. Not only is it the proper way to do Conway's Game of Life, but I also spent a long time having fun over-optimizing the engine (I got it up to 30+ million cells per second) and I relied heavily on being able to store 1 cell per byte. I'd be happy (or willing, I guess) to redo it with separate dead/empty states in the name of "following the specs" if you deem it necessary.

## Documentation
### Compilation
I didn't include a makefile because it's so incredibly simple. `gcc -o proj *.c` or `clang -o proj *.c` will both compile it to `proj`, which can be run in the normal ways.

### Running
The program is based on a simple loop that displays the board (initially empty) and a prompt (`>`), and waits for user input. The command is determined by the first letter of the input:
* `f [num]` progresses the board by `num` (or `1` if not provided) steps.
* `q` quits the program
* `t [x] [y]` toggles a cell at a position (or (0,0) if not provided). The coordinate system is completely wrong but as long as you stay within a `TP` it usually works out.
* `s [fname]` saves to a file with name `fname`, or `h` if not provided.
* `l [fname]` loads a board from a file with name `fname`, or `h` if not provided.

I've provided a few test files. Because the savefile format is binary, it's a pain to edit, but it is possible with a hex editor (because there are 4 bits per cell, and that is 1 hex digit). I would recommend using the `t` command to help with testing but the coordinates mapping is hit-or-miss.
#### Provided files:
* `osc` is a single 3x1 oscillator
* `o4` is a square of 3x1 oscillators
* `h` is the default savefile so I have no idea what's in there

## Testing
Valgrind reports no possible memory leaks from my testing. For your convenience, here are all of the relevant parts of code:
* `allocB()` and `deallocB()` in `board.c` (at the top)
* `loadFile()` and `saveFile()` in `board.c` (at the bottom)
* `file_utilities.c` (obviously :P)
* The UI save/load logic in `menu()` in `golp.c` (at the bottom of the function)
* And that single `allocB()` in `main()` in `golp.c`

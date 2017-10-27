/*
 * Display utility functions.
 * display.h
 * Author: Adel Khial
 */

#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

/**
 * Positions the console cursor to the specified x & y coordinates,
 * note that this implementation is specific to the windows API and
 * is not compatible with *nix systems.
 *
 * @param x the x coordinate in the console.
 * @param y the y coordinate in the console.
 */

void setCursorPosition(short x, short y);

/**
 * Returns the console window size (not the buffer size).
 *
 * @return returns a 2-element array with element 0 being the number
 * of columns and 1 being the number of lines.
 */

short* getConsoleSize();

/*
 * Changes the console window size.
 *
 * @param cols  the number of columns or width of the console.
 * @param lines the number of lines or height of the console.
 *
 * @return true if completed succesfully, false otherwise.
 */

bool setConsoleSize(short cols, short lines);

/*
 * Sets the cursor color according to the console color codes.<br>
 *
 *  0 = Black       8 = Gray <br>
 *  1 = Blue        9 = Light Blue <br>
 *  2 = Green       A = Light Green <br>
 *  3 = Aqua        B = Light Aqua <br>
 *  4 = Red         C = Light Red <br>
 *  5 = Purple      D = Light Purple <br>
 *  6 = Yellow      E = Light Yellow <br>
 *  7 = White       F = Bright White <br>
 *
 * @param color a value from 0 to 255 specifying the background and
 * the foreground color, where the the first 4 bits are the foreground
 * color and the last 4 are the background color.
 */

void setCursorColor(unsigned char color);

/*
 * Sets the console buffer size to the window size to eliminate scrollbars.
 *
 * @return true if completed succesfully, false otherwise.
 */

bool setBufferSize();

/*
 * Returns true if the word has a newline at the end. False otherwise.
 */

bool isNewlineWord(char* string);

/*
 * Simply prints a file, line by line.
 *
 * @param file the file to be printed.
 * @param status current printing status which can be:
 * CENTERED, LEFT or RIGHT aligned or JUSTIFIED.
 */

void printFile(FILE* file, short* status, short cols, short lines);

/*
 * Prints the text file page by page, a "page" is defined by the
 * size of the console.
 *
 * @param file the file to print.
 * @param cols the number of columns to be printed.
 * @param lines the number of lines to be printed.
 * @param pos the position of the last fully printed line.
 * @param currentWord the word last printed in the last line.
 * @param status current printing status which can be:
 * CENTERED, LEFT or RIGHT aligned or JUSTIFIED.
 *
 * @return the position of the last fully printed line.
 */
long printPage(FILE* file, short cols, short lines, long pos, int* currentWord, short* status);

#endif // DISPLAY_H_INCLUDED

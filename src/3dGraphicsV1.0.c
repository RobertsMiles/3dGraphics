#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

const int HORIZONTAL_COMPRESSION = 1;
const int VERTICAL_COMPRESSION = 2;
const int WIDTH_SCR = 90 / HORIZONTAL_COMPRESSION;
const int HEIGHT_SCR = 90 / VERTICAL_COMPRESSION;
const int WIDTH = 100;
const int HEIGHT = 100;
const int DEPTH = 100;

/**
 * Prints the screen, given the graphics array and the HEIGHT_SCR and WIDTH_SCR.
 */
void printScreen(int screen[HEIGHT_SCR][WIDTH_SCR]) {
	// print each point
	for (int y = 0; y < HEIGHT_SCR; y++) {
		for (int x = 0; x < WIDTH_SCR; x++) {
			attron(COLOR_PAIR(9));
			if (screen[y][x] == 1) {
				attron(COLOR_PAIR(1));
			}
			else if (screen[y][x] == 2) {
				attron(COLOR_PAIR(2));
			}
			else if (screen[y][x] == 2) {
				attron(COLOR_PAIR(2));
			}
			else if (screen[y][x] == 3) {
				attron(COLOR_PAIR(3));
			}
			else if (screen[y][x] == 4) {
				attron(COLOR_PAIR(4));
			}
			else if (screen[y][x] == 5) {
				attron(COLOR_PAIR(5));
			}
			else if (screen[y][x] == 6) {
				attron(COLOR_PAIR(6));
			}
			else if (screen[y][x] == 7) {
				attron(COLOR_PAIR(7));
			}
			mvprintw(y, x, "%d", screen[y][x]);
		}
	}
	refresh();
}

/**
 * Resets all point on the screen array to black
 */
void resetArray(int screen[HEIGHT_SCR][WIDTH_SCR]) {
	for (int y = 0; y < HEIGHT_SCR; y++) {
		for (int x = 0; x < WIDTH_SCR; x++) {
			screen[y][x] = 9;
		}
	}
}

/**
 * Converts a given 3D point to the proper screen coordinates
 * and writes the point to the screen array.
 */
void plotPointToScreen(int i, int j, int k, int screen[HEIGHT_SCR][WIDTH_SCR]) {
	// projects point to screen
	int x = atan2((double) k, (double) i) * 180 / M_PI;
	int y = atan2((double) j, (sqrt((double) pow(i, 2) + (double) pow(k, 2)))) * 180 / M_PI;

	// compress screen coordinate
	x /= HORIZONTAL_COMPRESSION;
	y /= VERTICAL_COMPRESSION;

	// checks if point is in-bounds with screen array
	if (x > -1 && y > -1 && x < WIDTH_SCR && y < HEIGHT_SCR) {

		// calculates distance from screen for heatmap
		double distance = sqrt(pow(i, 2) + pow(j, 2) + pow(k, 2));
		double maxDistance = sqrt(pow(WIDTH, 2) + pow(HEIGHT, 2) + pow(DEPTH, 2));
		int percentageAway = (int) (distance * 100 / maxDistance);

		// assigns appropriate heat map index to point on screen, preferencing the closest point
		if (percentageAway < 14 && screen[y][x] > 1) {
			screen[y][x] = 1;
		}
		else if (percentageAway < 28 && screen[y][x] > 2) {
			screen[y][x] = 2;
		}
		else if (percentageAway < 42 && screen[y][x] > 3) {
			screen[y][x] = 3;
		}
		else if (percentageAway < 56 && screen[y][x] > 4) {
			screen[y][x] = 4;
		}
		else if (percentageAway < 70 && screen[y][x] > 5) {
			screen[y][x] = 5;
		}
		else if (percentageAway < 84 && screen[y][x] > 6) {
			screen[y][x] = 6;
		}
		else if (screen[y][x] > 7) {
			screen[y][x] = 7;
		}
	}
}

// loop through all t, calculate each 3d point as a parametarized function - x(t); y(t); z(t)
// x coordinate on screen calculated as arctan(z / x), aka the angle in the x-z plane
// y coordinate on screen calculated as arctan(y / sqrt(x^2 + z^2)), aka the angle in the y plane

int main() {
	// start screen and check for color support
	initscr();
	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}

	// initialize variables
	int screen[HEIGHT_SCR][WIDTH_SCR];
	char inputChar;
	int iShift = 5;
	int jShift = 5;
	int kShift = 5;

	// stops user inputs from being printed in the terminal
	noecho();

	// setup color
	start_color();
	init_pair(9, COLOR_BLACK, COLOR_BLACK);
	init_pair(1, COLOR_RED, COLOR_RED);
	init_pair(2, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(3, COLOR_MAGENTA, COLOR_MAGENTA);
	init_pair(4, COLOR_BLUE, COLOR_BLUE);
	init_pair(5, COLOR_CYAN, COLOR_CYAN);
	init_pair(6, COLOR_GREEN, COLOR_GREEN);
	init_pair(7, COLOR_WHITE, COLOR_WHITE);

	// loop through each frame
	// while char not q, manipulate object in direction of char (read char after render)
	while (inputChar != 'q') {

		// reset graphic array
		resetArray(screen);

		// compute if each 3D location is or is not
		int radius = 1;
		for (int i = 0; i < WIDTH; i++) {
			for (int j = 0; j < HEIGHT; j++) {
				for (int k = 0; k < DEPTH; k++) {
					// if (k == (int) sqrt(-pow(i - iShift, 2) - pow(j - jShift, 2) + pow(radius, 2)) + kShift || k == (int) -sqrt(-pow(i - iShift, 2) - pow(j - jShift, 2) + pow(radius, 2)) + kShift) { // sphere
					// if (radius - pow(k - kShift, 4) == pow(i - iShift, 2) + pow(j - jShift, 2)) { // diamond
					// if (-k - kShift == pow(i - iShift, 2) - pow(j - jShift, 2)) { // saddle
					// if (radius == pow(i + iShift, 2) + pow(j + jShift, 2)) { // cylinder
					if (radius == pow(i - iShift, 2) + pow(j - jShift, 2) + pow(k - kShift, 2) + sin(4 * (i - iShift)) + sin(4 * (j - jShift)) + sin(4 * (k - kShift))) { // blob
						plotPointToScreen(i, j, k, screen);
					}
				}
			}
		}

		// print new frame
		printScreen(screen);

		// user input for translation or quit
		inputChar = getch();
		if (inputChar == 'q') {
			endwin();
			return 0;
		}
		else if (inputChar == 'i') {
			inputChar = getch();
			if (inputChar == '=') {
				iShift++;
			}
			else if (inputChar == '-') {
				iShift--;
			}
		}
		else if (inputChar == 'j') {
			inputChar = getch();
			if (inputChar == '=') {
				jShift++;
			}
			else if (inputChar == '-') {
				jShift--;
			}
		}
		else if (inputChar == 'k') {
			inputChar = getch();
			if (inputChar == '=') {
				kShift++;
			}
			else if (inputChar == '-') {
				kShift--;
			}
		}
	}
}

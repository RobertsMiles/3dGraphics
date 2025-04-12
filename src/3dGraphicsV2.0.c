#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

const int FOV = 90;
const int HORIZONTAL_COMPRESSION = 1;
const int VERTICAL_COMPRESSION = 2;
const int WIDTH_SCR = FOV / HORIZONTAL_COMPRESSION;
const int HEIGHT_SCR = FOV / VERTICAL_COMPRESSION;
const int WIDTH = 100;
const int HEIGHT = 100;
const int DEPTH = 100;

/**
 * The imaginary camera which has a position, direction, and field of view in space.
 * TODO remove FOV? or remove global constant FOV?
 */
struct Camera {
	int x;
	int y;
	int z;
	int xAngle; // most be between 0 and 360 (0 looks towards +x, 90 looks towards -z, 180 looks towards -x, etc)
	int yAngle; // must be between 0 and 180 (0 looks down, 180 looks up)
	int FOV;
};

/**
 * Prints the screen, given the screen array and the HEIGHT_SCR and WIDTH_SCR.
 */
void printScreen(int screen[HEIGHT_SCR][WIDTH_SCR]) {
	// print each point
	for (int j = 0; j < HEIGHT_SCR; j++) {
		for (int i = 0; i < WIDTH_SCR; i++) {
			attron(COLOR_PAIR(9));
			if (screen[j][i] == 1) {
				attron(COLOR_PAIR(1));
			}
			else if (screen[j][i] == 2) {
				attron(COLOR_PAIR(2));
			}
			else if (screen[j][i] == 2) {
				attron(COLOR_PAIR(2));
			}
			else if (screen[j][i] == 3) {
				attron(COLOR_PAIR(3));
			}
			else if (screen[j][i] == 4) {
				attron(COLOR_PAIR(4));
			}
			else if (screen[j][i] == 5) {
				attron(COLOR_PAIR(5));
			}
			else if (screen[j][i] == 6) {
				attron(COLOR_PAIR(6));
			}
			else if (screen[j][i] == 7) {
				attron(COLOR_PAIR(7));
			}
			mvprintw(j, i, "%d", screen[j][i]);
		}
	}

	// print image to screen
	refresh();
}

/**
 * Resets all point on the screen array to black
 */
void resetScreen(int screen[HEIGHT_SCR][WIDTH_SCR]) {
	for (int j = 0; j < HEIGHT_SCR; j++) {
		for (int i = 0; i < WIDTH_SCR; i++) {
			screen[j][i] = 9;
		}
	}
}

/**
 * Converts each point in space to the proper screen coordinate and writes that point to the screen array.
 */
void plotPointsToScreen(int screen[HEIGHT_SCR][WIDTH_SCR], int space[WIDTH][HEIGHT][DEPTH], struct Camera camera) {
	// if theta-xz brings x and z of test point (), continue
	//		if




	// TODO
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int z = 0; z < DEPTH; z++) {
				if (space[x][y][z] == 1) {
					//TODO debug
					printw("%s %d %d %d", "found point at:", x, y, z);
					refresh();

					for (int xAngle = camera.xAngle; xAngle < camera.xAngle + camera.FOV; xAngle++) {
						if (floor(atan2(z - camera.z, x - camera.x) * 180 / M_PI) <= xAngle && ceil(atan2(z - camera.z, x - camera.x) * 180 / M_PI) >= xAngle) {
							for (int yAngle = camera.yAngle; yAngle < camera.yAngle + camera.FOV; yAngle++) {
								if (floor(atan2(y - camera.y, sqrt(pow(x - camera.x, 2) + pow(z - camera.z, 2)))) <= yAngle && ceil(atan2(y - camera.y, sqrt(pow(x - camera.x, 2) + pow(z - camera.z, 2)))) >= yAngle) {

									// compress screen coordinate
									xAngle /= HORIZONTAL_COMPRESSION;
									yAngle /= VERTICAL_COMPRESSION;

									// calculates distance from screen for heatmap
									double distance = sqrt(pow(x - camera.x, 2) + pow(y - camera.y, 2) + pow(z - camera.z, 2));
									double maxDistance = sqrt(pow(WIDTH, 2) + pow(HEIGHT, 2) + pow(DEPTH, 2));
									int percentageAway = (int) (distance * 100 / maxDistance);

									// assigns appropriate heat map index to point on screen, preferencing the closest point
									if (percentageAway < 14 && screen[yAngle][xAngle] > 1) {
										screen[yAngle][xAngle] = 1;
									}
									else if (percentageAway < 28 && screen[yAngle][xAngle] > 2) {
										screen[yAngle][xAngle] = 2;
									}
									else if (percentageAway < 42 && screen[yAngle][xAngle] > 3) {
										screen[yAngle][xAngle] = 3;
									}
									else if (percentageAway < 56 && screen[yAngle][xAngle] > 4) {
										screen[yAngle][xAngle] = 4;
									}
									else if (percentageAway < 70 && screen[yAngle][xAngle] > 5) {
										screen[yAngle][xAngle] = 5;
									}
									else if (percentageAway < 84 && screen[yAngle][xAngle] > 6) {
										screen[yAngle][xAngle] = 6;
									}
									else if (screen[yAngle][xAngle] > 7) {
										screen[yAngle][xAngle] = 7;
									}

								}
							}
						}
					}




					// if truncate down <= num && truncate up >= num, true (floor, ceil)

					// if theta-xz brings x and z of test point (), continue
					//		if theta-y on hypotenuse of x and z bring y, graph
				}
			}
		}
	}





	/*
	// loop through each xAngle
	for (int xAngle = camera.xAngle - camera.FOV / 2; xAngle <= camera.xAngle + camera.FOV / 2; xAngle++) {
		// loop through each yAngle
		for (int yAngle = camera.yAngle - camera.FOV / 2; yAngle <= camera.yAngle + camera.FOV / 2; yAngle++) {
			// loop through each x
			for (int x = -WIDTH / 2; x <= WIDTH / 2; x++) {
				// loop through each y
				for () {
					// loop through each z
					for () {

					}
				}
			}
		}
	}
	*/
}

/**
 * Converts each point in space to the proper screen coordinate and writes that point to the screen array.
 */
void plotPointToScreen(int screen[HEIGHT_SCR][WIDTH_SCR], int x, int y, int z) {
	//TODO add scaling factor of space to screen for higher accuracy, such as 5 points projecting onto the same pixel on screen

	// projects point to screen
	int i = atan2((double) z, (double) x) * 180 / M_PI;
	int j = atan2((double) y, (sqrt((double) pow(x, 2) + (double) pow(z, 2)))) * 180 / M_PI;

	// compress screen coordinate
	i /= HORIZONTAL_COMPRESSION;
	j /= VERTICAL_COMPRESSION;

	// checks if point is in-bounds with screen array
	if (i > -1 && j > -1 && i < WIDTH_SCR && j < HEIGHT_SCR) {

		// calculates distance from screen for heatmap
		double distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
		double maxDistance = sqrt(pow(WIDTH, 2) + pow(HEIGHT, 2) + pow(DEPTH, 2));
		int percentageAway = (int) (distance * 100 / maxDistance);

		// assigns appropriate heat map index to point on screen, preferencing the closest point
		if (percentageAway < 14 && screen[j][i] > 1) {
			screen[j][i] = 1;
		}
		else if (percentageAway < 28 && screen[j][i] > 2) {
			screen[j][i] = 2;
		}
		else if (percentageAway < 42 && screen[j][i] > 3) {
			screen[j][i] = 3;
		}
		else if (percentageAway < 56 && screen[j][i] > 4) {
			screen[j][i] = 4;
		}
		else if (percentageAway < 70 && screen[j][i] > 5) {
			screen[j][i] = 5;
		}
		else if (percentageAway < 84 && screen[j][i] > 6) {
			screen[j][i] = 6;
		}
		else if (screen[j][i] > 7) {
			screen[j][i] = 7;
		}
	}
}

/**
 * Calculates and loads each point of the sphere into the space array.
 */
void calculateSphere(int x, int y, int z, int space[WIDTH][HEIGHT][DEPTH]) {
	int radius = 1;
	int xPos = x - WIDTH / 2;
	int yPos = y - HEIGHT / 2;
	int zPos = z - DEPTH / 2;
	space[x][y][z] = pow(radius, 2) == pow(xPos, 2) + pow(yPos, 2) + pow(zPos, 2);

	//TODO debug
	//printw("%s", "calculate sphere");
	//refresh();
}

int main() {
	// start screen and check for color support
	initscr();
	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal has no color support\n");
		exit(1);
	}

	// initialize variables
	int screen[HEIGHT_SCR][WIDTH_SCR];
	int space[WIDTH][HEIGHT][DEPTH];
	char inputChar; // TODO remove?
	struct Camera camera;
	camera.x = 0;
	camera.y = 5;
	camera.z = 5;
	camera.xAngle = 90;
	camera.yAngle = 45;
	camera.FOV = FOV;

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

	// calculates existence at each point and writes to space array
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			for (int z = 0; z < DEPTH; z++) {
				calculateSphere(x, y, z, space);
			}
		}
	}

	// loop through each frame or quit as specified through user input
	// TODO WASD controls position while arrow keys control direction
	while (1) {
		// render frame
		resetScreen(screen);
		plotPointsToScreen(screen, space, camera);
		printScreen(screen);

		// user input
		inputChar = getch();
		if (inputChar == 'q') {
			endwin();
			return 0;
		}
	}
}

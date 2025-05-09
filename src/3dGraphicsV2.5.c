// Miles Roberts
// April 2024

#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

const int FOV = 90;
const int HORIZONTAL_COMPRESSION = 1;
const int VERTICAL_COMPRESSION = 2;
const int WIDTH_SCR = FOV / HORIZONTAL_COMPRESSION;
const int HEIGHT_SCR = FOV / VERTICAL_COMPRESSION;
const int RADIUS_X = 35;
const int RADIUS_Y = 35;
const int RADIUS_Z = 35;

/**
 * The imaginary camera which has a position, direction, and field of view in space.
 */
struct Camera {
	int x;
	int y;
	int z;
};

/**
 * A point that belongs to an object in space. Contains end boolean to denote that no points exist beyond the last index.
 */
struct Point {
	int x;
	int y;
	int z;
	int end;
};

/**
 * Prints the screen, given the screen array and the HEIGHT_SCR and WIDTH_SCR.
 */
void printScreen(int screen[HEIGHT_SCR][WIDTH_SCR], struct Camera camera) {
	// print camera coordinate
	mvprintw(HEIGHT_SCR, WIDTH_SCR, "x: %d, y: %d, z: %d", camera.x, camera.y, camera.z);

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

	// print complete image to screen
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
void plotPointsToScreen(int screen[HEIGHT_SCR][WIDTH_SCR], struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)], struct Camera camera) {
	int end = 0;
	int index = 0;
	int x;
	int y;
	int z;
	double xAngle;
	double yAngle;
	int xScreen;
	int yScreen;
	int heightPercent;

	// loop through each point in space to plot
	for (int index = 0; space[index].end == 0; index++) {
		x = space[index].x - camera.x;
		y = space[index].y - camera.y;
		z = space[index].z - camera.z;

		// debug
		//printw("%d %d %d\n", x, y, z);

		// projects point to screen
		xAngle = atan2(z, x) * 180 / M_PI;
		yAngle = atan2((double) y, (sqrt((double) pow(x, 2) + (double) pow(z, 2)))) * 180 / M_PI;

		// debug
		//printw("%.4lf %.4lf\n", xAngle, yAngle);

		// round angles to nearest pixel
		xScreen = round(xAngle) - 45;
		yScreen = round(yAngle) + 45;

		// compress screen coordinate
		xAngle /= HORIZONTAL_COMPRESSION;
		yAngle /= VERTICAL_COMPRESSION;

		// debug
		//printw("%d %d\n", xScreen, yScreen);

		// checks if point is in-bounds with screen array
		if (xScreen > -1 && yScreen > -1 && xScreen < WIDTH_SCR && yScreen < HEIGHT_SCR) {

			// finds percentage of the total height covered by the y value
			heightPercent = round((space[index].y + RADIUS_Y) / (RADIUS_Y * 2.0) * 100);

			// assigns appropriate heat map index to point on screen, preferencing the closest point
			if (heightPercent < 14) {
				screen[yScreen][xScreen] = 1;
			}
			else if (heightPercent < 28) {
				screen[yScreen][xScreen] = 2;
			}
			else if (heightPercent < 42) {
				screen[yScreen][xScreen] = 3;
			}
			else if (heightPercent < 56) {
				screen[yScreen][xScreen] = 4;
			}
			else if (heightPercent < 70) {
				screen[yScreen][xScreen] = 5;
			}
			else if (heightPercent < 84) {
				screen[yScreen][xScreen] = 6;
			} else {
				screen[yScreen][xScreen] = 7;
			}
		}
	}



	// loop through each x direction to calculate line

	// look at each point that exists, if it is within field of view (within min/max xangle and min/max yangle) (range of possible x and possible y), continue
}

/**
 * Calculates if a sphere exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotSphere(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int radius = 35;
	if (pow(radius, 2) == pow(x, 2) + pow(y, 2) + pow(z, 2)) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Calculates if a cube exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotCube(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	if (abs(x) == RADIUS_X || abs(y) == RADIUS_Y || abs(z) == RADIUS_Z) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Calculates if a saddle exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotSaddle(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	if (-z == pow(x, 2) - pow(y, 2)) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Calculates if a blob exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotBlob(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int radius = 1;
	if (radius == pow(x, 2) + pow(y, 2) + pow(z, 2) + sin(4 * x) + sin(4 * y) + sin(4 * z)) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Calculates if a line-parabola hybrid exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotLineParabolaHybrid(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int radius = 1;
	if (z == pow(x, 2) + y) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Calculates if a sphere lattice exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotSphereLattice(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int radius = 1;
	if (2 == sin(x / 4.0) + sin(y / 4.0) + sin(z / 4.0)) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Calculates if a hyperboloid exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int plotHyperboloid(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int constant = 1;
	int constant2 = 1;
	if (constant2 == pow(x, 2) / pow(constant, 2) + pow(y, 2) / pow(constant, 2) + pow(z, 2) / pow(constant, 2)) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;
}

/**
 * Prints the coordinates of each point on the graph, for debugging.
 */
void printPoints(struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int rows = 50;
	erase();
	for (int index = 0; space[index].end == 0; index++) {
		mvprintw(index % rows, index / rows * 15, "%3d %3d %3d", space[index].x, space[index].y, space[index].z);
	}
}

int main() {
	// start screen and check for color support
	initscr();
	if (has_colors() == 0) {
		endwin();
		printf("Your terminal has no color support\n");
		return -1;
	}

	// initialize variables
	int debug = 0;
	char inputChar;
	char shapeChoice;
	int screen[HEIGHT_SCR][WIDTH_SCR];
	struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)];

	struct Camera camera;
	camera.x = 0;
	camera.y = 0;
	camera.z = -50;

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

	// user object choice
	printw("(1) sphere\n(2) cube\n(3) taco\n(4) saddle\n");
	shapeChoice = getch();

	// calculates chosen object's existence at each point and writes to space array
	int index = 0;
	for (int x = -RADIUS_X; x <= RADIUS_X; x++) {
		for (int y = -RADIUS_Y; y <= RADIUS_Y; y++) {
			for (int z = -RADIUS_Z; z <= RADIUS_Z; z++) {
				if (shapeChoice == '1') {
					index = plotSphere(x, y, z, index, space);
				}
				else if (shapeChoice == '2') {
					index = plotCube(x, y, z, index, space);
				}
				else if (shapeChoice == '3') {
					index = plotLineParabolaHybrid(x, y, z, index, space);
				}
				else if (shapeChoice == '4') {
					index = plotSaddle(x, y, z, index, space);
				}
			}
		}
	}
	space[index].end = 1;

	// loop through each frame or quit as specified through user input
	while (1) {
		if (debug) {
			printPoints(space);
		} else {
			// render frame
			resetScreen(screen);
			plotPointsToScreen(screen, space, camera);
			printScreen(screen, camera);
		}

		// user input
		inputChar = getch();

		// quit
		if (inputChar == 'q') {
			endwin();
			return 0;
		}

		// x axis
		else if (inputChar == 'a') {
			camera.x--;
		}
		else if (inputChar == 'd') {
			camera.x++;
		}

		// z axis
		else if (inputChar == 'w') {
			camera.y++;
		}
		else if (inputChar == 's') {
			camera.y--;
		}

		// y axis
		else if (inputChar == '\033') {
			getch();
			inputChar = getch();
			if (inputChar == 'A') {
				camera.z++;
			}
			else if (inputChar == 'B') {
				camera.z--;
			}
		}
	}
}

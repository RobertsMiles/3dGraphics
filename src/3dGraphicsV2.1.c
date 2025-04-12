#include <ncurses.h>
#include <stdlib.h>
#include <math.h>

const int FOV = 90;
const int HORIZONTAL_COMPRESSION = 1;
const int VERTICAL_COMPRESSION = 2;
const int WIDTH_SCR = FOV / HORIZONTAL_COMPRESSION;
const int HEIGHT_SCR = FOV / VERTICAL_COMPRESSION;
const int RADIUS_X = 10;
const int RADIUS_Y = 10;
const int RADIUS_Z = 10;
const int RENDER_DISTANCE = 50;

/**
 * The imaginary camera which has a position, direction, and field of view in space.
 * TODO remove FOV? or remove global constant FOV?
 */
struct Camera {
	int x;
	int y;
	int z;
	int xAngle; // most be between 0 and 360 (0 looks towards +x, 90 looks towards -z, 180 looks towards -x, etc)
	// int yAngle; // must be between 0 and 180 (0 looks down, 180 looks up)
	int FOV;
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

	// print camera coordinate
	//mvprintw(HEIGHT_SCR, 0, "x: %d, y: %d, z: %d", camera.x, camera.y, camera.z);

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
	double distance;
	int percentageAway;

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

		// compress screen coordinate
		xAngle /= HORIZONTAL_COMPRESSION;
		yAngle /= VERTICAL_COMPRESSION;

		// round angles to nearest pixel
		xScreen = round(xAngle);
		yScreen = round(yAngle);

		// debug
		//printw("%d %d\n", xScreen, yScreen);

		// checks if point is in-bounds with screen array
		if (xScreen > -1 && yScreen > -1 && xScreen < WIDTH_SCR && yScreen < HEIGHT_SCR) {

			// calculates distance from screen for heatmap
			distance = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
			percentageAway = round((distance / RENDER_DISTANCE * 100));

			// assigns appropriate heat map index to point on screen, preferencing the closest point
			if (percentageAway < 14 && screen[yScreen][xScreen] > 1) {
				screen[yScreen][xScreen] = 1;
			}
			else if (percentageAway < 28 && screen[yScreen][xScreen] > 2) {
				screen[yScreen][xScreen] = 2;
			}
			else if (percentageAway < 42 && screen[yScreen][xScreen] > 3) {
				screen[yScreen][xScreen] = 3;
			}
			else if (percentageAway < 56 && screen[yScreen][xScreen] > 4) {
				screen[yScreen][xScreen] = 4;
			}
			else if (percentageAway < 70 && screen[yScreen][xScreen] > 5) {
				screen[yScreen][xScreen] = 5;
			}
			else if (percentageAway < 84 && screen[yScreen][xScreen] > 6) {
				screen[yScreen][xScreen] = 6;
			}
			else if (screen[yScreen][xScreen] > 7) {
				screen[yScreen][xScreen] = 7;
			}
		}
	}



	// loop through each x direction to calculate line

	// look at each point that exists, if it is within field of view (within min/max xangle and min/max yangle) (range of possible x and possible y), continue
}

/**
 * Calculates if the sphere exists at the given point, and if so, adds it to the space array.
 * Returns current index.
 */
int calculateSphere(int x, int y, int z, int indexCount, struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)]) {
	int radius = 5;
	if (pow(radius, 2) == pow(x, 2) + pow(y, 2) + pow(z, 2)) {
		space[indexCount].x = x;
		space[indexCount].y = y;
		space[indexCount].z = z;
		indexCount++;
	}
	return indexCount;

	//TODO debug
	//printw("%s", "calculate sphere");
	//refresh();
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
	char inputChar;
	int screen[HEIGHT_SCR][WIDTH_SCR];
	struct Point space[(RADIUS_X * 2 + 1) * (RADIUS_Y * 2 + 1) * (RADIUS_Z * 2 + 1)];

	struct Camera camera;
	camera.x = -10;
	camera.y = -10;
	camera.z = -10;
	camera.xAngle = 90;
	// camera.yAngle = 45;
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
	int index = 0;
	for (int x = -RADIUS_X; x <= RADIUS_X; x++) {
		for (int y = -RADIUS_Y; y <= RADIUS_Y; y++) {
			for (int z = -RADIUS_Z; z <= RADIUS_Z; z++) {
				index = calculateSphere(x, y, z, index, space);
			}
		}
	}
	space[index].end = 1;

	// loop through each frame or quit as specified through user input
	// TODO WASD controls position while arrow keys control direction
	while (1) {
		// render frame
		resetScreen(screen);
		//erase();
		plotPointsToScreen(screen, space, camera);
		printScreen(screen, camera);

		// user input
		inputChar = getch();
		if (inputChar == 'q') {
			endwin();
			return 0;
		}
		else if (inputChar == 'i') {
			inputChar = getch();
			if (inputChar == '=') {
				camera.x++;
			}
			else if (inputChar == '-') {
				camera.x--;
			}
		}
		else if (inputChar == 'j') {
			inputChar = getch();
			if (inputChar == '=') {
				camera.y++;
			}
			else if (inputChar == '-') {
				camera.y--;
			}
		}
		else if (inputChar == 'k') {
			inputChar = getch();
			if (inputChar == '=') {
				camera.z++;
			}
			else if (inputChar == '-') {
				camera.z--;
			}
		}
	}
}

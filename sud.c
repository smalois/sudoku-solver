#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 9
#define COLS 9

int **array;

void allocate_array() {
	int i;
	array = (int**) malloc(ROWS * sizeof(int*));
	for(i = 0; i < ROWS;i++) {
		array[i] = (int*) malloc(COLS * sizeof(int));
	}
}

void fill_board(FILE* infile) {
	int i, j;
	char line[128], *num;
	for(i = 0; i < ROWS; i++) {
		fgets(line, sizeof line, infile);
		num = strtok(line, " \n");
		for(j = 0; j < COLS; j++) {
			array[i][j] = atoi(num);
			num = strtok(NULL, " \n");
		}
	}
}

void print_board() {
	int i, j;
	for(i = 0; i < ROWS; i++) {
		for(j = 0; j < COLS; j++) {
			printf("%d ", array[i][j]);
		}			
		printf("\n");
	}
}

int check_assignability(int digit, int r, int c) {
	int i = 0;
	int j = 0;
	/* Check row */
	for (i = 0; i < COLS; i++) {
		if (array[r][i] == digit) 
			return 0;
	}
	/* Check column */
	for (i = 0; i < ROWS; i++) {
		if (array[i][c] == digit) 
			return 0;
	}
	/* Check local box */
	for (j = 0; j < 3; j++) {
		for (i = 0; i < 3; i++) {
			if (array[i + 3 * (r/3)][j + 3 * (c/3)] == digit)
				return 0;
		}
	}	
	return 1;
}

int next_empty() {
	int r = 0;
	int c = 0;
	for (r = 0; r < ROWS; r++) {
		for (c = 0; c < COLS; c++) {
			if (array[r][c] == 0) {
				return ((10 * r) + c);
			}
		}
	}
	return 100;
}

int solve() {
	int i;

	int num = next_empty();
	int row = num / 10;
	int column = num % 10;
	
	/* Base case */
	if(num == 100) {
		printf("Complete\n");
		return 1;
	}

	for (i = 1; i <= 9; i++) {
		/* If value can be stored in box */
		if (check_assignability(i, row, column)) {
			/* Change the value */
			array[row][column] = i;
			if(solve()) {
				return 1;
			}
			array[row][column] = 0;
		} 
	}
	return 0;
}

int main(int argc, char *argv[]) {
	char instring[16];
	int n;
	FILE* infile;
	if (argc != 2) {
		exit(0);
	}
	infile = fopen(argv[1], "r");	
	allocate_array();
	fill_board(infile);
	if(!solve()) {
		printf("No Solution\n");
		exit(0);
	}
	print_board();
}

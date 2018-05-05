#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/* Define Constants */
#define MAXNAME 81
#define MAXELEM 10000
#define MAXINPUT 1000

#define lessLine(A,B) (A.line < B.line || (A.line == B.line && A.column < B.column))
#define lessColumn(A,B) (A.column < B.column || (A.column == B.column && A.line < B.line))

/* Define Struct */
typedef struct {
	unsigned long line, column;
	double value;
} Element;

typedef struct {
	Element elements[MAXELEM];
	double zero;
	int nElem;
	unsigned long min[2], max[2];
} Matrix;

/* Global Variables */
Matrix matrix;

/* ---------- Complementary Functions ---------- */

/**	Function: update_matrix_info
 *	Updates the information (characteristics) of the matrix (min, max) 
 */
void update_matrix_info() {
	int i;

	/* If matrix not empty */
	if (matrix.nElem > 0) {
		/* Inicialize charactheristics in order to compare after */
		matrix.min[0] = matrix.elements[0].line;
		matrix.min[1] = matrix.elements[0].column;
		matrix.max[0] = matrix.elements[0].line;
		matrix.max[1] = matrix.elements[0].column;

		/* Goes through all the elements stored in the Matrix in order to find the charactheristics */
		for (i = 0; i < matrix.nElem; i++) {
			if (matrix.elements[i].line < matrix.min[0]) matrix.min[0] = matrix.elements[i].line;
			if (matrix.elements[i].column < matrix.min[1]) matrix.min[1] = matrix.elements[i].column;
			if (matrix.elements[i].line > matrix.max[0]) matrix.max[0] = matrix.elements[i].line;
			if (matrix.elements[i].column > matrix.max[1]) matrix.max[1] = matrix.elements[i].column;
		}
	}

	/* In case matrix is empty (safeguard from bugs) */
	else {
		matrix.min[0] = 0;
		matrix.min[1] = 0;
		matrix.max[0] = 0;
		matrix.max[1] = 0;
	}
}

/* ---------- Asked Functions ---------- */

/**	Function: add_element
 *	@param line (int)
 *	@param column (int)
 *	@param value (double)
 *	Adds an element to matrix, subtitutes a value or removes element depending 
 *	on value and if already defined in matrix
 */
void add_element(unsigned long line, unsigned long column, double value) {
	int i, exchange = 0, substitute = 0;

	/* Verifies if position already exists in matrix */
	for (i = 0; (i < matrix.nElem) && (!substitute); i++) {
		/* In case an element is found in matrix with the same position as the one given: */
		if (!exchange && matrix.elements[i].line == line && matrix.elements[i].column == column) {
			/* If value = zero of the matrix exchange = 1, wich means it will be removed from matrix */
			if (value == matrix.zero) exchange ++;
			/* If value != zero of matrix the value of the defined position is substituted by the new one */
			else {
				matrix.elements[i].value = value;
				substitute ++;
			}
		}

		/* Copy every element in front of it to the position (remove element path) */
		else matrix.elements[i-exchange] = matrix.elements[i];
	}
	
	/* Decrease the variables that stores the number of elements */
	if (exchange == 1) matrix.nElem --;

	/* Add an element that doesn't already exist in the matrix and with the value != zero of matrix */
	if ((!substitute) && value != matrix.zero) {
		matrix.elements[matrix.nElem].line = line;
		matrix.elements[matrix.nElem].column = column;
		matrix.elements[matrix.nElem].value = value;
		matrix.nElem ++;
	}
}

/**	Function: print_matrix
 *	Prints every elemt stored in the matrix
 */
void print_matrix() {
	int i;
	/* if matrix doesn't have any element stored in it */
	if (matrix.nElem == 0) printf("empty matrix\n");
	/* else print each element as asked */
	else for (i = 0; i < matrix.nElem; i++)
			printf("[%lu;%lu]=%.3f\n", 
				matrix.elements[i].line, 
				matrix.elements[i].column, 
				matrix.elements[i].value);
}

/**	Function: matrix_info
 *	Prints information such as minimum and maximum, both line and column, number of elements,
 *	size and the density of the matrix
 */
void matrix_info() {
	int size;
	float dens;

	/* In case matrix is empty simply print "empty matrix" */
	if (matrix.nElem == 0) printf("empty matrix\n");
	/* If there are elements stored in the matrix */
	else {
		size = (matrix.max[0] - matrix.min[0] + 1) * (matrix.max[1] - matrix.min[1] + 1);
		dens = matrix.nElem * 100.0 / size;

		/* Effectively print the information about the matrix */
		printf("[%lu %lu] [%lu %lu] %d / %d = %.3f%%\n", 
			matrix.min[0], 
			matrix.min[1], 
			matrix.max[0], 
			matrix.max[1], 
			matrix.nElem, 
			size, 
			dens);
	}
}

/**	Function: list_line
 *	@param line (int)
 *	Lists an entire line, from the minimum to the maximum column of the matrix
 */
void list_line(unsigned long line) {
	int i, empty = 1;
	double valuesLine[matrix.max[1] - matrix.min[1] + 1];

	/* Starts every element of the array where values will be stored with the zero of matrix */
	for (i = 0; i <= matrix.max[1] - matrix.min[1]; i++) valuesLine[i] = matrix.zero;

	/* Goes through every element stored in the matrix */
	for (i = 0; i < matrix.nElem; i++) {
		/* If the element line from the matrix matches the one asked: */
		if (matrix.elements[i].line == line) {
			/* Stores it in the array value in the correct position according to the column */
			valuesLine[matrix.elements[i].column - matrix.min[1]] = matrix.elements[i].value;
			empty = 0;
		}
	}

	/* If at least exists one element from that line print the all line accordingly */
	if (empty == 0) for (i = 0; i <= matrix.max[1] - matrix.min[1]; i++) 
		printf(" %.3f", valuesLine[i]);

	/* If there isn't any element in the line given as parameter print "empty line" */
	else printf("empty line");
	printf("\n");
}

/**	Function: list_column
 *	@param column (int)
 *	Lists an entire line, from the minimum to the maximum column of the matrix
 */
void list_column(unsigned long column) {
	int i, empty = 1;
	double valuesColumn[matrix.max[0] + 1];

	/* Starts every element of the array where values will be stored with the zero of matrix */
	for (i = 0; i <= matrix.max[0] - matrix.min[0]; i++) valuesColumn[i] = matrix.zero;

	/* Goes through every element stored in the matrix */
	for (i = 0; i < matrix.nElem; i++) {
		/* If the element column from the matrix matches the one asked: */
		if (matrix.elements[i].column == column) {
			/* Stores it in the array value in the correct position according to the line */
			valuesColumn[matrix.elements[i].line - matrix.min[0]] = matrix.elements[i].value;
			empty = 0;
		}
	}

	/* If at least exists one element from that column print the all line accordingly */
	if (empty == 0) {
		for (i = 0; i <= matrix.max[0] - matrix.min[0]; i++) 
			printf("[%lu;%lu]=%.3f\n", i + matrix.min[0], column, valuesColumn[i]);
	}

	/* If there isn't any element in the column given as parameter print "empty column" */
	else printf("empty column\n");
}

/**	Function: sort_matrix
 *	@param mode (int)
 *	Sorts the elements stored in the matrix accordingly using insertion sort
 */
void sort_matrix(Element a[], int l, int r, int mode) {
	int i,j;
	
	for (i = l+1; i <= r; i++) {
		Element v = a[i];
		j = i-1;
		while ( j >= l && ((mode == 0 && lessLine(v, a[j])) || (mode == 1 && lessColumn(v, a[j])))) {
			a[j+1] = a[j];
			j--;
		}
		a[j+1] = v;
	}
}

/**	Function: new_zero
 *	@param newZero (double)
 *	Redefines the zero of the matrix (element not stored)
 */
void new_zero(double newZero) {
	int i, j = 0, contador = 0;

	/* Store the new zero of the matrix in the struct */
	matrix.zero = newZero;
	/* Removes every element already stored in the matrix with the same value as the new zero */
	for (i = 0; i < matrix.nElem; i++) {
		if (matrix.elements[i].value != newZero) {
			matrix.elements[j] = matrix.elements[i];
			j ++;
		}
		else contador ++;
	}

	/* Substracts to the #of elements stored in the matrix the #of elements that were removed */
	matrix.nElem -= contador;
}

/**	Function: input_matrix
 *	@param filename (char * [MAXNAME])
 *	Stores the elements saved in a file from a previous matrix in the one being refered at the moment
 */
void input_matrix(char *filename) {
	char input[MAXINPUT];
	int a, b;
	double c;
	FILE *file;

	/* Open File */
	file = fopen(filename, "r");

	/* Get line from file until the end */
	while ((fgets(input, MAXINPUT, file)) != NULL) {
		/*Segment the line taken from file into the different parameters */
		sscanf(input, "%d %d %lf", &a, &b, &c);
		/* Add element to the actual matrix according to the parameters */
		add_element(a, b, c);
	}

	/* Close file */
	fclose(file);
}

/**	Function: save_matrix
 *	@param filename (char * [MAXNAME])
 *	Saves the actual matrix elements into a file
 */
void save_matrix(char *filename) {
	FILE *file;
	int i;

	/* Open File */
	file = fopen(filename, "w");
	/* Goes through every element stored in the matrix: */
	for (i = 0; i < matrix.nElem; i++) {
		/* Each element corresponds to a line that contains all of its characteristics */
		fprintf(file, "%lu %lu %lf\n", matrix.elements[i].line, matrix.elements[i].column, matrix.elements[i].value);
	}
	/* Close file */
	fclose(file);
}

/**	Function: compress
 *	Compresses the matrix accordingly
 */
void compress() {
	const unsigned long TAMANHO = 2 * MAXELEM,
		cons_line = (matrix.max[0]) - (matrix.min[0]) + 1,
		cons_column = (matrix.max[1]) - (matrix.min[1]) + 1;
	int i, j, k, nLines = 0, done, print_max = 0;
	int index[TAMANHO], offset[cons_line], contador[cons_line], line_indexes[cons_column];
	double values[TAMANHO];
	unsigned long maxLine, line_offset;

	/* In case matrix is too dense to be compressed */
	if (matrix.nElem / (matrix.max[0] * matrix.max[1]) > 0.5) printf("dense matrix\n");

	else {
		/* Inicialize array where #lines will be stored as 0 */
		for (i = 0; i < cons_line; i++) contador[i] = 0;
		/* Inicialize final arrays where the information asked will be stored */
		for (i = 0; i < TAMANHO; i++) {
			index[i] = 0;
			offset[i] = 0;
			values[i] = matrix.zero;
		}

		/* Count number of elements in each line, being line = index + minimum line */
		for (i = 0; i < matrix.nElem; i++) {
			/* In case we foud a new line increment nLines in order to know how many different lines there are */
			if (contador[matrix.elements[i].line - matrix.min[0]] == 0) nLines ++;
			/* Increment the array that stores the #of elements of each line */
			contador[matrix.elements[i].line - matrix.min[0]]++;
		}


		for (j = 0; j < nLines; j++) {
			maxLine = 0;

			/* Find the line that as more elements */
			for (i = 0; i < cons_line; i++) if (contador[i] > contador[maxLine]) maxLine = i;

			k = 0;
			/* Store the indexes of elements of maxLine in the matrix */
			for (i = 0; i < matrix.nElem; i++) {
				if (matrix.elements[i].line == (maxLine + matrix.min[0])) {
					line_indexes[k] = i;
					k ++;
				}
			}
			
			line_offset = -1;
			/* Determine the minimum offset that will allow to write all elements of maxLine */
			do {
				line_offset ++;
				done = 1;
				for (i = 0; i < contador[maxLine]; i++)
					if (values[(matrix.elements[line_indexes[i]].column - matrix.min[1]) + line_offset] != matrix.zero)
						done = 0;
			} while (!done);

			/* print_max is the offset of the final print, so that compression is righ and uses padding */
			if (cons_column + line_offset > print_max) print_max = cons_column + line_offset;
			/* Place the offset of the line on the array */
			offset[maxLine] = line_offset;
			/* Place values on max_line on array index and values with right offset */
			for (i = 0; i < contador[maxLine]; i++) {
				index[(matrix.elements[line_indexes[i]].column - matrix.min[1]) + line_offset] = maxLine + matrix.min[0];
				values[(matrix.elements[line_indexes[i]].column - matrix.min[1]) + line_offset] = matrix.elements[line_indexes[i]].value;
			}

			/* Necessary in order to next iteration of loop get the next line with max elements */
			contador[maxLine] = 0;
		}

		/* Print the arrays asked for */
		printf("value =");
		for (i = 0; i < print_max; i++) printf(" %.3lf", values[i]);
		printf("\n");
		printf("index =");
		for (i = 0; i < print_max; i++) printf(" %d", index[i]);
		printf("\n");
		printf("offset =");
		for (i = 0; i < cons_line; i++) printf(" %d", offset[i]);
		printf("\n");
	}
}

/* Main */
int main(int argc, char **argv) {
	char input[MAXINPUT], saveName[MAXNAME];
	/* Define Variables to store input */
	unsigned long a, b;
	double c;
	char d[MAXNAME];

	/* Inicialize Struct Matrix Variables */
	matrix.nElem = 0;
	matrix.zero = 0;

	/* In case was given a previously created matrix as input in a file */
	if (argc == 2) {
		strcpy(saveName, argv[1]);
		input_matrix(argv[1]);
	}

	do {
		/* "Reset" input so that it wont misfire to a previoulsy used action */
		input[0] = ' ';
		/* Get input from user that depending on its firt characther will be divided into parameters */
		fgets(input, MAXINPUT, stdin);
		switch(input[0]) {
			case ('a'):
				sscanf(input, "a %lu %lu %lf", &a, &b, &c);
				add_element(a, b, c);
				break;
			case ('p'):
				print_matrix();
				break;
			case ('i'):
				update_matrix_info(matrix);
				matrix_info();
				break;
			case ('l'):
				update_matrix_info(matrix);
				sscanf(input, "l %lu", &a);
				list_line(a);
				break;
			case ('c'):
				update_matrix_info(matrix);
				sscanf(input, "c %lu", &a);
				list_column(a);
				break;
			case ('o'):
				sscanf(input, "o %s\n", d);
				if (!strcmp(d, "column\0")) sort_matrix(matrix.elements, 0, matrix.nElem-1, 1);
				else sort_matrix(matrix.elements, 0, matrix.nElem-1, 0);
				d[0] = 'n';
				break;
			case ('z'):
				sscanf(input, "z %lf", &c);
				new_zero(c);
				break;
			case ('w'):
				if (!strcmp(input, "w\0")) save_matrix(saveName);
				else {
					sscanf(input, "w %s", saveName);
					save_matrix(saveName);
				}
				break;
			case ('s'):
				update_matrix_info(matrix);
				compress();
				break;
		}

	/* If input = q then exit loop and consequently program */
	} while (input[0] != 'q');
	return 0;
}
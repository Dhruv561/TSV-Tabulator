/* Program to generate hierarchical reports from TSV structured data

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2024, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.
   All included code is (c) Copyright University of Melbourne, 2024

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: Dhruv Verma
   Dated:     04/09/2024

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <assert.h>

#define CHAR_CR '\r'	        /* CR character, in PC-format text files */
#define CHAR_NL '\n'         /* newline character, ends each input line */
#define CHAR_TB '\t'         /* tab character, ends each TSV field */

#define STATUS_EOL 1         /* return value for end of line field */
#define STATUS_EOF 2         /* return value for end of file field */
#define STATUS_NML 3         /* return value if neither of previous two */

#define MAXFIELDLEN 50       /* maximum field length */
#define MAXROWS 1000         /* maximum number of rows */
#define MAXCOLUMNS 30        /* maximum number of columns */
#define DASH 1               /* input value for printing dashes */
#define SPACE 0              /* input value for printing spaces */
#define SAME -1              /* return value if two rows are same */

/* one tsv field, stored within a fixed-length character string */
typedef char field_t[MAXFIELDLEN+1];

/**************************************************************/

int  mygetchar(void);
int  getfield(field_t dest);
void ta_daa(void);
void stage1(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns);
void stage2(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
            int sort_idx, char *sort_keys[]);
void stage3(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
            int sort_idx, char *sort_keys[]);
void create_array(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns);
void print_array(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns);
void print_row(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns);
void array_sort(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
                int sort_idx, char *sort_keys[]);
void swap_rows(field_t data[MAXROWS][MAXCOLUMNS], int row1, int row2, 
               int row_len);
void create_report(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
                   int sort_idx, char *sort_keys[], int max_len);
void max_strlen(field_t data[MAXROWS][MAXCOLUMNS], int row, int column, 
                int *max_len);
void print_space_dash(int length, int type);
int compare_rows(field_t data[MAXROWS][MAXCOLUMNS], int row1, int row2, 
                 int sort_idx, char *sort_keys[]);
int print_header(field_t data[MAXROWS][MAXCOLUMNS], int sort_idx, 
                 char *sort_keys[], int max_len);

/**************************************************************/

/* main program provides traffic control */
int main(int argc, char *argv[]) {

   field_t data[MAXROWS][MAXCOLUMNS];
   int rows, columns;

   stage1(data, &rows, &columns);
   if (argc > 1) {
      stage2(data, rows, columns, argc - 1, &argv[1]);
      stage3(data, rows, columns, argc - 1, &argv[1]);
   }
	/* all done, time to go home */
	ta_daa();
	return 0;
}

/**************************************************************/

/* read characters and build a string, stopping when a tab or newline
   as encountered, with the return value indicating what that
   terminating character was */
int getfield(field_t dest) {

	int ch, nchs=0;
	int status=STATUS_NML;

	dest[0] = '\0';
	while ((ch=mygetchar())
		&& (ch != CHAR_TB) && (ch != CHAR_NL) && (ch != EOF)) {

		if (nchs < MAXFIELDLEN) {
			/* ok to save this character */
			dest[nchs++] = ch;
			dest[nchs] = '\0';
		} else {
			/* silently discard extra characters if present */
		}
	}

	/* return status is defined by what character stopped the loop */
	if (ch == EOF) {
		status = STATUS_EOF;
	} else if (ch == CHAR_NL) {
		status = STATUS_EOL;
	} else if (ch == CHAR_TB) {
		status = STATUS_NML;
	}
	return status;
}

/**************************************************************/

/* Read a single character, bypassing any CR characters encountered,
   so as to work correctly with either PC-type or Unix-type input */
int mygetchar(void) {
	int c;
	while ((c=getchar()) == CHAR_CR) {
		/* empty loop body */
	}
	return c;
}

/**************************************************************/

/* Traffic control for stage 1, creating data structure and printing last row 
   in dataset*/
void stage1(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns) {
   printf("Stage 1\n");
   create_array(data, rows, columns);
   printf("row %d is:\n", (*rows) - 1);
   print_row(data, rows, columns);
   printf("\n");
}

/**************************************************************/

/* Traffic control for stage2, sorts array and prints out first, middle and last
   rows in sorted array */
void stage2(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
            int sort_idx, char *sort_keys[]) {
   array_sort(data, rows, columns, sort_idx, sort_keys);

   printf("Stage 2\n");

   // Print sorting column values for header 
   printf("sorting by \"%s\"", data[0][atoi(sort_keys[0]) - 1]);
   // Add comma if last sorting element
   if (sort_idx > 1) {
         printf(",\n");
      } else {
         printf("\n");
      }

   for (int i = 1; i < sort_idx; i++) {
      printf("   then by \"%s\"", data[0][atoi(sort_keys[i]) - 1]);
      // Add comma if last sorting element
      if (i != sort_idx - 1) {
         printf(",\n");
      } else {
         printf("\n");
      }
   }

   // Print first, middle and last row of data
   int row1 = 2;
   int row_mid = rows / 2 + 1;
   printf("row 1 is:\n");
   print_row(data, &row1, &columns);
   printf("row %d is:\n", rows / 2);
   print_row(data, &row_mid, &columns);
   printf("row %d is:\n", rows - 1);
   print_row(data, &rows, &columns);
   printf("\n");
}

/**************************************************************/

/* Traffic control for stage 3. Finds maximum length of item in last row to 
   identify spacing and dashes and prints the dataset as a report*/
void stage3(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
            int sort_idx, char *sort_keys[]) {
   printf("Stage 3\n");

   // Calculate the maximum word length in last sorting index column
   int max_len = 0;
   for (int i = 1; i < rows; i++) {
      int len = strlen(data[i][atoi(sort_keys[sort_idx - 1]) - 1]);
      if (len > max_len) {
          max_len = len;
      }
   }
   // Prints header row and returns integer dash length
   int dash_len = print_header(data, sort_idx, sort_keys, max_len); 

   // Print the data in specific columns required for report
   create_report(data, rows, columns, sort_idx, sort_keys, max_len);
   print_space_dash(dash_len, DASH);
   printf("\n");
}

/**************************************************************/

/* Prints ta daa */
void ta_daa(void) {
   printf("ta daa!\n");
}

/**************************************************************/

/* Creates an array of arrays with each element being a string from the 
   tsv data, keeping count of the number of rows and columns in array */
void create_array(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns) {
   int file_end = 0;

   for (*rows = 0; *rows < MAXROWS; (*rows)++) {
      for (int cols = 0; cols < MAXCOLUMNS; cols++) {
         /* Identifies if value is string, column or new line at specific row
            and column and increment columns accordingly */
         int status = getfield(data[*rows][cols]);
         if (status == STATUS_NML) {
            // Pass quietly
         } else if (status == STATUS_EOL) {
            *columns = cols + 1;
            break;
         } else {
            file_end = 1;
            break;
         }
      }

      // Reached end of file so stop row count
      if (file_end == 1) {
         break;
      }
   }
   printf("input tsv data has %d rows and %d columns\n", (*rows) - 1, *columns);
}

/**************************************************************/

/* Prints entire array */
void print_array(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns) {
   for (int i = 0; i < *rows; i++) {
      for (int j = 0; j < *columns; j++) {
         printf("%s ", data[i][j]);
      }
      printf("\n");
   }
}

/**************************************************************/

/* Prints specific row in array */
void print_row(field_t data[MAXROWS][MAXCOLUMNS], int *rows, int *columns) {
   for (int i = 0; i < *columns; i++) {
      printf("%4d: %-10s %s\n", i + 1, data[0][i], data[(*rows) - 1][i]);
   }
}

/**************************************************************/

/* Swaps two rows of the data structure by creating a temporary array and 
   swapping each individual column one at a time */
void swap_rows(field_t data[MAXROWS][MAXCOLUMNS], int row1, int row2, 
               int row_len) {
   field_t temp_array;
   for (int i = 0; i < row_len; i++) {
      strcpy(temp_array, data[row1][i]);
      strcpy(data[row1][i], data[row2][i]);
      strcpy(data[row2][i], temp_array);
   }
}

/**************************************************************/

/* Sorts an array of arrays using insertion sort based on the column indexes 
   given by user. If there is a tie, the array is sorted by the next column 
   index until the array is sorted */
void array_sort(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
                int sort_idx, char *sort_keys[]) {
   // Use insertion sort to cycle through rows
   for (int row1 = 2; row1 < rows; row1++) {
      for (int row2 = row1 - 1; row2 >= 1; row2--) {
         int swap = 0;

         // Compare if column values between both rows
         for (int i = 0; i < sort_idx; i++) {
            int column = atoi(sort_keys[i]) - 1;
            int compare_val = strcmp(data[row2 + 1][column], 
                                     data[row2][column]);

            if (compare_val < 0) {
               // First value is in wrong position and needs to be swapped
               swap = 1;
               break;
            }
            if (compare_val > 0) {
               // First value in correct position
               break;
            }
         }

         // Swap rows if needed
         if (swap) {
            swap_rows(data, row2 + 1, row2, columns);
         }
      }
   }
}

/**************************************************************/

/* Finds the length of a string and assigns it to max_len if it's the largest */
void max_strlen(field_t data[MAXROWS][MAXCOLUMNS], int row, int column, 
                int *max_len) {
   int len = strlen(data[row][column]);
   if (len > *max_len) {
      *max_len = len;
   }
}

/**************************************************************/

/* Prints header row for stage 3, returning integer dash_len which is the number
   of dashes that need to be printed */
int print_header(field_t data[MAXROWS][MAXCOLUMNS], int sort_idx, 
                  char *sort_keys[], int max_len) {
   // Calculates and prints required dashes
   int dash_len = ((sort_idx - 1) * 4) + max_len + 6;
   print_space_dash(dash_len, DASH);
   printf("\n");

   /* Cycle through sorting column indexes given by user input and print value
      for header rows only */
   for (int i = 0; i < sort_idx; i++) {
      int space_len = i * 4;
      print_space_dash(space_len, SPACE);
      printf("%s", data[0][atoi(sort_keys[i]) - 1]);
      if (i == sort_idx - 1) {
         // If last print spaces to align 'Count' to end of dashes
         print_space_dash(dash_len - strlen(data[0][atoi(sort_keys[i]) - 1]) 
                             - space_len - 6, SPACE);
      } else {
         printf("\n");
      }
   }
   printf(" Count\n");
   print_space_dash(dash_len, DASH);
   printf("\n");

   return dash_len;
}

/**************************************************************/

/* Prints either spaces or dashes based on desired length */
void print_space_dash(int length, int type) {
   for (int i = 0; i < length; i++) {
      if (type == 0) {
         printf(" ");
      }
      else {
         printf("-");
      }
   }
}

/**************************************************************/

/* Cycles through every column index from user input to check if two rows are 
   same. If same, integer SAME is returned otherwise index at which values in 
   rows are different is returned */
int compare_rows(field_t data[MAXROWS][MAXCOLUMNS], int row1, int row2, 
                 int sort_idx, char *sort_keys[]) {
   for (int i = 0; i < sort_idx; i++) {
      int column = atoi(sort_keys[i]) - 1;
      if (strcmp(data[row1][column], data[row2][column]) != 0) {
         // Return index at which column values differ
         return i;
      }
   }
   // No column value in either rows are different
   return SAME; 
}

/**************************************************************/

/* Creates and prints report for stage 3 based on order of input column indexes.
   Compares if previous row is same as current row. If same, medal count is 
   incremented, otherwise, all column values from the break index onwards and
   medal count are printed */
void create_report(field_t data[MAXROWS][MAXCOLUMNS], int rows, int columns, 
                  int sort_idx, char *sort_keys[], int max_len) {
   int count = 1;
   int break_idx;

   // Print out first row of data
   for (int i = 0; i < sort_idx; i++) {
      int column = atoi(sort_keys[i]) - 1;
      print_space_dash(i*4, SPACE);
      if (i != sort_idx - 1) {
         printf("%s\n", data[1][column]);
      } else {
         printf("%-*s ", max_len, data[1][atoi(sort_keys[i]) - 1]);
      }
   }

   // Print data from row 2 onwards only from specified columns given by user
   for (int row = 2; row < rows; row++) {
      int compare_val = compare_rows(data, row, row - 1, sort_idx, sort_keys);
      if (compare_val == SAME) {
         count++;
      } else {
         // Values in rows differ
         printf("%5d\n", count);
         break_idx = compare_val;
         // Print only values which differ
         for (int i = break_idx; i < sort_idx; i++) {
         print_space_dash(i*4, SPACE);
         if (i == sort_idx - 1) {
            printf("%-*s ", max_len, data[row][atoi(sort_keys[i]) - 1]);
         } else {
            printf("%s\n", data[row][atoi(sort_keys[i]) - 1]);
         }
         }
         // Reset count
         count = 1;
      }
   }
   printf("%5d\n", count);
}

/**************************************************************/

//algorithms are fun
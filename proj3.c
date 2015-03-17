/**
 * @file    proj3.c
 * @author  Jakub Vitasek <me@jvitasek.cz>
 * @date    1.12.2013
 * @brief   1BIT Project 3: Figure Lookups in Matrices
 * 
 * The program finds the longest horizontal/vertical lines or squares in a bitmap
 * saved as a .txt which contains a rectangular matrix of 0s and 1s. It then
 * prints the source/dest. coordinates of the line/square.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	int rows;
	int cols;
	char *cells;
} Bitmap;

char const *error = "\n\033[31;1m ERROR \033[0m\n";

char const *ERRMSG[] =
{
	"\nTo get the help message, type \"--help\" as the first argument (0 being ./proj3).\n\n",
	"You've entered too many arguments.",
	"Nothing to do.",
	"Unrecognized command.",
	"Nothing to do.",
	"File not found.",
	"Nothing found."
};

int arg_type_and_output(int argc, char const *argv[]); // Gets the user-side arguments.
void print_help(void); // Prints out the help message.
int load_file(Bitmap *bitmap, char const *argv[]); // Loads and tests the provided file.
int test_file(Bitmap *bitmap, char const *argv[]); // Tests the provided file.
char *init_array2D(Bitmap *bitmap, int r, int c); // Allocates memory for the 2D array.
void free_array2D(Bitmap *bitmap); // Frees the allocated memory.
char getcolor(Bitmap *bitmap, int x, int y); // Gets the "color" of a number in the matrix.
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2); // Finds the longest horizontal line.
int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2); // Finds the longest vertical line.
int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2); // Find the biggest square.

int main(int argc, char const *argv[])
{
	if(argc > 3)
	{
		fprintf(stderr, "%s%s%s", error, ERRMSG[1], ERRMSG[0]);	
		return EXIT_FAILURE;
	}
	else if(argc < 2)
	{
		fprintf(stderr, "%s%s%s", error, ERRMSG[2], ERRMSG[0]);
		return EXIT_FAILURE;
	}
	arg_type_and_output(argc, argv);
	return 0;
}

/* Gets the user-side arguments. */
int arg_type_and_output(int argc, const char *argv[])
{
	if(strcmp(argv[1], "--help") == 0)
	{
		print_help();
		return EXIT_SUCCESS;
	}
	else if(argc == 3)
	{
		int x1 = 0;
		int x2 = 0;
		int y1 = 0;
		int y2 = 0;
		Bitmap bitmap;
		
		if(load_file(&bitmap, argv) == 1)
		{
			printf("Invalid\n");
			return EXIT_FAILURE;
		}
		else
		{
			if((strcmp(argv[1], "--test") == 0))
			{
				if(test_file(&bitmap, argv) == 1)
				{
					printf("Invalid\n");
					return EXIT_SUCCESS;
				}
				else
				{
					printf("Valid\n");
					return EXIT_SUCCESS;
				}
			}
			else if((strcmp(argv[1], "--hline") == 0))
			{
				if(test_file(&bitmap, argv) == 1)
				{
					fprintf(stderr, "Invalid\n");
					return EXIT_FAILURE;
				}
				else
				{
					find_hline(&bitmap, &x1, &y1, &x2, &y2);
					if((x1 == 0) && (x2 == 0) && (y1 == 0) && (y2 == 0))
					{
						fprintf(stderr, "%s%s%s", error, ERRMSG[6], ERRMSG[0]);
						return EXIT_FAILURE;
					}
					printf("%d %d %d %d\n", x1, y1, x2, y2);
					return EXIT_SUCCESS;
				}
			}
			else if((strcmp(argv[1], "--vline") == 0))
			{
				if(test_file(&bitmap, argv) == 1)
				{
					fprintf(stderr, "Invalid\n");
					return EXIT_FAILURE;
				}
				else
				{
					find_vline(&bitmap, &x1, &y1, &x2, &y2);
					if((x1 == 0) && (x2 == 0) && (y1 == 0) && (y2 == 0))
					{
						fprintf(stderr, "%s%s%s", error, ERRMSG[6], ERRMSG[0]);
						return EXIT_FAILURE;
					}
					printf("%d %d %d %d\n", x1, y1, x2, y2);
					return EXIT_SUCCESS;
				}
			}
			else if((strcmp(argv[1], "--square") == 0))
			{
				if(test_file(&bitmap, argv) == 1)
				{
					fprintf(stderr, "Invalid\n");
					return EXIT_FAILURE;
				}
				else
				{
					find_square(&bitmap, &x1, &y1, &x2, &y2);
					if((x1 == 0) && (x2 == 0) && (y1 == 0) && (y2 == 0))
					{
						fprintf(stderr, "%s%s%s", error, ERRMSG[6], ERRMSG[0]);
						return EXIT_FAILURE;
					}
					printf("%d %d %d %d\n", x1, y1, x2, y2);
					return EXIT_SUCCESS;
				}
			}
			free_array2D(&bitmap);
		}

		
	}
	fprintf(stderr, "%s%s%s", error, ERRMSG[3], ERRMSG[0]);
	return EXIT_FAILURE;
}

/* Prints the help message. */
void print_help(void)
{
	printf("\n\033[32;1m HELP \033[0m\n"
		 "\n\"Figure Lookups in Matrices\" - proj3.\n"
         	 "Author: Jakub Vitasek (c) 2013\n"
        	 "The program finds the longest horizontal/vertical lines\n"
         	 "or squares in a bitmap saved as a .txt which contains\n" 
       		 "a rectangular matrix of 0s and 1s.\n"
         	 "Program starts upon entering the following prompts:\n"
         	 "./proj2 --help\n"
         	 "./proj2 --test soubor.txt\tTests the file for validity.\n"
         	 "./proj2 --hline obrazek.txt\tLooks for the longest hline.\n"
         	 "./proj2 --vline obrazek.txt\tLooks for the longest vline.\n"
         	 "./proj2 --square obrazek.txt\tLooks for the biggest square.\n\n");
}

/* Allocates memory for the 2D array. */
char *init_array2D(Bitmap *bitmap, int r, int c)
{
	bitmap->rows = r;
	bitmap->cols = c;
	return bitmap->cells = malloc(r * c * sizeof(*bitmap->cells));
}

/* Frees the allocated memory. */
void free_array2D(Bitmap *bitmap)
{
	free(bitmap->cells);
	bitmap->cells = NULL;
}

/* Loads the provided file. */
int load_file(Bitmap *bitmap, char const *argv[])
{
	FILE *f; 
	if((f = fopen(argv[2], "r")) == NULL)
	{
		fclose(f);
		fprintf(stderr, "%s%s%s", error, ERRMSG[5], ERRMSG[0]);
		return EXIT_FAILURE;
	}
	int r, c, placehold = 0;

	fscanf(f, "%d%d", &r, &c);
	init_array2D(bitmap, r, c);

	for(int i = 0; i < r*c; i++)
	{
		fscanf(f, "%d", &placehold);
		if(placehold == 1 || placehold == 0)
			bitmap->cells[i] = placehold;
		else
		{
			printf("Invalid\n");
			return EXIT_FAILURE;
		}
	}
	fclose(f);
	return EXIT_SUCCESS;
}

/* Tests the provided file. */
int test_file(Bitmap *bitmap, char const *argv[])
{
	int r = bitmap->rows;
	int c = bitmap->cols;
	int x = 0;
	FILE *f;
	if((f = fopen(argv[2], "r")) == NULL)
		return 1;
	
	for(int i = 0; i < r*c; i++)
	{
		if(fscanf(f, "%d", &x) != 1)
		{
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

/* Gets the "color" of a number in the matrix. */
char getcolor(Bitmap *bitmap, int x, int y)
{
	return bitmap->cells[bitmap->cols * y + x];
}

/* Finds the longest horizontal line. */
int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
	int max, count = 0;
	for(int i = 0; i < bitmap->rows; i++)
	{
		count = 0;
		for(int j = 0; j < bitmap->cols; j++)
		{
			if(getcolor(bitmap, j, i) == 1)
				count++;
			else
				count = 0;

			if(count > max)
			{
				max = count;
				*x1 = i;
				*x2 = i;
				*y1 = j - max + 1; // Starts at zero!
				*y2 = j;
			}
		}
	}
	return EXIT_SUCCESS;
}

/* Finds the longest vertical line. */
int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
	int max, count = 0;
	for(int j = 0; j < bitmap->cols; j++)
	{
		count = 0;
		for(int i = 0; i < bitmap->rows; i++)
		{
			if(getcolor(bitmap, j, i) == 1)
				count++;
			else
				count = 0;

			if(count > max)
			{
				max = count;
				*x1 = i - max + 1; // Starts at zero!
				*x2 = i;
				*y1 = j;
				*y2 = j;
			}
		}
	}
	return EXIT_SUCCESS;
}

/* Finds the biggest square. */
int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
	int count, max = 0;
	int n = 0;
	int calc = 0;
	int length_left = 0;
	int length_right = 0;
	int length_bot = 0;
	for(int i = 0; i < bitmap->rows; i++)
	{
		count = 0;
		n = 0;
		calc = 0;
		for(int j = 0; j < bitmap->cols; j++)
		{
			calc++;
			if(getcolor(bitmap, j, i) == 1)
				count++;
			else
				count = 0;

			if(count == 3) // We're interested.
			{
				// Loading the possible rest of ones.
				if(calc == bitmap->cols)
					n = 0;
				else
				{
					while((getcolor(bitmap, j+n, i) == 1) && (n < bitmap->cols))
						n++;
				}
				// We have to account for the ones already counted.
				n += count;

				// Left side check.
				while((getcolor(bitmap, j-count+1, i+length_left) == 1) && (length_left < n))
					length_left++;

				// Right side check.
				while((getcolor(bitmap, j+n, i+length_right) == 1) && (length_right < n))
					length_right++;

				// Bottom side check.
				while((getcolor(bitmap, j+length_bot, i+n-1) == 1) && (length_bot < n))
					length_bot++;

				if((n == length_right) && (n == length_left) && (n == length_bot) && (count > max))
				{
					*x1 = i;
					*y1 = j-count+1;
					*x2 = i+n-1;
					*y2 = j;
					max = count;
				}
			}
		}
	}
	return 0;
}
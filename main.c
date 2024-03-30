#include <stdarg.h>       // va_*
#include <stdio.h>        // calloc
#include <stdlib.h>       // printf
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>        // time()
#include <unistd.h>      // sleep()

/* TO DEV
    
    Functions :
    - user_evaluation.
    - take into account user_evaluation.

    Features :
    - manage separator : '\n' ' '
    - get the directory of this script to add the data dir into the same dir as it.
    - restart -> loop into main.
    
*/

void read_input(char **ptr_input, int lenght_wished)
{
    int max_lenght = lenght_wished + 1 ;
    printf("Please provide an input : ") ;
    int i = 0 ;
    if (*ptr_input == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
        return ;
    }
    for (i=0 ; i<max_lenght; i++) 
    { (*ptr_input)[i] = getchar() ;
      if ((*ptr_input)[i] == '\n')
      	{ (*ptr_input)[i] = '\0' ;
      	  break ;
      	}
      else if (i==(max_lenght-1))
      { (*ptr_input)[i] = '\0' ;
        break ;
      }
    }
}

void empty_file(char *file_path)
{
    FILE *fptr = fopen(file_path, "w") ;
    fclose(fptr); 
}

void write_input_to_file(char **ptr_input, char *file_path)
{
    int i = 0 ;
    if (*ptr_input == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
        return ;
    }

    FILE *fptr = fopen(file_path, "a") ;
    while ((*ptr_input)[i] != '\0')
    {
      fputc((*ptr_input)[i], fptr) ;
      i++ ;
    }
    fprintf(fptr, "\n") ;
    fclose(fptr); 
}

// Count the number of lines within a file.
int lines_number_get(char *file_path)
{
  FILE *fptr = fopen(file_path, "r");
  long int lines =0;

  if ( fptr == NULL ) {
    return -1;
  }

  while (EOF != (fscanf(fptr, "%*[^\n]"), fscanf(fptr,"%*c")))
        ++lines;
  
  return lines;
}

// Concatenate a number of 'count' string passed to the function.
char* concat(int count, ...)
{
    va_list ap;
    int i;

    // Find required length to store merged string
    int len = 1; // room for NULL
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    // Allocate memory to concat strings
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    // Actually concatenate strings
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
    {
        char *s = va_arg(ap, char*);
        strcpy(merged+null_pos, s);
        null_pos += strlen(s);
    }
    va_end(ap);

    return merged;
}

int random_get_from_range(int min, int max)
{
  int random_number = min + rand() / (RAND_MAX / (max - min + 1) + 1) ;
  printf("Random number : %d\n", random_number) ;
  return random_number ;
}

char* get_line_content_from_file(char * file_path, int line_number, int length)
{
  int max_length = length + 1 ; 
  char *line_content = calloc(sizeof(char), max_length); 
  int current_line = 0 ;
  FILE *fptr = fopen(file_path, "r") ;
  
  do 
  {
    if (++current_line == line_number) {
      fgets(line_content, sizeof line_content, fptr);
      break;
    }

  } while((fscanf(fptr, "%*[^\n]"), fscanf(fptr, "%*c")) != EOF);
  fclose(fptr);
  if(current_line == line_number)
  { return line_content ; }
  else
    { return NULL ; } // in case of error
}


int main()
{
    // Link seed to the time to get ALWAYS different results.
    srand(time(NULL)) ;
  
    int i ; // used in several loops
    int length = 100 ;
    char input_from_user[length] ;
    char data_dir[] = "data" ;
    char file_basename[] = "inputs_list.txt" ;
    char * file_path =  concat(3, data_dir, "/", file_basename) ; 
    char * ptr_input ;
    ptr_input = input_from_user ;
    FILE *fptr;

    /*
    // Test whether the data directory exists and create it else.
    struct stat st = {0};
    if (stat(data_dir, &st) == -1)
      { printf("Creation of the directory %s.\n", data_dir) ;
        mkdir(data_dir, 0777); }
    else
      { printf("The directory \"%s\" already exists.\n", data_dir) ; }
    */

    /*
    // Optional : Empty file
    empty_file(file_path) ;

    // Get inputs from user
    for (i=0 ; i<5; i++)
    {
        read_input(&ptr_input, length) ;
        printf("Input provided : '%s'\n", input_from_user) ;
        write_input_to_file(&ptr_input, file_path) ;
    }
    */
    
    int lines = lines_number_get(file_path) ;
    int minimum_number = 1 ;
    int max_number_of_outputs = 10 ;
    int number_of_random_outputs_to_get = random_get_from_range(minimum_number, max_number_of_outputs) ;
    char * new_output ;
    char * current_output ; 
    char * final_output ;
    int rand_number ;

    // Define current_output
    rand_number = random_get_from_range(minimum_number, lines) ;
    current_output = get_line_content_from_file(file_path, rand_number, length) ;
    // Start at second loop with index of 1.
    for (i=1 ; i<number_of_random_outputs_to_get; i++)
    {
        rand_number = random_get_from_range(minimum_number, lines) ;
        new_output = get_line_content_from_file(file_path, rand_number, length) ;
	final_output = concat(2, current_output, new_output) ;
	current_output = final_output ;
    }

    // Print output
    printf("Final output size %d\nContent '%s'\n", number_of_random_outputs_to_get, final_output) ;
    
    return 0 ;
 }


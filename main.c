#include <stdarg.h>       // va_*
#include <stdio.h>        // calloc
#include <stdlib.h>       // printf
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/* TO DEV
    
    Functions :
    - create_random_output on existing list.
    - show_random_output.
    - user_evaluation.
    - take into account user_evaluation.

    Features :
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

void empty_file(FILE *fptr, char *filename)
{
    fptr = fopen(filename, "w") ;
    //fptr = fopen("input_list.txt", "w") ;
    fclose(fptr); 
}

void write_input_to_file(char **ptr_input, char *filename, FILE *fptr)
{
    int i = 0 ;
    if (*ptr_input == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
        return ;
    }

    fptr = fopen(filename, "a") ;
    while ((*ptr_input)[i] != '\0')
    {
      fputc((*ptr_input)[i], fptr) ;
      i++ ;
    }
    fprintf(fptr, "\n") ;
    fclose(fptr); 
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

int main()
{   
    int length = 100 ;
    char input_from_user[length] ;
    char data_dir[] = "data" ;
    char file_basename[] = "inputs_list.txt" ;
    char * file_path =  concat(3, data_dir, "/", file_basename) ; 
    char * ptr_input ;
    ptr_input = input_from_user ;
    FILE *fptr;

    // Test whether the data directory exists and create it else.
    struct stat st = {0};
    if (stat(data_dir, &st) == -1)
      { printf("Creation of the directory %s.\n", data_dir) ;
        mkdir(data_dir, 0777); }
    else
      { printf("The directory \"%s\" already exists.\n", data_dir) ; }
    
    empty_file(fptr, file_path) ;
    
    read_input(&ptr_input, length) ;
    printf("Input provided : '%s'\n", input_from_user) ;

    write_input_to_file(&ptr_input, file_path, fptr) ;
    
    read_input(&ptr_input, length) ;
    printf("Input provided : '%s'\n", input_from_user) ;
    write_input_to_file(&ptr_input, file_path, fptr) ;
    return 0 ;
 }


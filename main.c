#include <string.h>
#include <stdio.h>
#include <stdlib.h>
/* TO DEV

    Features :
    - dynamic filename - provide the name to functions but define the filename outside.
    - create directory for data dir
    - use path to data dir
    - restart -> loop into main.
    
    Functions :
    - create_random_output on existing list.
    - show_random_output.
    - user_evaluation.
    - take into account user_evaluation.

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

void empty_file(FILE *fptr)
{
    fptr = fopen("input_list.txt", "w") ;
    fclose(fptr); 
}

void write_input_to_file(char **ptr_input, FILE *fptr)
{
    int i = 0 ;
    if (*ptr_input == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
        return ;
    }

    fptr = fopen("input_list.txt", "a") ;
    while ((*ptr_input)[i] != '\0')
    {
      fputc((*ptr_input)[i], fptr) ;
      i++ ;
    }
    fprintf(fptr, "\n") ;
    fclose(fptr); 
}

int main()
{   
    int length = 100 ;
    char input_from_user[length] ;
    char * ptr_input ;
    ptr_input = input_from_user ;
    FILE *fptr;

    empty_file(fptr) ;
    
    read_input(&ptr_input, length) ;
    printf("Input provided : '%s'\n", input_from_user) ;    
    write_input_to_file(&ptr_input, fptr) ;
    
    read_input(&ptr_input, length) ;
    printf("Input provided : '%s'\n", input_from_user) ;
    write_input_to_file(&ptr_input, fptr) ;
 
}


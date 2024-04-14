#include <stdarg.h>       // va_*
#include <stdio.h>        // printf
#include <stdlib.h>       // calloc
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>        // time()
#include <unistd.h>      // sleep()

//////////////////////////////////////////////////////////////////////
/////////////////////// PREVIOUS CODE ////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
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

int random_get_from_range(int min, int max)
{
  int random_number = min + rand() / (RAND_MAX / (max - min + 1) + 1) ;
  return random_number ;
}

*/

////////////////////////////////////////////////////////////////////

/*
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
*/
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
/*    
    int lines = lines_number_get(file_path) ;
    int minimum_number = 1 ;
    int max_number_of_outputs = 10 ;
    int number_of_random_outputs_to_get = random_get_from_range(minimum_number, max_number_of_outputs) ;
    char * new_output ;
    char * current_output ; 
    char * final_output ;
    // Define here the special char that can be separators.
    char* special_char_list = " " ;
    int special_char_list_length = strlen(special_char_list) ;
    int random_index_special_char ; 
    char random_special_char ;
    int rand_number ;
    int new_output_length ;

    // Define current_output
    current_output = "" ;
    
    // Get outputs
    for (i=0 ; i<number_of_random_outputs_to_get; i++)
    {
        rand_number = random_get_from_range(minimum_number, lines) ;
        new_output = get_line_content_from_file(file_path, rand_number, length) ;
	new_output_length = strlen(new_output) ;
	// Replace the separator '\n' by a special char defined in the above list.
	if (new_output[new_output_length-1] == '\n')
        {
	  random_index_special_char = random_get_from_range(0, special_char_list_length) ;
	  random_special_char = special_char_list[random_index_special_char] ;
	  new_output[new_output_length-1] = random_special_char ;
	}

	// Concatenate current output with new output
	final_output = concat(2, current_output, new_output) ;
	// Set current output to (new) final_output
	current_output = final_output ;
    }

    // Print output
    printf("Output produced : '%s'\n", final_output) ;
    
    return 0 ;
 }
*/

//////////////////////////////////////////////////////////////////////
/////////////////////// NEW CODE /////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
/////////////////////// FUNCTIONS ////////////////////////////////////
//////////////////////////////////////////////////////////////////////
    
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

// Get a specific line from a file
void get_line_content_from_file(char **ptr_input, char * file_path, int line_number, int length)
{
  int i ;
  int max_length = length + 1 ; 
  char *line_content = calloc(sizeof(char), max_length); 
  int current_line = 0 ;
  FILE *fptr = fopen(file_path, "r") ;
  if (*ptr_input == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
      return ;
    }
  do 
  {
    if (++current_line == line_number) {
      for (i=0 ; i<max_length; i++) 
	{ (*ptr_input)[i] = fgetc(fptr) ;
	  if ((*ptr_input)[i] == '\n')
	    { (*ptr_input)[i] = '\0' ;
	      break ;
	    }
	  else if (i==(max_length-1))
	    { (*ptr_input)[i] = '\0' ;
	      break ;
	    }
    }
      break;
    }

  } while((fscanf(fptr, "%*[^\n]"), fscanf(fptr, "%*c")) != EOF);
  fclose(fptr);
}

// Get the length of a string.
size_t strlen(const char *str)
{
    const char *s;
    for (s = str; *s; ++s);
    return(s - str);
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

// Convert a string to an int array
void convert_string_to_int_array(char **ptr_input, int **ptr_output, int length)
{
    int i ; // Used in loop
    int current_char ;
    int current_int ;
    if (*ptr_input == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
        return ;
    }
    if (*ptr_output == NULL)
    {  printf("ERROR : a nul pointer was provided to the function.\n") ;
        return ;
    }
    for (i=0 ; i<length; i++) 
    {
      // If nul char, then fill with 0 the current char and the rest.
      if ((*ptr_input)[i] == '\0')
      { (*ptr_output)[i] = 0 ;
	// Fill the rest of the array with 0 = dummy number
	for (++i; i<length; i++)
	{ (*ptr_output)[i] = 0 ; }
	  break ;
      }
      // If reached the index of the max_length (=max_length-1)
      else if (i==(length-1))
      { (*ptr_output)[i] = 0 ;
        break ;
      }
      // Else fill normally with the content of the char casted to int.
      else
      { current_char = (*ptr_input)[i] ;
      // Conversion from char to int here
        current_int = (int)current_char ;
        (*ptr_output)[i] = current_int ;
      }
    }
}

//////////////////////////////////////////////////////////////////////
/////////////////////// MAIN CODE ////////////////////////////////////
//////////////////////////////////////////////////////////////////////

/*
  Enhancements :
  - read/write from binary files
      - Test if they exists.
          - If not, create them.
      - Else read from them.
  - convert int to short int to use less memory. 
  - re-write the bash script into C
      - Use functions as much as possible.
      - Set the random range of token from 1-NB-OF-TOKENS as 0 is the dummy one.
  - Results
      - Keep a list of all proposed words with an indicator of 0 (good) / 1 (bad) qualifying the matches.
      - Keep a list of all token and their frequence in the list of good word found (matches).
          - Sort this list by :
	      - 1. Frequence
	      - 2. Alphabetical order
      - Produce analysis of average delta (timestamps, indexes)
  - Speed
      - Use hash to set token and dictionary words in places much fast to check.
      - Avoid looping as much as possible.
      - Mesure speed of the code block to analyse what to improve and compare with precedent code.
  - New functionalities :
      - Use the ponderation of token frequence whithin good matches to propose new words.
          - Alter the random of token proposition with this ponderation.
      - Train AI to propose words.
      - Use the training to filter the proposition (random) of words
          - When a word is proposed, it is first checked by AI and if OK, then checked with the dictionary.
*/

int main()
{
    // Files
    char data_dir[] = "data" ;
    char basename_token[] = "base_token.txt" ;
    char basename_dictionary[] = "dictionary.txt" ;
    char * path_token =  concat(3, data_dir, "/", basename_token) ;
    char * path_dictionary =  concat(3, data_dir, "/", basename_dictionary) ;
    int total_lines_file_token = lines_number_get(path_token) ;
    //int total_lines_file_dictionary = lines_number_get(path_dictionary) ; // Real data 
    int total_lines_file_dictionary = 10000 ; // Use for test while developping.

    // Arrays (char, int, array of int)
    short int word_length = 5 ; // Could be adapted dynamically by getting the longest length of all words from the dictionary.
    char * ptr_char_array = malloc(word_length * sizeof(char)) ;
    int word_max_length = word_length + 1 ;
    int * ptr_int_array = malloc(word_length * sizeof(int)) ;
    int * array_of_array_of_token_int = (int *)malloc(total_lines_file_token * word_max_length * sizeof(int)) ;
    int* array_of_array_of_dictionary_int = (int *)malloc(total_lines_file_dictionary * word_max_length * sizeof(int)) ;

    // Others
    int i, j ; // used in several loops
    int current_line ;
    
    //////////////////// START POINT ////////////////////////

    printf("Using a token file of %d lines.\n", total_lines_file_token) ;
    printf("Using a dictionary file of %d lines.\n", total_lines_file_dictionary) ;
    
    // Read each line of the file
    for (i=0 ; i<total_lines_file_token ; i++)
    {
      current_line = i+1 ;
      //get_line_content_from_file(&ptr_char_array, path_token, current_line, word_max_length) ;
      get_line_content_from_file(&ptr_char_array, path_token, current_line, word_max_length) ;
      // DEBUG printf("%d : '%s'\n", current_line, ptr_char_array) ;
      // Convert the content to int
      convert_string_to_int_array(&ptr_char_array, &ptr_int_array, word_max_length) ;

      // Fill the array of array of ints
      for (j=0; j<word_max_length; j++)
      { *(array_of_array_of_token_int+i*word_max_length+j) =  ptr_int_array[j] ;
      }
    }

/* DEBUG
    // Print its content
    for (i=0; i<total_lines_file_token; i++)
    {
      printf("Content of array %d\n", i) ;
      for (j=0; j<word_max_length; j++)
	{ printf("Index %d : value %d\n", j, *(array_of_array_of_token_int + i*word_max_length + j)) ;
      }
      printf("\n") ; // Esthetic
    }
*/

    // Read each line of the file
    for (i=0 ; i<total_lines_file_dictionary ; i++)
    {
      current_line = i+1 ;
      get_line_content_from_file(&ptr_char_array, path_dictionary, current_line, word_max_length) ;
      // DEBUG printf("%d : '%s'\n", current_line, ptr_char_array) ;
      // Convert the content to int
      convert_string_to_int_array(&ptr_char_array, &ptr_int_array, word_max_length) ;

      // Fill the array of array of ints
      for (j=0; j<word_max_length; j++)
      { *(array_of_array_of_dictionary_int+i*word_max_length+j) =  ptr_int_array[j] ;
      }
    }

/* DEBUG
    // Print its content
    for (i=0; i<total_lines_file_dictionary; i++)
    {
      printf("Content of array %d\n", i) ;
      for (j=0; j<word_max_length; j++)
	{ printf("Index %d : value %d\n", j, *(array_of_array_of_dictionary_int + i*word_max_length + j)) ;
      }
      printf("\n") ; // Esthetic
    }
*/
    
    // Freeing all pointers
    printf("Freeing all memory pointer...\n") ; 
    free(path_token) ;
    free(path_dictionary) ;
    free(ptr_char_array) ;
    free(ptr_int_array) ;
    free(array_of_array_of_token_int) ;
    free(array_of_array_of_dictionary_int) ;

    // Exit message
    printf("End of main\n") ;
    return 0 ;
}


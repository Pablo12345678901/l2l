 /* Macro */
#define _POSIX_SOURCE 0
#define _POSIX_C_SOURCE 200809L
#define _LARGEFILE_SOURCE
#define _LARGEFILE64_SOURCE
#define _FILE_OFFSET_BITS 64
#define _ISOC11_SOURCE
#define _GNU_SOURCE

/* Libc libraries */
#include <dirent.h>       /* */
#include <errno.h>        /* Error management */
#include <fcntl.h>        /* */
#include <grp.h>          /* */
#include <libgen.h>       /* Finding Tokens in a String */
#include <limits.h>       /* */
#include <pwd.h>          /* */
#include <signal.h>       /* */
#include <stdio.h>        /* printf */
#include <stdlib.h>       /* calloc */
#include <string.h>       /* */
#include <sys/stat.h>     /* File mode */
#include <sys/times.h>    /* */
#include <sys/types.h>    /* */
#include <termios.h>      /* */
#include <time.h>         /* time() */
#include <unistd.h>       /* sleep() */

/* Other libraries */
#include <stdarg.h>       /* va_* */
#include <stdbool.h>      /* boolean type 'bool' */

/**************************************************
***************************************************
**************************************************/

/* Functions */

/* Test whether a file exists */
bool file_exists(char *filename)
{
  struct stat buffer;   
  return (stat (filename, &buffer) == 0);
}

/* Count the number of lines within a file. */
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

/* Get a specific line from a file */
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

/* Concatenate a number of 'count' string passed to the function. */
char * concat(int count, ...)
{
    va_list ap;
    int i;

    /* Find required length to store merged string */
    int len = 1; /* room for NULL */
    va_start(ap, count);
    for(i=0 ; i<count ; i++)
        len += strlen(va_arg(ap, char*));
    va_end(ap);

    /* Allocate memory to concat strings */
    char *merged = calloc(sizeof(char),len);
    int null_pos = 0;

    /* Actually concatenate strings */
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

/* Convert a string to an int array */
void convert_string_to_int_array(char **ptr_input, int **ptr_output, int length)
{
    int i ; /* Used in loops */
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
      /* If nul char, then fill with 0 the current char and the rest. */
      if ((*ptr_input)[i] == '\0')
      { (*ptr_output)[i] = 0 ;
	/* Fill the rest of the array with 0 = dummy number */
	for (++i; i<length; i++)
	{ (*ptr_output)[i] = 0 ; }
	  break ;
      }
      /* If reached the index of the max_length (=max_length-1) */
      else if (i==(length-1))
      { (*ptr_output)[i] = 0 ;
        break ;
      }
      /* Else fill normally with the content of the char casted to int. */
      else
      { current_char = (*ptr_input)[i] ;
	/* Conversion from char to int here */
        current_int = (int)current_char ;
        (*ptr_output)[i] = current_int ;
      }
    }
}

/* From the errno, and any additional message, print an error message to stderr */
void print_error_to_stderr(int error_code, char * error_message_other)
{ char * error_message_head = "ERROR:" ;
  char * error_message_specific = strerror(errno) ;
  if(strlen(error_message_other) >= 1)
    { char * error_message_all = concat(5, error_message_head, " ", error_message_specific, " ", error_message_other) ;
     perror(error_message_all) ;
    }
  else
    { char * error_message_all = concat(3, error_message_head, " ", error_message_specific) ;
      perror(error_message_all) ;
    }
}

int directory_test_if_exists_or_create_with_error_management(char * directory_path)
{
  /* Control if the result directory does not exists, create it else */
  if (file_exists(directory_path) == 0)
    { char * error_no_such_dir = concat(3, ": \"", directory_path, "\" - creating it... ") ;
      print_error_to_stderr(errno, error_no_such_dir) ;
      /* Test whether creation was well performed and print error message else. */
      if (mkdir(directory_path, 0755) != 0)
	{ char * error_mkdir = concat(3, ": mkdir(", directory_path, ") error ") ;
	  print_error_to_stderr(errno, error_mkdir) ;
	  /* Exit with error if could not create it. */
	  exit(EXIT_FAILURE) ; 
	}
    }
}

/* Control if the file exists, delete it if so. */
int file_test_if_exists_and_remove_with_error_management(char * path_file)
{
  /* Control if the file exists. */
  if (file_exists(path_file) == 1)
    { /* Remove it if so and check if it was well done - exit with error else. */
      if (remove(path_file) != 0)
      { char * error_rm_file = concat(3, ": remove(", path_file, ") error ") ;
	print_error_to_stderr(errno, error_rm_file) ;
	exit(EXIT_FAILURE) ;
      }
    }
}

/* Write an input to a file with a backline char at the end '\n' */
void write_input_to_file(char *ptr_input, char *file_path)
{    
    /* Check whether the pointer is not null. */
    if (ptr_input == NULL)
    { printf("ERROR : a nul pointer was provided to the function.\n") ;
      return ;
    }
    /* Append the content of the input to the file. */
    FILE *fptr = fopen(file_path, "a") ;
    int i = 0 ; /* For loops */
    while (ptr_input[i] != '\0')
      { fputc(ptr_input[i], fptr) ;
	i++ ;
      }
    /* Add backline at the end. */
    fprintf(fptr, "\n") ;
    fclose(fptr); 
}

/* Copy a file to another with error management */
/* Source : https://stackoverflow.com/questions/2180079/how-can-i-copy-a-file-on-unix-using-c */
int copy_a_file_to_another_with_error_management(const char *from, const char *to)
{
  /* File descriptors */
  int fd_to, fd_from;
  /* Buffer size set for efficiency */
  char buf[4096];
  /* Counting the number of bytes copied */
  ssize_t nread;
  /* For saving potential error */
  int saved_errno;
  
  /* Try to open file descriptor for copied file */
  fd_from = open(from, O_RDONLY);
  if (fd_from < 0)
    goto out_error;

  /* Try to open file descriptor for copy file. */
  fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
  if (fd_to < 0)
    goto out_error;
    
  /* While there is still something to read from the file descriptor of the copied file */
  while (nread = read(fd_from, buf, sizeof buf), nread > 0)
    {
      /* Set the output pointer to the buffer */
      char *out_ptr = buf;
      /* Conserve the number of bytes written */
      ssize_t nwritten;
      
      /* Loop until there is nothing else to read. */
      do
	{
	  nwritten = write(fd_to, out_ptr, nread);

	  /* If the number of written bytes is greater than 0. */
	  if (nwritten >= 0)
            {
	      /* Reduce the number of bytes to read. */
	      nread -= nwritten;
	      /* Move the ptr of the number of written bytes */
	      out_ptr += nwritten;
            }
	  /* Else ...*/
	  else if (errno != EINTR)
	    {
	      goto out_error;
	    }
	} while (nread > 0);
    }

    /* If nothing else is to be read */
    if (nread == 0)
    {
      /* Test to close the file descriptor of the copy file */
        if (close(fd_to) < 0)
        {
            fd_to = -1;
            goto out_error;
        }
	/* Close the file descriptor of the copied file */
        close(fd_from);

        /* Success! */
        return 0;
    }

 /* Manage error */
 out_error:
    saved_errno = errno;

    /* Close the file descriptor of copied file */
    close(fd_from);
    /* If it exists, close the file descriptor of the copy file */
    if (fd_to >= 0)
        close(fd_to);
    
    /* Print error message and exit */
    char * error_cp_file = concat(5, ": copy \"", from, "\" to \"", to, "\" failed ") ;
    print_error_to_stderr(saved_errno, error_cp_file) ;
    /* Exit with error if could not create it. */
    exit(EXIT_FAILURE) ; 
}

/**************************************************
***************************************************
***************************************************
**************************************************/

/* MAIN CODE */

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

  - Others
      - Option management.
      - Test command failure + Error message.
      - Exit code incrementation.
      - Use as much as possible array of int.
      - Anaylse results, optimize model.
      - Add capital letter support
      - Add 'special' character support '.,-_ ...'
*/

int main(int argc, char **argv)
{
    /* Main indicators to set the model */
  int number_of_wished_proposed_results = 1000 ; /* Number of index within the array of proposed results. */
  int average_word_length = 4 ;
  int variation_max_word_size = 2 ;
  /* The randomly size of the word will be set in the range "1 <-> AVERAGE_WORD_LENGHT+VARIATION" or "LENGTH_MIN_WORD <-> AVERAGE_WORD_LENGHT+VARIATION" if the lenght min. is greater than 1. Cannot be lower than 1 : 0 word lenght does not exists. */

  /* General variables */
  int i ; /* For loops */
  
  /* Get the main directory : the one where this script is. */
  char * script_path = argv[0] ;
  char * script_directory = dirname(script_path) ;
  char * absolute_path_script_directory = canonicalize_file_name(script_directory) ;

  /* Data */
  char * basename_dir_data = "data" ;
  char * path_dir_data = concat(3, absolute_path_script_directory, "/", basename_dir_data) ;
  char * basename_list_of_base_token = "base_token.txt" ;
  char * path_list_of_base_token = concat(3, path_dir_data, "/", basename_list_of_base_token) ;
  char * basename_dictionary = "dictionary.txt" ; /* Source : https://github.com/dwyl/english-words/blob/master/words_alpha.txt */
  char * path_dictionary = concat(3, path_dir_data, "/", basename_dictionary) ;
  
  /* Outputs */
  char * basename_dir_results = "output" ;
  char * path_dir_results = concat(3, absolute_path_script_directory, "/", basename_dir_results) ;
  char * basename_list_deltas_timestamps_and_indexes_between_good_results = "deltas_timestamps_and_indexes_between_good_results.txt" ;
  char * path_list_deltas_timestamps_and_indexes_between_good_results = concat(3, path_dir_results, "/", basename_list_deltas_timestamps_and_indexes_between_good_results) ;
  char * basename_list_of_enhanced_token = "enhanced_token.txt" ;
  char * path_list_of_enhanced_token = concat(3, path_dir_results, "/", basename_list_of_enhanced_token) ;
  char * basename_dummy_file_to_receive_signal_to_show_that_command_is_finished = "dummy_file_to_receive_signal_to_show_that_command_is_finished.txt" ;
  char * path_dummy_file_to_receive_signal_to_show_that_command_is_finished = concat(3, path_dir_results, "/", basename_dummy_file_to_receive_signal_to_show_that_command_is_finished) ;

  /* Test whether directories exist, else test to create them, it no successfull, exit with failure code. */
  directory_test_if_exists_or_create_with_error_management(path_dir_results) ;
  directory_test_if_exists_or_create_with_error_management(path_dir_data) ;

  /* Test whether the file already exists and remove it if so. Will be re-created from scratch. */
  file_test_if_exists_and_remove_with_error_management(path_list_of_base_token) ; 

  /* Fill the token array and add each token to the base token file. */
  char array_of_base_token[100] ;
  int number_of_base_token = 26 ;
  char current_char = 'a' ;
  for (i=0; i<number_of_base_token; i++)
    { /* New cleared (calloc) memory space for the char */
      char * ptr_char = (char *) calloc(2, sizeof(char)) ; /* 2 : for the '\0' at the end of the string */
      *ptr_char = current_char ;
      array_of_base_token[i] = current_char ;
      /* Write the char to the base token file */
      write_input_to_file(ptr_char, path_list_of_base_token) ;
      /* Change the char at each loop */
      current_char++ ;
      /* Free memory for next loop */
      free(ptr_char) ; 
    }

  /* If the enhanced token file does not exists and copy the base token to it. */
  if (file_exists(path_list_of_enhanced_token) == 0)
    { copy_a_file_to_another_with_error_management(path_list_of_base_token, path_list_of_enhanced_token) ;
    }

  /* Set the path of base token to the enhanced ones. */
  char * path_list_of_current_token_used = path_list_of_enhanced_token ;

  /* Printing a message before leaving script with sucess code */
  printf("\nEnd of script.\n") ;
  return EXIT_SUCCESS ;
}

/**************************************************
***************************************************
**************************************************/

/* BASH CODE REMAINING FOR THE TRANSLATION FROM BASH TO C

# Update the number of token and therefore the maximum index available by counting the lines of the file.
# As it can increase from a run to another, it is important to do it in order to enjoy the new token chunch as available token.
NUMBER_OF_TOKEN="$(cat $PATH_LIST_OF_BASE_TOKEN | wc -l)"
INDEX_MAX_TOKEN=$((NUMBER_OF_TOKEN-1))

# Set the dummy index for empty letter in a word as index_max + 1
# Could be enhanced to be set to 0).
DUMMY_INDEX_FOR_NO_TOKEN=$((INDEX_MAX_TOKEN+1))

# Set the range between which the each word lenght will be randomly choosed
# Set the mininum
# Avoid getting a word lenght of 0, always minimum lenght >=1
if [ $((AVERAGE_WORD_LENGTH-VARIATION_MAX_WORD_SIZE)) -lt 1 ]
then
    # If < 1 : set to 1
    LENGHT_MIN_WORD=1
else
    # Else, set normally
    LENGTH_MIN_WORD=$((AVERAGE_WORD_LENGTH-VARIATION_MAX_WORD_SIZE))
fi
# Set the maximum
LENGTH_MAX_WORD=$((AVERAGE_WORD_LENGTH+VARIATION_MAX_WORD_SIZE))

# Declare a two dimension array that will contains the list of array of proposed indexes (that can be translated to a list of words).
declare -A ARRAY_PROPOSED_TOKEN_INDEXES_ARRAYS
X_DIMENSION_SIZE=$NUMBER_OF_WISHED_PROPOSED_RESULTS
Y_DIMENSION_SIZE=$LENGTH_MAX_WORD

declare -a ARRAY_RESULTS_TRUE_FALSE_INT

# Loop to produce the results
for ((a=0; a<$NUMBER_OF_WISHED_PROPOSED_RESULTS; a++))
do
    # Get a random lenght base on a range of min-max value (set on the top of the screen). If the average lenght of a word is well set, its increase the probability to find a good result.
    CURRENT_REQUIRED_NUMBER_OF_INDEXES=$(shuf -i $LENGTH_MIN_WORD-$LENGTH_MAX_WORD -n 1)
    # Loop on each letter
    for ((b=0; b<$((CURRENT_REQUIRED_NUMBER_OF_INDEXES)); b++ ))
    do
	# Get a random index from the range, each index = one token
	CURRENT_PROPOSED_INDEX=$(shuf -i 0-$INDEX_MAX_TOKEN -n 1)
	# Add the token index to the array
	ARRAY_PROPOSED_TOKEN_INDEXES_ARRAYS[$a,$b]=$CURRENT_PROPOSED_INDEX
	# Add the equivalent letter to a 'word' variable
	WORD+="${ARRAY_TOKEN[$CURRENT_PROPOSED_INDEX]}"
    done
    # Fill the non-letter with a dummy number
    if [ $CURRENT_REQUIRED_NUMBER_OF_INDEXES -lt $LENGTH_MAX_WORD ]
    then
	NUMBER_OF_INDEXES_TO_FILL=$((LENGTH_MAX_WORD-CURRENT_REQUIRED_NUMBER_OF_INDEXES))
	FIRST_INDEX_TO_FILL=$((LENGTH_MAX_WORD-NUMBER_OF_INDEXES_TO_FILL))
	for ((c=$FIRST_INDEX_TO_FILL; c<$LENGTH_MAX_WORD; c++))
	do
	    ARRAY_PROPOSED_TOKEN_INDEXES_ARRAYS[$a,$c]=$DUMMY_INDEX_FOR_NO_TOKEN
	done
    fi

    # Search silently the proposed word within the dictionary
    # The represent a string that start by the word, and then a non-char at least once or immediately followed by the end of line '$'
    grep --max-count=1 -E "^$WORD([^a-zA-Z]+|$)" "$PATH_DICTIONARY" &>/dev/null

    # Get the return code of previous command within a variable. Useful for debugging.
    FLAG_DICTIONARY=$?
    
    # Conserve the result of the dictionary check within the results array
    ARRAY_RESULTS_TRUE_FALSE_INT+=($FLAG_DICTIONARY)
    
    # Unset the word
    WORD=
done

# Get new tokens chunck and add them to the file
PREVIOUS_INDEX_OF_POSITIVE_RESULT=

for ((d=0; d<$NUMBER_OF_WISHED_PROPOSED_RESULTS ; d++))
do
    # Only if the result was good
    if [ ${ARRAY_RESULTS_TRUE_FALSE_INT[$d]} -eq 0 ]
    then
	# Re-construct the word
	for ((e=0 ; e<$LENGTH_MAX_WORD ; e++ ))
	do
	    CURRENT_TOKEN=${ARRAY_PROPOSED_TOKEN_INDEXES_ARRAYS[$d,$e]}
	    # Check if it is a dummy token = means that the word ended and its length is lower than the maximum length.
	    if [ $CURRENT_TOKEN -eq $DUMMY_INDEX_FOR_NO_TOKEN ]
	    then
		# Break loop to save CPU.
		break
	    else
		# Else add the translated token to the word
		MATCHED_WORD="${MATCHED_WORD}${ARRAY_TOKEN[$CURRENT_TOKEN]}"
	    fi
	done
	
	# Get the new tokens
	# Get the word lenght
	MATCHED_WORD_LENGTH=${#MATCHED_WORD}
	MATCH_WORD_MAX_INDEX=$((MATCHED_WORD_LENGTH-1))
	# Get a random chuck length
	CHUNCK_LENGTH=$(shuf -i 1-$MATCHED_WORD_LENGTH -n 1)
	FIRST_INDEX_OF_END_OF_CHUNCK=$((CHUNCK_LENGTH-1))
	TOTAL_CHUCKS=$((MATCHED_WORD_LENGTH-CHUNCK_LENGTH+1))
	# Always set the chunck start index to 0 before looping
	CHUNCK_INDEX_START=0
	# Get the current chuck
	for ((f=$FIRST_INDEX_OF_END_OF_CHUNCK ; f<$MATCHED_WORD_LENGTH ; f++))
	do
	    NEW_CHUNCK="${MATCHED_WORD:CHUNCK_INDEX_START:CHUNCK_LENGTH}"
	    # Test if it is already in the enhanced token list.
	    # Search silently the proposed word within the enhanced token list.
	    # The represent a string that start by the word, and then a non-char at least once or immediately an end of line char '$'.
	    grep --max-count=1 -E "^${NEW_CHUNCK}([^a-zA-Z]+|$)" "$PATH_LIST_OF_ENHANCED_TOKEN" &>/dev/null
	    # Test if it not present into the file
	    if (($?))
	    then
		# If so, add it
		echo "$NEW_CHUNCK" >> "$PATH_LIST_OF_ENHANCED_TOKEN"
	    fi
	    # Increase the start index at each loop
	    ((CHUNCK_INDEX_START++))
	done
	# Unset the word for the next loop
	MATCHED_WORD=

	# Conserve the index of the current match
	# If the current index of result is set so there is already a match
	if [ ! -z $PREVIOUS_INDEX_OF_POSITIVE_RESULT ]
	then
	    # Compute the difference between timestamps of matched (=good) results.
	    # Precision with nanoseconds.
	    # DATE FORMAT : HOURS MINUTES SECONDS NANOSECONDS
	    CURRENT_TIMESTAMP="$(date +%H:%M:%S:%N)"
	    JOINED_TIMESTAMPS_STRING_FOR_COMPUTATION="$CURRENT_TIMESTAMP,$PREVIOUS_TIMESTAMP"
	    DELTA_TIMESTAMPS=$(awk -F '[:, ]' '{print ($1*60*60*1000+$2*60*1000+$3*1000+$4)-($5*60*60*1000+$6*60*1000+$7*1000+$8)}' <<< "$JOINED_TIMESTAMPS_STRING_FOR_COMPUTATION")
	    # Compute the difference between two index of matchs.
	    CURRENT_INDEX_OF_POSITIVE_RESULT=$d
	    DELTA_INDEXES_OF_POSITIVE_RESULTS=$((CURRENT_INDEX_OF_POSITIVE_RESULT-PREVIOUS_INDEX_OF_POSITIVE_RESULT))

	    # Add the result to STDOUT and file with tee
	    echo -e "DT: $DELTA_TIMESTAMPS \tDI: $DELTA_INDEXES_OF_POSITIVE_RESULTS \tCI: $CURRENT_INDEX_OF_POSITIVE_RESULT \tCT: $CURRENT_TIMESTAMP" |
	    tee -a "$PATH_LIST_DELTAS_TIMESTAMPS_AND_INDEXES_BETWEEN_GOOD_RESULTS"  
	fi

	# And set the current index and timestamp to the current one
	# DATE FORMAT : HOURS MINUTES SECONDS NANOSECONDS
	PREVIOUS_TIMESTAMP="$(date +%H:%M:%S:%N)"
	PREVIOUS_INDEX_OF_POSITIVE_RESULT=$d
     fi
done

# Print confirmation message with timestamp at the end as this script can be long to execute.
FINAL_SCRIPT_TIMESTAMP="$(date +%H:%M:%S:%N)"
echo "$FINAL_SCRIPT_TIMESTAMP : OK : SCRIPT FINISHED" |
# And tee this message to a dummy file that is monitored by the script that run a daemon
tee -a "$PATH_DUMMY_FILE_TO_RECEIVE_SIGNAL_TO_SHOW_THAT_COMMAND_IS_FINISHED" 
exit 0

*/ /* END OF BASH CODE */

/**************************************************
***************************************************
**************************************************/

/* OLD : C FUNCTIONS USED IN PRECEDENT VERSION OF CODE */

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
*/

/*
void empty_file(char *file_path)
{
    FILE *fptr = fopen(file_path, "w") ;
    fclose(fptr); 
}

*/

/*
int random_get_from_range(int min, int max)
{
  int random_number = min + rand() / (RAND_MAX / (max - min + 1) + 1) ;
  return random_number ;
}
*/

/**************************************************
***************************************************
**************************************************/

/* OLD : PRECEDENT VERSION OF CODE 1 = MORE RECENT */

/*
int main()
{
    char data_dir[] = "data" ;
    char basename_token[] = "base_token.txt" ;
    char basename_dictionary[] = "dictionary.txt" ;
    char * path_token =  concat(3, data_dir, "/", basename_token) ;
    char * path_dictionary =  concat(3, data_dir, "/", basename_dictionary) ;
    int total_lines_file_token = lines_number_get(path_token) ;
    int total_lines_file_dictionary = lines_number_get(path_dictionary) ; // Real data

    short int word_length = 5 ;
    char * ptr_char_array = malloc(word_length * sizeof(char)) ;
    int word_max_length = word_length + 1 ;
    int * ptr_int_array = malloc(word_length * sizeof(int)) ;
    int * array_of_array_of_token_int = (int *)malloc(total_lines_file_token * word_max_length * sizeof(int)) ;
    int* array_of_array_of_dictionary_int = (int *)malloc(total_lines_file_dictionary * word_max_length * sizeof(int)) ;

    int i, j ;
    int current_line ;
    
    printf("Using a token file of %d lines.\n", total_lines_file_token) ;
    printf("Using a dictionary file of %d lines.\n", total_lines_file_dictionary) ;
    
    for (i=0 ; i<total_lines_file_token ; i++)
    {
      current_line = i+1 ;
      get_line_content_from_file(&ptr_char_array, path_token, current_line, word_max_length) ;
      convert_string_to_int_array(&ptr_char_array, &ptr_int_array, word_max_length) ;

      for (j=0; j<word_max_length; j++)
      { *(array_of_array_of_token_int+i*word_max_length+j) =  ptr_int_array[j] ;
      }
    }
'''
    for (i=0 ; i<total_lines_file_dictionary ; i++)
    {
      current_line = i+1 ;
      get_line_content_from_file(&ptr_char_array, path_dictionary, current_line, word_max_length) ;
      convert_string_to_int_array(&ptr_char_array, &ptr_int_array, word_max_length) ;

    for (i=0; i<total_lines_file_token; i++)
    {
      printf("Content of array %d\n", i) ;
      for (j=0; j<word_max_length; j++)
	{ printf("Index %d : value %d\n", j, *(array_of_array_of_token_int + i*word_max_length + j)) ;
      }
      printf("\n") ;
    }
      
    for (j=0; j<word_max_length; j++)
      { *(array_of_array_of_dictionary_int+i*word_max_length+j) =  ptr_int_array[j] ;
      }
    }

    for (i=0; i<total_lines_file_dictionary; i++)
    {
      printf("Content of array %d\n", i) ;
      for (j=0; j<word_max_length; j++)
	{ printf("Index %d : value %d\n", j, *(array_of_array_of_dictionary_int + i*word_max_length + j)) ;
      }
      printf("\n") ;
    }

    printf("Freeing all memory pointer...\n") ; 
    free(path_token) ;
    free(path_dictionary) ;
    free(ptr_char_array) ;
    free(ptr_int_array) ;
    free(array_of_array_of_token_int) ;
    free(array_of_array_of_dictionary_int) ;

    printf("End of main\n") ;
    return 0 ;
}

*/

/**************************************************
***************************************************
**************************************************/

/* OLD : PRECEDENT VERSION OF CODE 2 = LESS RECENT */

/*
int main()
{
    srand(time(NULL)) ; 
    int i ; 
    int length = 100 ;
    char input_from_user[length] ;
    char data_dir[] = "data" ;
    char file_basename[] = "inputs_list.txt" ;
    char * file_path =  concat(3, data_dir, "/", file_basename) ; 
    char * ptr_input ;
    ptr_input = input_from_user ;
    FILE *fptr;
    
    struct stat st = {0};
    if (stat(data_dir, &st) == -1)
      { printf("Creation of the directory %s.\n", data_dir) ;
        mkdir(data_dir, 0777); }
    else
      { printf("The directory \"%s\" already exists.\n", data_dir) ; }

    empty_file(file_path) ;

    for (i=0 ; i<5; i++)
    {
        read_input(&ptr_input, length) ;
        printf("Input provided : '%s'\n", input_from_user) ;
        write_input_to_file(&ptr_input, file_path) ;
    }

    int lines = lines_number_get(file_path) ;
    int minimum_number = 1 ;
    int max_number_of_outputs = 10 ;
    int number_of_random_outputs_to_get = random_get_from_range(minimum_number, max_number_of_outputs) ;
    char * new_output ;
    char * current_output ; 
    char * final_output ;
    char* special_char_list = " " ;
    int special_char_list_length = strlen(special_char_list) ;
    int random_index_special_char ; 
    char random_special_char ;
    int rand_number ;
    int new_output_length ;

    current_output = "" ;
    
    for (i=0 ; i<number_of_random_outputs_to_get; i++)
    {
        rand_number = random_get_from_range(minimum_number, lines) ;
        new_output = get_line_content_from_file(file_path, rand_number, length) ;
	new_output_length = strlen(new_output) ;
	if (new_output[new_output_length-1] == '\n')
        {
	  random_index_special_char = random_get_from_range(0, special_char_list_length) ;
	  random_special_char = special_char_list[random_index_special_char] ;
	  new_output[new_output_length-1] = random_special_char ;
	}

	final_output = concat(2, current_output, new_output) ;
	current_output = final_output ;
    }

    printf("Output produced : '%s'\n", final_output) ;
    
    return 0 ;
 }
*/

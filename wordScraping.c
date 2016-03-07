#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CURL "curl"
#define PIPE_GREP "| grep -i \"<a class=.popular.\" >> "
//#define PIPE_SED  "| sed -n \"/<div class='meaning'>/,/</div>/p\""
#define WEBSITE " http://www.urbandictionary.com"

#define PIPE_SED  "| sed -n '/<div class=.meaning.>/,/<.div>/p' >> "


/* 
 * Word Scraping
 */



char* concat(char *s1, char *s2);


int main(int argc, char *argv[])
{       
   char *output = argv[1];   /* output file name */
   char *meaning ="meaning"; /* meaning file name will contain definition and words */

   const char* popular = "/popular.php?character=";	/* const part of http link */
   const char* define = "/define.php?term=";		/* const part of http link */
   char * line = NULL;	/* line contains the words*/

   if (argc < 2) /*no command specified*/
    {
        fprintf(stderr, "Usage: ./program_name output_name...");
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(output, "w+");
    if (!fp) {
        fprintf(stderr, "Failed on output_name");
        exit(EXIT_FAILURE);
    }
    
    char ca2[] = "A";
    char* alpha= &ca2[0];
    int i = 0;

    for(i=0; i < 1; i++) { // 26 letters of the alphabit
	int next_char= 'B'+ i ; // starts at A, next char is B 
       	char* c1 = concat(concat(concat(concat(concat(CURL, WEBSITE), popular), alpha), PIPE_GREP), output);        /* creates the cmd, must be an easier way to do this - .  -)*/    
	*alpha= (char)next_char;
       	system(c1);			
       	free(c1);
    } // output now contains all the popular words
  
    fclose(fp);

    char* c2 = concat("perl -pi -e 's{ \<a.*?\"\> | </a>}{}x' ", output); // reduce to only WORD\n
    system(c2);
    system(c2);	//must run twice
    free(c2);

    concat("perl -pi -e 's{[ ]}{+}x' ", output);
    char* cmd2 = concat("perl -pi -e 's{[ ]}{+}g' ", output); // reduce all whitespace to +
    system(cmd2);
    free(cmd2);

    fp = fopen(output, "r");
    if (!fp) {
        fprintf(stderr, "Failed on opening file");
        exit(EXIT_FAILURE);
    }

    FILE *fp2 = fopen(meaning, "w+");	// 'meaning' file to place the words/definitions

    if (!fp2) {
        fprintf(stderr, "Failed on fp2");
        exit(EXIT_FAILURE);
    }


    size_t len = 0;
    ssize_t symbol;
    while ((symbol = getline(&line, &len, fp)) != EOF) {	   

	char* string = concat(concat(concat(concat("echo \"<div class='meaning'>", line), "</div>\""), " >> "), meaning); 
        line[strcspn(line, "\n")] = 0;	// get rid of endline

	char* c3 = concat(concat(concat(concat(concat(CURL, WEBSITE), define), line), PIPE_SED), meaning); 
	
	system(string);	// cmd$ echo <div... > WORD </div> >> file  (did it like this because of buffering issues)
	free(string);
	
	system(c3);    	// cmd$ curl WEBSITE/define=WORD | sed extract only <div> tags >> file
	free(c3); 
		
   }
   
    fclose(fp); fclose(fp2);	// close all file handlers
    return 0;
}


/*********** HELPER FUNCTION DEFINED BELOW **********************/

   char* concat(char *s1, char *s2) {

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);//+1 for the zero-terminator
    //normally would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);//+1 to copy the null-terminator
    return result;
}






#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define CURL "curl"
#define PIPE_GREP "| grep -i \"<a class=.popular.\" >> "
#define PIPE_SED  "| sed -n \"/<div class='meaning'>/,/</div>/p\""
#define WEBSITE " http://www.urbandictionary.com"


/* 
 * Word Scraping
 */

char* concat(char *s1, char *s2);


int main(int argc, char *argv[])
{       
   char *output = argv[1];   /* output file name */
   char *meaning ="meaning";
   char *format = "format";
   const char* popular = "/popular.php?character=";
   const char* define = "/define.php?term=";
   char * line = NULL;

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
	int next_char= 'B'+ i ; // starts at A so B is next
       	char* c1 = concat(concat(concat(concat(concat(CURL, WEBSITE), popular), alpha), PIPE_GREP), output);        /* creates the cmd, must be an easier way to do this - .  -)*/    
	*alpha= (char)next_char;
       	system(c1);			
       	free(c1);
    } // output now contains all the popular words
 
    fclose(fp);

    char* c2 = concat("perl -pi -e 's{ \<a.*?\"\> | </a>}{}x' ", output); // run this cmd twice to reduce txt to WORD only
    system(c2);
    system(c2);
    free(c2);

    char* cmd2 = concat("perl -pi -e 's{[ ]}{+}x' ", output); // run this cmd for ' ' => + 
    system(cmd2);
    free(cmd2);

    fp = fopen(output, "r");
    if (!fp) {
        fprintf(stderr, "Failed on opening file");
        exit(EXIT_FAILURE);
    }

    FILE *fp2 = fopen(meaning, "w+");	//file for writting out the meaning of the words

    if (!fp2) {
        fprintf(stderr, "Failed on fp2");
        exit(EXIT_FAILURE);
    }


//sed -n '/<div class=.meaning.*>/,/<\/div>/p' FILENAME 


   size_t len = 0;
   ssize_t symbol;
   while ((symbol = getline(&line, &len, fp)) != EOF) {	
    	fprintf(fp2, "%s\n", line);
        line[strcspn(line, "\n")] = 0;
	char* c3 = concat(concat(concat(concat(concat(CURL, WEBSITE), define), line), " >> "), meaning);   

     system(c3);    
     free(c3);
   }
   
    fclose(fp); fclose(fp2);
    return 0;
}



/*********** HELPER FUNCTION DEFINED BELOW **********************/

   char* concat(char *s1, char *s2) {

    size_t len1 = strlen(s1);
    size_t len2 = strlen(s2);
    char *result = malloc(len1+len2+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    memcpy(result, s1, len1);
    memcpy(result+len1, s2, len2+1);//+1 to copy the null-terminator
    return result;
}






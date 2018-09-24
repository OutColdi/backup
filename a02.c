#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <math.h>

// structure to hold entry data from file
typedef struct _entry_t {
	float hotness;
	char *album;
	char *artist;
	float duration;
	char *song;
	int release;
} entry_t;

char *zstring_strtok(char *str, const char *delim) {
    static char *static_str=0;    	/* var to store last address */
    int index=0, strlength=0;      	/* integers for indexes */
    int found = 0;              	/* check if delim is found */

    /* delimiter cannot be NULL
    * if no more char left, return NULL as well
    */
    if (delim==0 || (str == 0 && static_str == 0))
        return 0;

    if (str == 0)
        str = static_str;

    /* get length of string */
    while(str[strlength])
        strlength++;

    /* find the first occurrence of delim */
    for (index=0;index<strlength;index++)
        if (str[index]==delim[0]) {
            found=1;
            break;
        }

    /* if delim is not contained in str, return str */
    if (!found) {
        static_str = 0;
        return str;
    }

    /* check for consecutive delimiters
    *if first char is delim, return delim
    */
    if (str[0]==delim[0]) {
        static_str = (str + 1);
        return (char *)delim;
    }

    /* terminate the string
    * this assignment requires char[], so str has to
    * be char[] rather than *char
    */
    str[index] = '\0';

    /* save the rest of the string */
    if ((str + index + 1)!=0)
        static_str = (str + index + 1);
    else
        static_str = 0;

        return str;
}

// inserts into subject[] at position pos
// char[] original, char insertion character, int position to insert
void append(char subject[], const char insert[], int pos) {
    char buf[100] = {};

    strncpy(buf, subject, pos); 
    int len = strlen(buf);
    strcpy(buf+len, insert); 
    len += strlen(insert);  
    strcpy(buf+len, subject+pos); 

    strcpy(subject, buf);   
}

// Binary search for array of structs
// *arr to sort, size of search, char* searching for
void binsearch(struct _entry_t *arr[10002], int size, char *name){

	int bottom= 0;
	int mid;
	int top = size - 1;
	int i;
	int loop = 0;

	while(bottom <= top){
	    mid = (bottom + top)/2;
	    if (strcmp(arr[mid]->song, name) == 0){
	    	loop = 1;
	        printf("%s performed by %s on album %s\nYear "
	        	"Released: %i | Duration: %f | Hotness: %f\n"
	        	,arr[mid]->song, arr[mid]->artist,arr[mid]->album,arr[mid]->release,
	        	arr[mid]->duration,arr[mid]->hotness);
	        	for(i = 0; i < 10; i++){
	        		mid = mid+1;
	        		if (strcmp(arr[mid]->song, name) == 0){
	        			printf("\n%s performed by %s on album %s\nYear "
	        				"Released: %i | Duration: %f | Hotness: %f\n\n"
	        				,arr[mid]->song, arr[mid]->artist,arr[mid]->album,arr[mid]->release,
	        				arr[mid]->duration,arr[mid]->hotness);
	        				mid+1;
	        		}
	        	}
	        	return;
	    } else if (strcmp(arr[mid]->song, name) > 0){
	        top = mid - 1;
	    } else if (strcmp(arr[mid]->song, name) < 0){
	        bottom = mid + 1;
	    }

	}

	if(loop == 0){
	    printf("Entry not found.\n");
	}

	return;

}

// Sorts array
// _entry_t *arr array of structs to sort
void sort_arr(struct _entry_t *arr[10002]){
	int k, j;
	struct _entry_t temp;
	int max = 10000;

	for (k = 0; k < max; k++)
	{
	    for (j = 0; j < max - k; j++)
	    {
	        if (strcmp(arr[j]->song, arr[j + 1]->song) > 0)
	        {
	            memcpy(&temp, arr[j], sizeof(struct _entry_t));
	            memcpy(arr[j], arr[j + 1], sizeof(struct _entry_t));
	            memcpy(arr[j + 1], &temp, sizeof(struct _entry_t));
	        }
	    }
	}
}

// Reads in csv file and calls sort and search functions
// Features looping interface
int main(int argc, char *argv[]){
	entry_t *arr[10002];
	char buf[4096];
	buf[4095] = '\0';

	FILE *csv_file = fopen("./SongCSV.csv", "r");

	char *saveptr;
	int i = 0;

	// Skips first line of csv
	fgets(buf, 4095, csv_file);

	// Loops and reads in each line to struct
	while(fgets(buf, 4095, csv_file) != NULL){

		arr[i] = malloc(sizeof(entry_t));
		if (NULL == arr[0]) {exit(-1);}

		saveptr = buf;
		char *token = zstring_strtok(saveptr, ",");
		int j = 0;
		for(j = 0; j<3;j++){
			token = zstring_strtok(NULL, ",");
		}

		int length = strlen(token);
		arr[i]->album = malloc(length+1);
		strncpy(arr[i]->album, token, length + 1);

		for(j = 0; j<5;j++){
			token = zstring_strtok(NULL, ",");
		}

		length = strlen(token);
		arr[i]->artist = malloc(length+1);
		strncpy(arr[i]->artist, token, length + 1);

		for(j = 0; j<2;j++){
			token = zstring_strtok(NULL, ",");
		}

		arr[i]->duration = atof(token);

		for(j = 0; j<4;j++){
			token = zstring_strtok(NULL, ",");
		}

		arr[i]->hotness = atof(token);

		for(j = 0; j<3;j++){
			token = zstring_strtok(NULL, ",");
		}

		length = strlen(token);
		arr[i]->song = malloc(length+1);
		strncpy(arr[i]->song, token, length + 1);

		token = zstring_strtok(saveptr, ",");

		arr[i]->release = atof(token);

		i++;
	}


	printf("\nReading and Sorting List...\n");
	printf("-----------------------------\n");

	// Sorts array
	sort_arr(arr);

	// Breaks while loop
	int loop = 0;

	// Loops interface to search
	while(loop == 0){
		char name[100];
		printf("\nEnter song name: ");
		fgets(name, 100, stdin);
		append(name, "\"", 0);
		append(name, "\"", strlen(name+1));
		puts(name);
		char *pos;
		if ((pos=strchr(name, '\n')) != NULL){
		    *pos = '\0';
		}
		if(strcmp(name, "\"ZZZ\"") == 0){
			loop = 1;
			printf("\nExiting program...\n\n");
			return;
		}
		printf("\nSearching...\n");
		printf("------------------------\n");

		binsearch(arr, 10001, name);
	}
	
	return 0;
}
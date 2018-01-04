/* Name : Huong Truong
 * Class : CSCI 2240-003
 * Program # : 4
 * Due Date : Nov 16 2016
 *
 * Honor Pledge: On my honor as a student of the University
 * of Nebraska at Omaha, I have neither given nor received
 * unauthorized help on this homework assignment.
 *
 * NAME: Huong Truong
 * EMAIL: httruong@unomaha.edu
 * NUID: 52233854
 * Colleagues: None
 */


#define _XOPEN_SOURCE 500
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>


/* Fuction Name: stripWhitespace
 * Parameters: char *s
 * Return: void
 * Description: Set strips of new lines, and tabs to one space.
*/
void stripWhitespace(char *s)
{
	int i;
	for(i= strlen(s) - 1; i >= 0 && (s[i] == '\n' || s[i] == '\t'); i--)
	{
		s[i] = '\0';
	}
}

/* Fuction Name: freeWordList
 * Parameters: char **wordlist, int len
 * Return: void
 * Description: deallocate the memory of wordlist
*/
void freeWordList(char **wordlist, int len)
{
	int i;
	for(i = 0; i < len; i ++)
	{
		free(wordlist[i]);
	}
	free(wordlist);
}

/* Fuction Name: ignoreCase
 * Parameters: char c
 * Return: char c
 * Description: convert to lowercase chars
*/
char ignoreCase(char c)
{
	if(c >= 65 && c <= 90)
	{
		return c + 32;
	}
	return c;
}

/* Fuction Name: comparator
 * Parameters: const void *arg1, const void *arg2
 * Return: int
 * Description: Compare 2 strings
*/
int comparator(const void *arg1, const void *arg2)
{
	const char *str1 = *(char * const *)arg1;
	const char *str2 = *(char * const *)arg2;
	int i = 0;
	while(str1[i] != '\0' && str2[i] != '\0')
	{
		if(ignoreCase(str1[i]) > ignoreCase(str2[i]))
		{
			return 1;
		}
		else if(ignoreCase(str1[i]) < ignoreCase(str2[i]))
        {
			return -1;
		}
		i++;
	}
	if(str1[i] != '\0')
	{
		return 1;
	}
	if(str2[i] != '\0')
	{
		return -1;
	}
	return 0;
}

/* Fuction Name: process
 * Parameters: FILE *file, int charCount, char *fileName
 * Return: void
 * Description: The process of arranging the text in file.
*/
void processText(FILE *file, int charCount, char *fileName)
{
	FILE *out;
	char *outName, *outBuffer, *word;
	char inputLine[225];
	int outLen;
	/*fseek start at the beginning of the file regardless of prior processing */
	if(fseek(file, 0, SEEK_SET) < 0) 
	{
		fprintf(stderr, "Encountered error while processing a file\n");
		exit(EXIT_FAILURE);
	}

	/* outName length = length of filename string + 4 bytes for '.out'+
	 * + 1 byte for null terminator*/
	outName = malloc(strlen(fileName) + 5);
	if(outName == NULL)
	{
		fprintf(stderr, "Malloc error\n");
		exit(EXIT_FAILURE);
	}
	/*First character initialized to null for strcat to work*/
	outName[0] = '\0';
	strcat(outName, fileName);
	strcat(outName, ".out");

	out = fopen(outName, "w");
	free(outName);
	if(out == NULL)
	{
		fprintf(stderr, "Error opening a file\n");
		exit(EXIT_FAILURE);
	}
	/*Set outBuffer length to one more than the character limit 
	* so a newline character can be added in an edge case. 
	* Also set first character to null terminator for strcat*/

	outBuffer = malloc(charCount + 1);
	outBuffer[0] = '\0';
	outLen = 0;
	while(fgets(inputLine, 225, file))
	{
		for(word = strtok(inputLine, " ");word != NULL && *word != '\n';
			word = strtok(NULL, " "))
		{
			stripWhitespace(word);
			outLen += strlen(word) + 1; /* + 1 for spacebar character*/
			if(outLen <= charCount + 1)
			{
				strcat(outBuffer, word);
				strcat(outBuffer, " ");
			}
			else {
				/*Exchange last spacebar for newline */
				outBuffer[strlen(outBuffer) - 1] = '\n';
				fprintf(out, "%s", outBuffer);
				outBuffer[0] = '\0';
				strcat(outBuffer, word);
				strcat(outBuffer, " ");
				outLen = strlen(word) + 1;
			}
		}
	}
	/* Write what's left in the buffer */
	outBuffer[strlen(outBuffer) - 1] = '\n';
	fprintf(out, "%s", outBuffer);
	free(outBuffer);
	fclose(out);
}

/* Fuction Name: countText
 * Parameters: FILE *file, char *fileName
 * Return: void
 * Description: count the text and print out in file.
*/


void countText(FILE *file, char *fileName)
{
	FILE *out;
	char *outName, *tok, *prev;
	char **wordlist = malloc(sizeof(char *));
	int wordIdx;
	char inputBuffer[255];
	int i = 1;
	int count;

	if(fseek(file, 0, SEEK_SET) <0)
	{
		fprintf(stderr, "Encountered error while processing a file\n");
		exit(EXIT_FAILURE);
	}
	/* allocate filename length + 6 bytes for '.words', + 1 byte for null*/
	outName = malloc(strlen(fileName) + 7);
	if(outName == NULL)
	{
		fprintf(stderr, "Error allocating memory\n");
		exit(EXIT_FAILURE);
	}
	outName[0] = '\0';
	strcat(outName, fileName);
	strcat(outName, ".words");

	out = fopen(outName, "w");
	free(outName);
	if(out == NULL) 
	{ 
		fprintf(stderr, "Error opening a file\n");
		exit(EXIT_FAILURE);
	}
	wordIdx = 0;
	while(fgets(inputBuffer, 255, file))
	{
		for(tok = strtok(inputBuffer, " "); tok != NULL && *tok != '\n'; tok = strtok(NULL, " "))
		{
			stripWhitespace(tok);
			wordlist = realloc(wordlist, (wordIdx + 1) * sizeof(char *));
			wordlist[wordIdx] = strdup(tok);
			wordIdx++;
		}
	}
	qsort(wordlist, wordIdx, sizeof(char *), comparator);
	prev = wordlist[0];
	count = 1;
	for(i = 1; i < wordIdx; i++)
	{
		if(strcmp(prev, wordlist[i]))
		{
			fprintf(out, "%s - %d\n", prev, count);
			count = 0;
		}
		prev = wordlist[i];
		count++;
	}
	/* write the last word information */
	fprintf(out, "%s - %d\n", prev, count);
	fclose(out);
}

/* Fuction Name: main
 * Parameters: argc, argv for command line arguments
 * Return: 0 for successful execution
 * Description: Implement a rather simple word processor program
*/

int main(int argc, char **argv)
{
	int charCount;
	FILE *fileHandle;
	if(argc < 3)
	{
		fprintf(stderr, "Not enough arguments specified\n");
		exit(EXIT_FAILURE);
	}
	charCount = atoi(argv[1]);
	if(charCount < 25 || charCount > 100) 
	{
		fprintf(stderr, "Wrong character count specified\n");
		exit(EXIT_FAILURE);
	}
	fileHandle = fopen(argv[2], "r");
	if(fileHandle == NULL)
	{
		fprintf(stderr, "Error opening the specified file\n");
		exit(EXIT_FAILURE);
	}
	processText(fileHandle, charCount, argv[2]);
	countText(fileHandle, argv[2]);
	fclose(fileHandle);
	return 0;
}


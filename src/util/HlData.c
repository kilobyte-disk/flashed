/*
 * kilobyte-disk 2025
 * HlData.c
 *
 * Purpose:
 * A utility that reads and writes Helium formatted data.
 *
 * [ Conventions for file.hld ]
 * 
 * # Comments begin with a '#'
 * Comments can ONLY be on their OWN line.
 * 
 * data|data
 * foo|bar
 *
 * Data is stored in key-pair strings.
 * Calling DATA_ReadHlData(); will return a 2D array like so:
 * parsed[n][n][n] = {
 * 	{ "data", "data" };
 * 	{ "foo", "bar" };
 * };
 *
 * That's it! Very simple!
 * =======================
 *
 *
 */

#define MAX_CP_LEN 15
#define MAX_LINE_LENGTH 250
#define MAX_HLDATA_LENGTH 500
#define MAX_ARG_LENGTH 1000

#define SPLIT_CHAR '|'
#define COMMENT_CHAR '#'

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HlData.h"

/* Data utils */
unsigned int GetFileLen(FILE *fptr)
{
	unsigned int counter = 0;
	char dummy_buffer[4];

	while (fgets(dummy_buffer, 4, fptr)) {
		counter++;
	}

	rewind(fptr);

	printf("[HlData]: File is %i lines long.\n", counter);

	return counter;
}

unsigned int GetDeckLen(FILE *fptr)
{
	unsigned int counter = 0;
	char dummy_buffer[4];

	while (fgets(dummy_buffer, 4, fptr)) {
		/* Ignore comment lines, blank lines, and newlines */
		char l0 = dummy_buffer[0];
		if ((l0 == COMMENT_CHAR) || (l0 == '\0') || (l0 == '\n')) {
			continue;
		}

		counter++;
	}

	rewind(fptr);

	printf("[HlData]: Deck contains %i items.\n", counter);
	
	return counter;
}


/*
 * Parse an .hld file into a 2D array
 * containing key-value pairs of strings.
 * (read header for more info)
 */
struct HlData DATA_ReadHlData(char *path)
{
	/* Load file */
	FILE *file = fopen(path, "r");

	if (file == NULL) {
		printf("[HlData]: (FATAL) Could not read file at path:\n");
		printf("%s\n", path);

		return (struct HlData) { 0 };
	}

	char line[MAX_LINE_LENGTH];
	memset(line, 0, MAX_LINE_LENGTH);

	char *data[MAX_HLDATA_LENGTH][2];
	int line_counter = -1;
	int element_counter = 0;

	/* Init struct */
	struct HlData hldata;

	hldata.elements = 0;

	while (fgets(line, MAX_LINE_LENGTH, file)) {
		line_counter++;

		/* Ignore comment lines, blank lines, and newlines */
		char l0 = line[0];
		if ((l0 == COMMENT_CHAR) || (l0 == '\0') || (l0 == '\n')) {
			continue;
		}

		/* Begin parsing of the line */
		char arg1[MAX_ARG_LENGTH];
		char arg2[MAX_ARG_LENGTH];
		memset(arg1, 0, MAX_ARG_LENGTH);
		memset(arg2, 0, MAX_ARG_LENGTH);

		int line_length = strlen(line);
	
		/* line_char, write_char */
		int lc = 0;
		int wc = 0;

		/* Parse first argument */
		while ((line[lc] != SPLIT_CHAR) && (line[lc] != '\0') && (lc < line_length)) {
			arg1[wc] = line[lc];

			lc++;
			wc++;
		}
		
		arg1[wc + 1] = '\0';

		/* Skip second arg if we already made it to the end of the line */
		if ((line[lc] == '\0') || (lc >= line_length)) {
			printf("[HlData]: (ERROR) Could not properly parse data line! Missing a '|' split at line %i\n", line_counter);
			printf("[HlData]: Offending line: %s\n", line);
			continue;
		}

		/* Jump over SPLIT_CHAR to the next char */
		lc++;
		wc = 0;

		while ((line[lc] != '\0') && (line[lc] != '\n') && (lc < line_length)) {
			arg2[wc] = line[lc];

			lc++;
			wc++;
		}

		arg2[wc + 1] = '\0';

		/* Write to each char buffer */
		strncpy(hldata.data[element_counter][0], arg1, sizeof(arg1));
		strncpy(hldata.data[element_counter][1], arg2, sizeof(arg2));
		
		/* Flush line buffer */
		memset(line, 0, MAX_LINE_LENGTH);
		element_counter++;
	}

	fclose(file);

	hldata.elements = element_counter;

	printf("[HlData]: Successfully parsed .hll file at '%s'\n", path);

	return hldata;
}

/*
 * Parse an .hdk file into a 2D array
 * containing key-value pairs of strings.
 * 
 */
struct HlDeck DATA_ReadHlDeck(char *path)
{
	/* Load file */
	FILE *file = fopen(path, "r");

	if (file == NULL) {
		printf("[HlData]: ERROR: Could not read file at path:\n");
		printf("%s\n", path);
	}

	char line[MAX_LINE_LENGTH];
	memset(line, 0, MAX_LINE_LENGTH);

	char *data[MAX_HLDATA_LENGTH][2];
	int line_counter = -1;
	int element_counter = 0;

	/* Init struct */
	struct HlDeck hldeck;

	/* Allocate memory */
	unsigned int file_len = GetDeckLen(file);
	size_t array_size = sizeof(struct HlDeckCard) * file_len;

	struct HlDeckCard *cardsptr = malloc(array_size);
	if (cardsptr == NULL) {
		printf("[HlData]: FATAL: malloc fail\n");
	}

	hldeck.cards = cardsptr;

	hldeck.elements = 0;

	while (fgets(line, MAX_LINE_LENGTH, file)) {
		line_counter++;

		/* Ignore comment lines, blank lines, and newlines */
		char l0 = line[0];
		if ((l0 == COMMENT_CHAR) || (l0 == '\0') || (l0 == '\n')) {
			continue;
		}

		/* Begin parsing of the line */
		char arg1[MAX_ARG_LENGTH];
		char arg2[MAX_ARG_LENGTH];
		memset(arg1, 0, MAX_ARG_LENGTH);
		memset(arg2, 0, MAX_ARG_LENGTH);

		int line_length = strlen(line);
	
		/* line_char, write_char */
		int lc = 0;
		int wc = 0;
		int len1 = 1;
		int len2 = 1;

		/* Parse first argument */
		while ((line[lc] != SPLIT_CHAR) && (line[lc] != '\0') && (lc < line_length)) {
			arg1[wc] = line[lc];

			lc++;
			wc++;
			len1++;
		}
		
		arg1[wc + 1] = '\0';

		/* Skip second arg if we already made it to the end of the line */
		if ((line[lc] == '\0') || (lc >= line_length)) {
			printf("[HlData]: (ERROR) Could not properly parse data line! Missing a '|' split at line %i\n", line_counter);
			printf("[HlData]: Offending line: %s\n", line);
			continue;
		}

		/* Jump over SPLIT_CHAR to the next char */
		lc++;
		wc = 0;

		while ((line[lc] != '\0') && (line[lc] != '\n') && (lc < line_length)) {
			arg2[wc] = line[lc];

			lc++;
			wc++;
			len2++;
		}

		arg2[wc + 1] = '\0';

		/* Allocate each buffer */
		char *frontbuf = malloc(sizeof(arg1));
		char *backbuf = malloc(sizeof(arg2));

		if (frontbuf == NULL || backbuf == NULL) {
			printf("[HlData]: FATAL: malloc fail\n");
		}

		struct HlDeckCard card;

		card.front = frontbuf;
		card.back = backbuf;

		hldeck.cards[element_counter] = card;

		/* Write to each char buffer */
		strncpy(frontbuf, arg1, sizeof(arg1));
		strncpy(backbuf, arg2, sizeof(arg2));
		
		/* Flush line buffer */
		memset(line, 0, MAX_LINE_LENGTH);
		element_counter++;
	}

	fclose(file);

	hldeck.elements = element_counter;

	printf("[HlData]: Successfully parsed .hdk file at '%s'\n", path);

	return hldeck;
}



/*
 * Generate codepoints from specified start hex to end hex
 */
struct HlCodepoints DATA_GenCodepoints(unsigned int start, unsigned int end)
{
	printf("[HlData]: Generating codepoints from %X -> %X\n", start, end);

	struct HlCodepoints data;

	data.codepoints = calloc(abs(start - end), sizeof(int));
	if (data.codepoints == NULL) {
		printf("[HlData]: FATAL: malloc fail\n");
	}

	int count_direction = 1;
	if (start > end) {
		printf("[HlData]: Counting backwards!\n");
		count_direction = -1;
	}

	unsigned int counter = start;
	unsigned int lines = 0;

	while (start != end) {
		data.codepoints[lines] = counter;

		counter += count_direction;
		lines++;
	}

	printf("[HlData]: Generated %i codepoints.", lines);
	data.count = lines;

	return data;
}




/*
 * Parse an .hlcodept file into a
 * HlCodepoints struct
 */
struct HlCodepoints DATA_ReadCodepoints(char *path)
{
	FILE *file = fopen(path, "r");

	size_t hexstr_max = MAX_CP_LEN;

	char line[hexstr_max];
	unsigned int elements = 0;

	char codept_str[hexstr_max];
	memset(codept_str, 0, hexstr_max);

	struct HlCodepoints data;

	unsigned int file_len = GetFileLen(file);

	int *cptr = calloc(file_len, sizeof(int));
	data.codepoints = cptr;
	if (cptr == NULL) {
		printf("[HlData]: FATAL: malloc fail\n");
	}

	while (fgets(line, hexstr_max, file)) {
		/* Skip newlines, empty lines, and comments */
		char l0 = line[0];
		if ((l0 == COMMENT_CHAR) || (l0 == '\0') || (l0 == '\n')) {
			continue;
		}

		
		/* 
		 * Write line to a new buffer to strip it
		 * of trailing newlines and other garbage
		 */
		int i = 0;
		int lnlen = strlen(line);

		/* Insert hexadecimal prefix */
		codept_str[0] = '0';
		codept_str[1] = 'x';	

		while ((line[i] != '\0') && (line[i] != '\n') && (i < lnlen)) {
			codept_str[i + 2] = line[i];
			
			i++;
		}

		codept_str[i + 2] = '\0';

		//printf("%s\n", codept_str);

		/* Convert to hexadecimal */
		int codepoint = (int) strtol(codept_str, NULL, 16);

		if (codepoint > 0) {
			data.codepoints[elements] = codepoint;
		}

		memset(codept_str, 0, hexstr_max);
		elements++;
	}

	fclose(file);

	data.count = elements;

	printf("[HlData]: Successfully loaded codepoints at '%s'\n", path);

	return data;
}

void DATA_AppendCodepoints(struct HlCodepoints *destination, struct HlCodepoints *codepoints)
{
	unsigned int add_len = codepoints->count;
	unsigned int dest_len = destination->count;

	if ((dest_len + add_len) > MAX_CODEPOINTS) {
		printf(
			"[HlData]: Failed to combine codepoints! Exceeds %u limit!\n",
			MAX_CODEPOINTS
		);
	}

	/* Add 2 to the lengths to account for the offset from lengths starting at zero. */
	size_t new_size = sizeof(int) * (dest_len + add_len + 2);
	printf(
		"[HlData]: Attempting realloc, new size: %u (%u elements).\n",
		new_size,
		(add_len + dest_len)
	);

	int *newptr = realloc(destination->codepoints, new_size);
	if (newptr == NULL) {
		printf("[HlData]: FATAL: Unable to allocate more memory (AppendCodepoints).\n");
	}

	destination->codepoints = newptr;

	unsigned int i = 0;

	for (i = 0; i <= add_len; i++) {
		destination->codepoints[dest_len + i] = codepoints->codepoints[i];
	}

	destination->count = (dest_len + add_len);
}

void DATA_FreeCodepoints(struct HlCodepoints *data)
{
	printf("[HlData]: Freeing codepoints (%li bytes)\n", (sizeof(int) * data->count));

	data->count = 0;

	free(data->codepoints);
}



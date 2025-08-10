#ifndef HLFILEPARSE
#define HLFILEPARSE

#define MAX_CODEPOINTS 65535 /* uint max limit */

#define MAX_LINE_LENGTH 250
#define MAX_HLDATA_LENGTH 500
#define MAX_ARG_LENGTH 1000

struct HlData {
	unsigned int elements;
	char data[MAX_HLDATA_LENGTH][2][MAX_ARG_LENGTH]; 
};

struct HlDeck {
	long int elements;

	char **front;
	char **back;
};


struct HlCodepoints {
	unsigned int count;
	int *codepoints;
};

extern struct HlData DATA_ReadHlData(char *path);
extern struct HlDeck DATA_ReadHlDeck(char *path);
extern struct HlCodepoints DATA_ReadCodepoints(char *path);
extern void DATA_AppendCodepoints(struct HlCodepoints *destination, struct HlCodepoints *codepoints);
extern void DATA_FreeCodepoints(struct HlCodepoints *data);


#endif

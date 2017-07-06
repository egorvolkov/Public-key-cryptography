#include "receiver.h"

extern const uchar size;
extern struct Module moduleStruct;

void transmitterConnection() {
	remove(PATH_TO_THE_SECOND_FLAG);

	FILE* file;
	file = fopen(PATH_TO_THE_FIRST_FLAG, "wb");
	if (!file) {
		printf("Can't open the file for setting a flag\n");
		return;
	}
	fclose(file);

	while (1) {
		file = fopen(PATH_TO_THE_SECOND_FLAG, "r");
		if (file)
			break;
	}
	fclose(file);
	remove(PATH_TO_THE_SECOND_FLAG);
}
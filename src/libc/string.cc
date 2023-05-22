#include "./string.h"

int strcmp(char * str1, char * str2) {
	for (int i = 0;;i++) {
		if (str1[i] < str2[i])
			return -1;
		else if (str1[i] > str2[i])
			return 1;
		else if (str1[i] == 0)
			return 0;
	}
}

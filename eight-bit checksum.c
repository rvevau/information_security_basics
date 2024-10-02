#include <stdio.h>

int main()
{
	FILE* file = fopen("summ.txt", "r");
	int C = 0, T = 0, M = (255 + 1);
	char ch;
	while ((ch = fgetc(file)) != EOF) {
		T += ch;
	}
	fclose(file);
	C = T % M;
	printf("%d", C);
	return (0);
}


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <math.h>



struct set {
    char letter[9000];
    int code;
};

struct set dictionary[100000];

void add_new_word(char* current_string, int index) { 
    dictionary[index].code = index;
    strcpy(dictionary[index].letter, current_string);
}


int find_word(char* current_string, int size) { 
    for (int i = 0; i < size; i++) {
        if (strcmp(current_string, dictionary[i].letter) == 0) return i;
    }
    return -1;
}

char* find_code(int current_code, int size, char* current_letter) { 
    for (int i = 0; i < size; i++) {
        if (current_code == dictionary[i].code) {
            strcpy(current_letter, dictionary[i].letter);
            return current_letter;
        }
    }
    return 0;
}



int compress(FILE* input, FILE* output, double file_size) {
    clock_t begin = clock();

    int size = 0; 
    for (int i = 0; i < 256; i++) { 
        unsigned char ch = (unsigned char)i;
        char string[] = { ch, '\0' };
        add_new_word(string, i);
        size++;
    }

    char string[100000];
    char sym[2];
    sym[1] = '\0';
    string[1] = '\0';

    string[0] = fgetc(input);
    while (!feof(input)) {
        sym[0] = fgetc(input);
        char new_string[100000] = { '\0' };
        strcpy(new_string, string);
        if (find_word(strcat(new_string, sym), size) != -1) {
            strcpy(string, new_string);
        }
        else {
            int code = find_word(string, size);
            fprintf(output, "%d ", code);
            file_size += ceil(log2(code)); 
            add_new_word(strcat(string, sym), size);
            size++;
            string[0] = sym[0];
            string[1] = '\0';
        }
    }
    clock_t end = clock();
    printf("Время кодирования: %f секунд\n", (double)(end - begin) / CLOCKS_PER_SEC);
    return file_size;
}



void decompress(FILE* input, FILE* output) {
    clock_t begin = clock();
    time_t t0 = time(0);
    clock_t start_time = clock();

    int size = 0;
    for (int i = 0; i < 256; i++) { 
        unsigned char ch = (unsigned char)i;
        char string[] = { ch, '\0' };
        add_new_word(string, i);
        size++;
    }

    int code[100000];
    int counter = 0;
    for (int k = 0; !feof(input); k++) {
        fscanf(input, "%d", &code[k]);
        counter++;
    }

    int old = code[0];
    int n;
    char string[100000] = { '\0' }, sym[100000] = { '\0' };
    find_code(old, size, string);
    strcpy(sym, string);
    fputs(sym, output);

    for (int i = 0; i < counter - 1; i++) {
        n = code[i + 1];
        if (find_code(n, size, string) == 0) {
            strcpy(string, dictionary[old].letter);
            strcat(string, sym);
        }
        else {
            strcpy(string, dictionary[n].letter);
        }

        fputs(string, output);
        sym[0] = '\0';
        string[1] = '\0';
        strcpy(sym, string);
        char new_string[100000] = "";
        strcpy(new_string, dictionary[old].letter);
        strcat(new_string, sym);
        add_new_word(new_string, size);
        size++;
        old = n;
    }
    clock_t end = clock();
    time_t t1 = time(0);
    double time_in_seconds = difftime(t1, t0);
    printf("Время декодирования: %f секунд\n", (double)(end - begin) / CLOCKS_PER_SEC);
    printf("Время декодирования через time_t: %f секунд\n", time_in_seconds);

    clock_t end_time = clock();
    double elapsed_time_ms = (double)(end_time - start_time) * 1000 / CLOCKS_PER_SEC;
    printf("Elapsed time: %.2f milliseconds\n", elapsed_time_ms);

}


int main()
{
    setlocale(LC_ALL, "Rus");
    srand(time(NULL));
    printf("1 - кодирование\n");
    printf("2 - декодирование\n");
    int n;
    scanf("%d", &n);

    FILE* code_input = fopen("code_input.txt", "r+");
    FILE* code_output = fopen("code_output.txt", "w");
    FILE* decode_input = fopen("decode_input.txt", "r+");
    FILE* decode_output = fopen("decode_output.txt", "w");

    int before = 0;
    double after = 0;

    if (n == 1) {
        after = compress(code_input, code_output, after);
        fseek(code_input, 0L, SEEK_END);
        before = ftell(code_input) * 8; 
        printf("Коэффициент сжатия: %f\n", (after / (double)before));
    }

    if (n == 2) {
        decompress(decode_input, decode_output);
    }

    fclose(code_input);
    fclose(code_output);
    fclose(decode_input);
    fclose(decode_output);
}





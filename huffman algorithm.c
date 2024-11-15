#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <locale.h>
#include <time.h>

struct table { 
    unsigned int symbol;
    char letter; 
    char coded[30];
    struct table* next;
};


struct array_of_symbols { 
    unsigned int symbol; 
    int counter; 
    char coded; 
    struct array_of_symbols* next;
    struct array_of_symbols* prev;
    struct array_of_symbols* right;
    struct array_of_symbols* left;
};


void sort(struct array_of_symbols* head) { 
    struct array_of_symbols* pointer = head->next;
    while (pointer) {
        struct array_of_symbols* next = pointer->next, * prev = pointer->prev;
        while (prev && pointer->counter < prev->counter) prev = prev->prev;
        if (pointer->prev) pointer->prev->next = pointer->next;
        if (pointer->next) pointer->next->prev = pointer->prev;
        if (prev) {
            pointer->next = prev->next;
            pointer->prev = prev;
            if (prev->next) prev->next->prev = pointer;
            prev->next = pointer;
        }
        else {
            pointer->next = head->next;
            head->next->prev = pointer;
            head->next = pointer;
            head->next->prev = NULL;
        }
        pointer = next;
    }
}

void fill_table(struct array_of_symbols* root, struct array_of_symbols* current_knot, char decoded[], struct table* start) {
    if (current_knot) {
        if (current_knot != root) {
            strncat(decoded, &current_knot->coded, 1);
            decoded[strlen(decoded)] = '\0';
        }
        if (current_knot->symbol > 0) {
            if (start->next == NULL) {
                struct table* new_knot = (struct table*)malloc(sizeof(struct table));
                start->next = new_knot;
                new_knot->next = NULL;
                new_knot->symbol = current_knot->symbol;
                new_knot->letter = new_knot->symbol;
                strcpy(&new_knot->coded, decoded);
                decoded[strlen(decoded) - 1] = '\0';
            }
            else {
                struct table* current = start->next;
                struct table* last = start;
                while (current->symbol != current_knot->symbol && current->next != NULL) {
                    current = current->next;
                    last = last->next;
                }
                if (current->next == NULL && current->symbol != current_knot->symbol) {
                    struct table* new_knot = (struct table*)malloc(sizeof(struct table));
                    current->next = new_knot;
                    new_knot->next = NULL;
                    new_knot->symbol = current_knot->symbol;
                    new_knot->letter = new_knot->symbol;
                    strcpy(&new_knot->coded, decoded);
                    decoded[strlen(decoded) - 1] = '\0';
                }
            }
        }
        fill_table(root, current_knot->left, decoded, start);
        if (current_knot == root) decoded[0] = '\0';
        fill_table(root, current_knot->right, decoded, start);
        if (current_knot->symbol == 0 && current_knot != root) decoded[strlen(decoded) - 1] = '\0';
    }
}


void decode_alphabet(struct array_of_symbols* head, struct table* start) {
    struct array_of_symbols* first = (struct array_of_symbols*)malloc(sizeof(struct array_of_symbols));
    FILE* input = fopen("input_to_decode.txt", "r");
    head->next = first;
    first->prev = head;
    first->next = NULL;
    char c = fgetc(input);
    while (!feof(input)) { 
        struct array_of_symbols* pointer = head->next;
        while (pointer->next != NULL) {
            if (pointer->symbol == c) { pointer->counter++; break; }
            else pointer = pointer->next;
        }
        if (pointer->next == NULL) {
            if (pointer->symbol == c) pointer->counter++;
            else {
                struct array_of_symbols* new_knot = (struct array_of_symbols*)malloc(sizeof(struct array_of_symbols));
                new_knot->symbol = c;
                new_knot->counter = 1;
                new_knot->next = NULL;
                new_knot->prev = pointer;
                pointer->next = new_knot;
                new_knot->right = NULL;
                new_knot->left = NULL;
            }
        }
        c = fgetc(input);
    }
    head = head->next;
    sort(head);
    while (head->next->next != NULL) {
        struct array_of_symbols* NEW_KNOT = (struct array_of_symbols*)malloc(sizeof(struct array_of_symbols));
        NEW_KNOT->symbol = 0;
        struct array_of_symbols* OLD_RIGHT = head->next->next;
        struct array_of_symbols* OLD_LEFT = head->next;
        NEW_KNOT->prev = OLD_LEFT->prev;
        NEW_KNOT->next = OLD_RIGHT->next;
        OLD_RIGHT->prev = NEW_KNOT;
        OLD_LEFT->prev->next = NEW_KNOT;
        NEW_KNOT->left = OLD_LEFT;
        NEW_KNOT->left->coded = '0';
        NEW_KNOT->right = OLD_RIGHT;
        NEW_KNOT->right->coded = '1';
        NEW_KNOT->left->next = NULL;
        NEW_KNOT->left->prev = NULL;
        NEW_KNOT->right->next = NULL;
        NEW_KNOT->right->prev = NULL;
        NEW_KNOT->counter = NEW_KNOT->left->counter + NEW_KNOT->right->counter;
        sort(head);
    }
    struct array_of_symbols* root = head->next;
    char decoded[30] = { '0' };
    decoded[0] = '\0';
    fill_table(root, root, decoded, start);
    fclose(input);
}


void decode(struct table* start) {
    FILE* input = fopen("input_to_decode.txt", "r");
    FILE* output = fopen("output_decoded.txt", "w");
    char C = fgetc(input);
    while (!feof(input)) {
        struct table* current = start->next;
        while (current) {
            if (current->symbol == C) {
                fprintf(output, "%s", current->coded);
                break;
            }
            current = current->next;
        }
        C = fgetc(input);
    }
    fclose(output);
}


void encode(struct array_of_symbols* root) {
    FILE* input = fopen("output_decoded.txt", "r");
    FILE* output = fopen("encoded.txt", "w");
    char c = fgetc(input);
    struct array_of_symbols* current_knot = root;
    while (!feof(input)) {
        if (c == '0') {
            if (current_knot->left) current_knot = current_knot->left;
        }
        else if (c == '1') {
            if (current_knot->right) current_knot = current_knot->right;
        }
        if (current_knot->symbol > 0) {
            fprintf(output, "%c", current_knot->symbol);
            current_knot = root;
        }
        c = fgetc(input);
    }
}


int main() {
    setlocale(LC_ALL, "ru");
    struct array_of_symbols* head = (struct array_of_symbols*)malloc(sizeof(struct array_of_symbols));
    struct table* start = (struct table*)malloc(sizeof(struct table));
    start->next = NULL;
    struct array_of_symbols* root;
    double START, END, TOTALTIME;
    int choice;
    printf("Choose:\nDecode - 1\nEncode - 2\n\n--> ");
    scanf("%d", &choice);
    FILE* input_to_decode = fopen("input_to_decode.txt", "r");
    FILE* output_decoded = fopen("output_decoded.txt", "r");



    fclose(input_to_decode);
    fclose(output_decoded);

    if (choice == 1) {
        START = clock();
        decode_alphabet(head, start);
        decode(start);
        END = clock();
        TOTALTIME = (END - START) / 1000;
    }
    else if (choice == 2) {
        decode_alphabet(head, start);
        root = head->next->next;
        START = clock();
        encode(root);
        END = clock();
        TOTALTIME = (END - START) / 1000;
    }
    printf("Потраченное время - %F\n\n", TOTALTIME);


    return 0;
}
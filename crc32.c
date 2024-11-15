#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <malloc.h>


void codesum() {

    FILE* filec = fopen("text.txt", "r");
    int c = 0;
    char simb = fgetc(filec);
    unsigned long t = 0;
    while (simb != EOF) {
        t += simb;
        simb = fgetc(filec);
    }

    fclose(filec);

    c = t % (256); 
    printf("%ld", c);
}

void crcpolinomdiv() {
    FILE* text = fopen("text.txt", "r");
  
    int deviver[] = { 1,1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1 };

    int sim = getc(text); 
    int textlen = 0;
    while (sim != EOF) { 
        textlen++;
        sim = getc(text);
    }


    long* bintext = (long*)calloc((textlen * 8) + 32, sizeof(int)); 
    unsigned long bintextlen = (textlen * 8) + 32;
    int k = 7, j = 1; 
    fseek(text, 1, SEEK_SET);
    fseek(text, -1, SEEK_CUR);
    for (int i = 0; i < textlen; i++) {
        sim = getc(text);
        while (sim != 0) {
            bintext[k] = sim % 2;
            sim /= 2;
            k--;
        }
        j++; 
        k = (8 * j) - 1;
    }
    fclose(text);
    
    int remainder[33] = { 0 }; 
    for (int i = 0; i < 33; i++) {
        remainder[i] = bintext[i];
    }
    long save = 0, itersave = 0;
    int lenremainder = 0;
    while ((bintextlen - save) >= 33) { 
        for (int j = 0; j < 33; j++) { 
            if (remainder[j] == deviver[j]) remainder[j] = 0;
            else remainder[j] = 1;
        }

        while (remainder[itersave] == 0) {
            save++;
            itersave++;

        }
        int i = 0;
        for (i; i < (33 - itersave); i++) {
            remainder[i] = remainder[i + itersave];
        }

        for (i; i < 33; i++) {
            remainder[i] = bintext[i + save];
        }

        if ((save + 33) >= bintextlen) {  
            lenremainder = 33 - ((save + 33) - bintextlen);
        }
        itersave = 0;
    }
    

    printf("Bin: ");

    for (int h = 0; h < lenremainder; h++) {
        printf("%d", remainder[h]);
    }



    unsigned long long dec = 0;
    int deg = lenremainder - 1;
    for (int i = 0; i < lenremainder; i++) {
        dec += pow(2, deg) * remainder[i];
        deg--;
    }
    printf("\nDec: ");
    printf("%llu", dec);
    printf("\nHex: ");
    printf("%llX", dec);
    free(bintext);
}

void crcbitsdvig() { 
    FILE* text = fopen("text.txt", "r");
    int deviver[] = { 1,1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,1 }; 

    int sim = getc(text);
    int textlen = 0;
    while (sim != EOF) {
        textlen++;
        sim = getc(text);
    }
    int* bintext = (int*)calloc((textlen * 8) + 32, sizeof(int));
    unsigned long bintextlen = (textlen * 8) + 32;
    int k = 7, j = 1;
    fseek(text, 1, SEEK_SET);
    fseek(text, -1, SEEK_CUR);
    for (int i = 0; i < textlen; i++) {
        sim = getc(text);
        while (sim != 0) {
            bintext[k] = sim % 2;
            sim /= 2;
            k--;
        }
        j++;
        k = (8 * j) - 1;
    }
    fclose(text);
    int rgstr[32] = { 0 }; 
    int saveind = 0;
    int out = 0;

    for (int i = 0; i < 32; i++) { 
        rgstr[i] = bintext[i];
        saveind++;
    }
    while (bintextlen > saveind) {
        out = rgstr[0];
        for (int j = 0; j < 31; j++) {
            rgstr[j] = rgstr[j + 1];
        }
        rgstr[31] = bintext[saveind]; 
        saveind++;
        if (out == 1) {
            for (int i = 0; i < 32; i++) {
                if (rgstr[i] == deviver[i + 1]) rgstr[i] = 0;
                else rgstr[i] = 1;

            }
        }
        else continue;
    }

    printf("Bin: ");
    for (int h = 0; h < 32; h++) {
        printf("%d", rgstr[h]);
    }
    unsigned long long dec = 0;
    int deg = 32 - 1;
    for (int i = 0; i < 32; i++) {
        dec += pow(2, deg) * rgstr[i];
        deg--;
    }
    printf("\nDec: ");
    printf("%llu", dec);
    printf("\nHex: ");
    printf("%llX", dec);
    free(bintext);



}


int main() {
    printf("Choose function:\n1 - codesum\n2 - crcpolinomdiv\n3 - crcbitsdvig\n");
    int N;
    scanf("%d", &N);
    switch (N) {

    case 1:
        codesum();
        break;
    case 2:
        crcpolinomdiv();
        break;
    case 3:
        crcbitsdvig();
        break;
    default:
        printf("Error");
    }
}

//Encryption and decryption utility using the Merkle-Hellman algorithm
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int len_of_bits;

int bin2dec(int n)
{
    int num = n;
    int dec_value = 0;
    int base = 1;
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
        dec_value += last_digit * base;
        base = base * 2;
    }
    return dec_value;
}

int dec2bin(int num)
{
    int bin = 0, k = 1;
    if (num >= 0) {
        while (num)
        {
            bin += (num % 2) * k;
            k *= 10;
            num /= 2;
        }
        return bin;
    }
}

int encrypt(char text, int open_key[10]) {
    int letters = dec2bin(int(text));
    char string_of_bins[100];
    string_of_bins[0] = '\0';
    _itoa_s(letters, string_of_bins, 10);    
    int message = 0;
    len_of_bits = strlen(string_of_bins);
    int j = 0;
    for (int i = 0; i < len_of_bits; i++) {
        if (string_of_bins[i] == '1') {
            message += open_key[j];
            if (j == 9) {
                j = 0;
            }
            else {
                j++;
            }
        }
        else {
            if (j == 9) {
                j = 0;
            }
            else {
                j++;
            }
        }
    }
    return message;
}


char decrypt(int message, int close_key[10], int r, int q) {
    int n_1;
    for (n_1 = 0; n_1 < 1000; n_1++) {
        if (((n_1 * r) % q) == 1) {
            break;
        }
    }

    int* almost_done = (int*)malloc(sizeof(int) * len_of_bits);
    for (int i = 0; i < len_of_bits; i++) {
        almost_done[i] = 0;
    }

    int decoded_message = (message * n_1) % q;
    int curr_max = 0;
    int index = 0;
    while (decoded_message > 0) {
        for (int i = 0; i < len_of_bits; i++) {
            if (close_key[i] <= decoded_message && close_key[i] > curr_max) {
                curr_max = close_key[i];
                index = i;
            }
        }
        decoded_message -= curr_max;
        curr_max = 0;
        almost_done[index] = 1;
    }
    char* sdecoded_message = (char*)malloc(sizeof(char) * len_of_bits);
    sdecoded_message[0] = '\0';
    for (int i = 0; i < len_of_bits; i++) {
        char buffer[10];
        _itoa_s(almost_done[i], buffer, 10);
        strcat(sdecoded_message, buffer);
    }
    int dec_decoded_message = 0;
    dec_decoded_message = bin2dec(atoi(sdecoded_message));
    char final_decoded_message = char(dec_decoded_message);
    return char(final_decoded_message);
}

void open_key_generation(int close_key[10], int q, int r, int open_key[10]) {

    int tmp = 0;
    for (int i = 0; i < 10; i++) {
        tmp = (r * close_key[i]) % q;
        open_key[i] = tmp;
    }
}

int main(int argc, const char* argv[]) {
    int open_key[10] = {};
    int close_key[10] = { 1, 2, 7, 13, 67, 91, 182, 370, 734, 1468 }; // Самостоятельно задаем закрытый ключ
    int sum_key = 2935; // Сумма элементов закрытого ключа
    int q = 3329; // Произвольное простое число большее суммы
    int r = 4139; // Произвольное простое число, взаимно простое с q
    char text[100]; // Сообщение для шифровки
    printf("Enter text to decode: ");
    gets_s(text);
    open_key_generation(close_key, q, r, open_key); // Генерируем открытый ключ
    printf("Encrypted message: ");
    for (int i = 0; i < strlen(text); i++) {
        int message = encrypt(text[i], open_key);
        printf("%d ", message);
    }
    printf("\n");
    printf("Decrypted message: ");
    for (int i = 0; i < strlen(text); i++) {
        int message = encrypt(text[i], open_key);
        printf("%c", decrypt(message, close_key, r, q));
    }
    printf("\n");
    return 0;   
}

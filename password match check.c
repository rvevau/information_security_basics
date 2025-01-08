#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
char kod[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // алфавит base64
unsigned int index(char a) {
	for (size_t i = 0; i < strlen(kod); i++)
	{
		if (a == kod[i])
		{
			return i;    // возвращает индекс символа из алфавита base64
		}
	}
	return 0;
}
void print_mass(char* arr, int len) {
	for (size_t i = 1; i < len; i++)
	{
		printf("%c", arr[i]);    // выводит массив
	}
}
void dec_to_bin(unsigned int n, size_t* arr, unsigned int l_place) {
	for (size_t i = 0; i < 6; i++)
	{
		arr[l_place - i] = n % 2; // переводит число из двоичной сс в десятичную сс
		n /= 2;
	}
}
unsigned int bin_to_dec(size_t* bin_s, unsigned int l_place) {
	unsigned int sum = 0;
	for (size_t i = 0; i < 8; i++) // переводит число из двоичной сс в десятичную, используя формулу суммы произведений каждого бита на соответствующую степень двойки
	{
		sum += bin_s[l_place - i] * pow(2, i);
	}
	return sum;
}
int main() {
	setlocale(LC_ALL, "rus");
	char s[1000], decod[1000];
	size_t bin_s[1000] = { 0 };
	unsigned int len;
	printf("Введите строку: ");
	scanf("%s", s);
	for (size_t i = 1; i <= strlen(s); i++)
	{
		int dec; // каждый символ из массива "s" преобразуется в десятич число, которое затем преобразуется в двоичный формат и сохраняется в массиве "bin_s"
		dec = index(s[i - 1]);
		dec_to_bin(dec, bin_s, i * 6 - 1);
	}
	for (size_t i = 1; i <= strlen(s); i++)
	{
		decod[i] = bin_to_dec(bin_s, i * 8 - 1); // каждые 8 бит из массива "bin_s" преобразуются в десятичное число с помощью функции "bin_to_dec" и сохраняются в массиве "decod".
	}
	printf("Расшифрованный пароль: ");
	print_mass(decod, strlen(decod));
	return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void final_permutation(char *array);
void str_to_hex(char *str, char **hex);
void hex_to_binary(char **hex, char *b);
void binary_to_hex(char *b, char **hex, int binary_size);

void ip(char *array);
void pc1(char *key64, char *key56);

char get_hex_single(char *half_byte);
char* get_half_byte(char hex);


int main()
{
	unsigned char text[8];
	unsigned char key[8];

	char text_binary[64];
	char key64_binary[64];

	char key56_binary[56];
	char key28_left[28];
	char key28_right[28];

	unsigned int aux_buf;

	int i = 0, opt = 0;

	char **text_hex, **key64_hex;
	text_hex = (char **) malloc(sizeof (char *) * 8);
	key64_hex = (char **) malloc(sizeof (char *) * 8);
	for (i = 0; i < 8; i++)
	{
		text_hex[i] = (char *) malloc(sizeof(char) * 2);
		key64_hex[i] = (char *) malloc(sizeof(char) * 2);
	}

	char **key56_hex = (char **) malloc(sizeof (char *) * 7);
	for (i = 0; i < 8; i++)
		key56_hex[i] = (char *) malloc(sizeof(char) * 2);


	printf("Type of input:\n\t1 - ASCII\n\t2 - Decimal\n\t3 - Hex\n> ");
	scanf("%d", &opt);


	if(opt == 1)
	{
		printf("Text to cipher (8 chars) > ");
		scanf("%s", text);

		printf("Key (8 chars) > ");
		scanf("%s", key);
	} 
	else if (opt == 2)
	{
		i = 0;
		printf("Text to cipher (8 integer values) > ");
		while(1)
		{
			scanf("%u", &aux_buf);
			sprintf(&text[i], "%c", aux_buf);
			i++;
			if (i >= 8)
				break;
		}
		text[i]='\0';

		printf("Key (8 integer values) > ");
		i = 0;
		while(1)
		{
			scanf("%u", &aux_buf);
			sprintf(&key[i], "%c", aux_buf);
			i++;
			if (i >= 8)
				break;
		}
		key[i]='\0';
	} 
	else
	{
		i = 0;
		printf("Text to cipher (16 hex) > ");
		while(1)
		{
			scanf("%02x", &aux_buf);
			sprintf(&text[i], "%c", aux_buf);
			i++;
			if (i >= 8)
				break;
		}
		text[i]='\0';
		i = 0;
		printf("Key (16 hex) > ");
		while(1)
		{
			scanf("%02x", &aux_buf);
			sprintf(&key[i], "%c", aux_buf);
			i++;
			if (i >= 8)
				break;
		}
		key[i]='\0';
	}
	
	str_to_hex(text, text_hex); //get text_hex from text
	hex_to_binary(text_hex, text_binary); //get text_binary

	str_to_hex(key, key64_hex); //get key64_hex
	hex_to_binary(key64_hex, key64_binary); //get key64

	ip(text_binary); //initial permutation
	binary_to_hex(text_binary, text_hex, 64); //get permuted hex

	pc1(key64_binary, key56_binary); //get key56
	binary_to_hex(key56_binary, key56_hex, 56); //get permuted hex


	for (i = 0; i < 7; i++)
	{
		printf("%c", key56_hex[i][0]);
		printf("%c ", key56_hex[i][1]);
	}

	// for (i = 0; i < 8 * 8; i++)
	// {
	// 	// if (i % 4 == 0 && i!=0)
	// 	// 	printf(" ");
	// 	if (i % 8 == 0 && i!=0)
	// 		printf(" ");
	// 	printf("%c ", key64_binary[i]);
	// }
	// printf("\n");
	// for (i = 0; i < 8 * 7; i++)
	// {
	// 	// if (i % 4 == 0 && i!=0)
	// 	// 	printf(" ");
	// 	if (i % 8 == 0 && i!=0)
	// 		printf(" ");
	// 	printf("%c ", key56_binary[i]);
	// }
	printf("\n");
	

	return 0;
}

void str_to_hex(char *str, char **hex)
/*
	hex format:
	hex[letter][hex chars];

	the letter column will have 8 indexes, as the "hex chars"
	on will have 2: one for each hex character of the pair
*/
{
	int i;
	
	for (i = 0; i < 8; i++)
	{
		sprintf(hex[i], "%02X", str[i]);
	}
}

void hex_to_binary(char **hex, char *b)
/*
	binary format
	binary[64];

	the binary array will have 64 bits
*/
{
	int i;
	char half_byte[4];

	for (i = 0; i < 8; i++)
	{
		strcpy(half_byte, get_half_byte(hex[i][0]));
		b[(i * 8)] 		= half_byte[0];
		b[(i * 8) + 1] = half_byte[1];
		b[(i * 8) + 2] = half_byte[2];
		b[(i * 8) + 3] = half_byte[3];

		strcpy(half_byte, get_half_byte(hex[i][1]));
		b[(i * 8) + 4] = half_byte[0];
		b[(i * 8) + 5] = half_byte[1];
		b[(i * 8) + 6] = half_byte[2];
		b[(i * 8) + 7] = half_byte[3];
	}
}

void binary_to_hex(char *b, char **hex, int binary_size)
{
	char h;
	char half_byte[4];
	int i;

	for (i = 0; i < binary_size; i = i+8)
	{
		half_byte[0] = b[i];
		half_byte[1] = b[i + 1]; 
		half_byte[2] = b[i + 2]; 
		half_byte[3] = b[i + 3]; 
		h = get_hex_single(half_byte);
		// printf("%c", h);
		hex[i/8][0] = h;
		half_byte[0] = b[i + 4];
		half_byte[1] = b[i + 5]; 
		half_byte[2] = b[i + 6]; 
		half_byte[3] = b[i + 7]; 
		h = get_hex_single(half_byte);
		// printf("%c ", h);
		hex[i/8][1] = h;
	}
}

char* get_half_byte(char hex)
/*
	function that, given a hex char, returns 4 bits
	corresponding the hex value of the char passed
*/
{
	switch(hex)
	{
		case '0':
			return "0000";
		case '1':
			return "0001";
		case '2':
			return "0010";
		case '3':
			return "0011";
		case '4':
			return "0100";
		case '5':
			return "0101";
		case '6':
			return "0110";
		case '7':
			return "0111";
		case '8':
			return "1000";
		case '9':
			return "1001";
		case 'A':
			return "1010";
		case 'B':
			return "1011";
		case 'C':
			return "1100";
		case 'D':
			return "1101";
		case 'E':
			return "1110";
		case 'F':
			return "1111";
		default:
			return "0000";
	}
}

char get_hex_single(char *half_byte)
/*
	function that, given 4 bits, returns the hex char
	corresponding to the bits informed
*/
{
	if(strcmp(half_byte, "0000") == 0)
		return '0';
	else if(strcmp(half_byte, "0001") == 0)
		return '1';
	else if(strcmp(half_byte, "0010") == 0)
		return '2';
	else if(strcmp(half_byte, "0011") == 0)
		return '3';
	else if(strcmp(half_byte, "0100") == 0)
		return '4';
	else if(strcmp(half_byte, "0101") == 0)
		return '5';
	else if(strcmp(half_byte, "0110") == 0)
		return '6';
	else if(strcmp(half_byte, "0111") == 0)
		return '7';
	else if(strcmp(half_byte, "1000") == 0)
		return '8';
	else if(strcmp(half_byte, "1001") == 0)
		return '9';
	else if(strcmp(half_byte, "1010") == 0)
		return 'A';
	else if(strcmp(half_byte, "1011") == 0)
		return 'B';
	else if(strcmp(half_byte, "1100") == 0)
		return 'C';
	else if(strcmp(half_byte, "1101") == 0)
		return 'D';
	else if(strcmp(half_byte, "1110") == 0)
		return 'E';
	else if(strcmp(half_byte, "1111") == 0)
		return 'F';
	else
		return ' ';
}

void ip(char *array)
{
	int i;
	int perm_array[] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17, 9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};

	char array_cp[8 * 8];
	for (i = 0; i < 8 * 8; i++)
		array_cp[i] = array[i];
		
	for(i = 0; i < 8 * 8; i++)
	{
		array[i] = array_cp[perm_array[i] - 1];
	}
}

void final_permutation(char *array)
{
	int perm_array[] = {40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 
		47, 15, 55, 23, 63, 31, 38, 6, 46, 14, 54, 22, 62, 30, 
		37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20, 
		60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 
		50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25};
}

void pc1(char *key64, char *key56)
{
	int i;
	int perm_array[] = {
		57, 49, 41, 33, 25, 17, 9, 
		1, 58, 50, 42, 34, 26, 18, 
		10, 2, 59, 51, 43, 35, 27, 
		19, 11, 3, 60, 52, 44, 36, 
		63, 55, 47, 39, 31, 23, 15, 
		7, 62, 54, 46, 38, 30, 22, 
		14, 6, 61, 53, 45, 37, 29, 
		21, 13, 5, 28, 20, 12, 4
	};
		
	for(i = 0; i < 8 * 7; i++)
		key56[i] = key64[perm_array[i] - 1];
	
}
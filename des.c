#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DES Algorithm Operations */
void ip(char *);
void inverse_ip(char *);
void pc1(char *, char *);
void pc2(char *, char *);
void expansion(char *text32, char *text48);
void p(char *t32);

/* SBOXES */
void s1(char *, char *); 		void s2(char *, char *);
void s3(char *, char *); 		void s4(char *, char *);
void s5(char *, char *); 		void s6(char *, char *);
void s7(char *, char *); 		void s8(char *, char *);

/* Helper Functions*/
void  join_sr(char *, char *, char *, char *, char *, char *, char *, char *, char *);
int   binary_to_int(char *);
char* int_to_binary(int);
void  str_to_hex(char *, char **);
void hex_to_binary(char **, char *, int);
void binary_to_hex(char *, char **, int);
char* get_half_byte(char );
char  get_hex_single(char *);
void  printh(char **, int);

/* Operations Functions*/
void  split(char *, char *, char *, int);
void  join(char *, char *, char *, int);
void  shift_block(char *, char *, int);
void  swap(char *, char *, int);
void xor(char *k, char *t, char *r, int s);

struct ShiftStep {
	char l_block[28];
	char r_block[28];
};

/* structure that will represent all the 16 shift steps */
typedef struct {
	struct ShiftStep round_n[16];
	int shift_n[16]; // can be 1 or 2
} ShiftStructure;

int main()
{
	ShiftStructure shift_s;
	char text[9];
	char text64_binary[64];
	char text32_left[32];
	char text32_right[32];
	char text48_binary[48];
	char text32_r_f[48];
	char t_xor_round[48];

	char sp1[7];	char sr1[4];
	char sp2[7];	char sr2[4];
	char sp3[7];	char sr3[4];
	char sp4[7];	char sr4[4];
	char sp5[7];	char sr5[4];
	char sp6[7];	char sr6[4];
	char sp7[7];	char sr7[4];
	char sp8[7];	char sr8[4];
	
	char key[9];
	char key64_binary[64];
	char key56_binary[56];
	char key48_binary[56];
	char key28_left[28];
	char key28_right[28];

	int aux_buf;
	int i = 0, opt = 0, j = 0;

	char **text_hex, **text48_hex, **text32_hex, 
			 **key64_hex, **key56_hex, **key48_hex;

	text_hex  = (char **) malloc(sizeof (char *) * 8);
	key64_hex = (char **) malloc(sizeof (char *) * 8);
	for (i = 0; i < 8; i++)
	{
		text_hex[i]  = (char *) malloc(sizeof(char) * 2);
		key64_hex[i] = (char *) malloc(sizeof(char) * 2);
	}
	key56_hex = (char **) malloc(sizeof (char *) * 7);
	for (i = 0; i < 7; i++)
		key56_hex[i] = (char *) malloc(sizeof(char) * 2);

	key48_hex  = (char **) malloc(sizeof (char *) * 6);
	text48_hex = (char **) malloc(sizeof (char *) * 6);
	for (i = 0; i < 6; i++) // 48 = 6 pairs of 8 bits
	{
		key48_hex[i]  = (char *) malloc(sizeof(char) * 2);
		text48_hex[i]  = (char *) malloc(sizeof (char) * 2);
	}
	text32_hex = (char **) malloc(sizeof (char *) * 4);
	for (i = 0; i < 4; i++)
		text32_hex[i] = (char *) malloc(sizeof(char) * 2);

	shift_s.shift_n[0]  = 1; 		shift_s.shift_n[1]  = 1;
	shift_s.shift_n[2]  = 2; 		shift_s.shift_n[3]  = 2;
	shift_s.shift_n[4]  = 2;		shift_s.shift_n[5]  = 2;
	shift_s.shift_n[6]  = 2;		shift_s.shift_n[7]  = 2;
	shift_s.shift_n[8]  = 1;		shift_s.shift_n[9]  = 2;
	shift_s.shift_n[10] = 2;		shift_s.shift_n[11] = 2;
	shift_s.shift_n[12] = 2;		shift_s.shift_n[13] = 2;
	shift_s.shift_n[14] = 2;		shift_s.shift_n[15] = 1;

	printf("Type of input:\n\t1 - ASCII\n\t2 - Decimal\n\t3 - Hex\n> ");
	scanf("%d", &opt);

	if(opt == 1) // ASCII String
	{
		printf("Text to cipher (8 chars) > ");
		scanf("%s", text);

		printf("Key (8 chars) > ");
		scanf("%s", key);
	} 
	else if (opt == 2) // INT
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
	} 
	else // HEX
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
		
	}
	// setting end to inputs
	text[8]='\0'; 
	key[8]='\0';
	printf("-------------------------------------------\n\n");

	printf("PLAIN TEXT: \"%s\"\n", text);

	str_to_hex(text, text_hex); //get text_hex from text
	hex_to_binary(text_hex, text64_binary, 64); //get text64_binary

	printh(text_hex, 8);
	printf("\n\n");

	ip(text64_binary); //initial permutation
	binary_to_hex(text64_binary, text_hex, 64); //get permuted hex

	//print ip hexadecimal
	printf("IP\n");
	printh(text_hex, 8);

	str_to_hex(key, key64_hex); //get key64_hex
	hex_to_binary(key64_hex, key64_binary, 64); //get key64_binary

	printf("\n\nKEY: \"%s\"\n", key);
	printh(key64_hex, 8);
	printf("\n\n");

	pc1(key64_binary, key56_binary); //get key56 binary from pc1
	binary_to_hex(key56_binary, key56_hex, 56); //get permuted hex

	printf("PC1 - SELECT KEY\n");
	printh(key56_hex, 7);
	printf("\n\n");

	// Split key into right & left
	split(key56_binary, key28_left, key28_right, 56); 

	// Set shifted blocks regarding the key
	shift_block(shift_s.round_n[0].l_block, key28_left, shift_s.shift_n[0]);
	shift_block(shift_s.round_n[0].r_block, key28_right, shift_s.shift_n[0]);
	for (i = 1; i < 16; i++)
	{
		shift_block(shift_s.round_n[i].l_block, shift_s.round_n[i-1].l_block, shift_s.shift_n[i]);
		shift_block(shift_s.round_n[i].r_block, shift_s.round_n[i-1].r_block, shift_s.shift_n[i]);
	}

	// Split text binary
	split(text64_binary, text32_left, text32_right, 64);

	// Begin Rounds Loop
	for (j = 0; j < 16; j++)
	{
		join(shift_s.round_n[j].l_block, shift_s.round_n[j].r_block, key56_binary, 56);
		
		binary_to_hex(key56_binary, key56_hex, 56); //get shifted hex

		printf("--- CHAVE DE ROUND %d ---\n", j+1);
		printf("Shift: ");
		printh(key56_hex, 7);

		hex_to_binary(key56_hex, key56_binary, 56); //get key56 binary

		pc2(key56_binary, key48_binary);
		binary_to_hex(key48_binary, key48_hex, 48); //get permuted hex

		printf("\nPC2: ");
		//Print PC2 hex
		printh(key48_hex, 6);
		printf("\n");
		printh(key48_hex, 6);
		printf("\n\n");

		printf("--- ROUND %d ---\n", j+1);

		// Getting cipher text to print
		join(text32_left, text32_right, text64_binary, 64);
		binary_to_hex(text64_binary, text_hex, 64);
		printh(text_hex, 8);

		// expand right key
		expansion(text32_right, text48_binary);
		binary_to_hex(text48_binary, text48_hex, 48); //get expanded hex for printing
		// print expanded key
		printf("\nExpansion: ");
		printh(text48_hex, 6);
		printf("\n");

		xor(key48_binary, text48_binary, t_xor_round, 48);
		binary_to_hex(t_xor_round, text48_hex, 48); //get expanded hex
		printf("Add Key: ");
		//print cipher HEX
		printh(text48_hex, 6);

		// sbox on t_xor_round
		strncpy(sp1, &t_xor_round[0],  6);
		sp1[6] = '\0';
		strncpy(sp2, &t_xor_round[6],  6);
		sp2[6] = '\0';
		strncpy(sp3, &t_xor_round[12], 6);
		sp3[6] = '\0';
		strncpy(sp4, &t_xor_round[18], 6);
		sp4[6] = '\0';
		strncpy(sp5, &t_xor_round[24], 6);
		sp5[6] = '\0';
		strncpy(sp6, &t_xor_round[30], 6);
		sp6[6] = '\0';
		strncpy(sp7, &t_xor_round[36], 6);
		sp7[6] = '\0';
		strncpy(sp8, &t_xor_round[42], 6);
		sp8[6] = '\0';
		
		s1(sp1, sr1);
		s2(sp2, sr2);
		s3(sp3, sr3);
		s4(sp4, sr4);
		s5(sp5, sr5);
		s6(sp6, sr6);
		s7(sp7, sr7);
		s8(sp8, sr8);

		// join all the sbox partials and put them on text32_r_f
		join_sr(sr1, sr2, sr3, sr4, sr5, sr6, sr7, sr8, text32_r_f);
		binary_to_hex(text32_r_f, text32_hex, 32);

		printf("\nS-Box: ");
		printh(text32_hex, 4);

		// inner permutation on the sbox result
		p(text32_r_f);
		binary_to_hex(text32_r_f, text32_hex, 32);

		printf("\nPermutation: ");
		printh(text32_hex, 4);

		// adding (XOT) left to right  
		xor(text32_left, text32_r_f, text32_r_f, 48);
		binary_to_hex(text32_r_f, text32_hex, 32);
		
		printf("\nAdd Left: ");
		printh(text32_hex, 4);
		printf("\n");

		// Ln = Rn-1 && Lr = f(Lr-1) XOR Ln-1
		for (i = 0; i < 32; i++)
		{
			text32_left[i] = text32_right[i];
			text32_right[i] = text32_r_f[i];
		}

		//converting right & left for printing
		binary_to_hex(text32_left, text32_hex, 32);
		printh(text32_hex, 4);
		
		binary_to_hex(text32_right, text32_hex, 32);
		printh(text32_hex, 4);
		
		printf("\n\n");
	}
	
	// After round loop is finished

	// Swap left & right
	swap(text32_left, text32_right, 32);
	printf("Swap: ");
	// convert to hex for printing
	binary_to_hex(text32_left, text32_hex, 32);
	printh(text32_hex, 4);
	binary_to_hex(text32_right, text32_hex, 32);
	printh(text32_hex, 4);
	printf("\n\n");
	
	// join both sides & apply inverse_ip
	join(text32_left, text32_right, text64_binary, 64);
	inverse_ip(text64_binary);

	printf("Inverse IP: ");
	binary_to_hex(text64_binary, text_hex, 64);
	printh(text_hex, 8);

	printf("\n");

	return 0;
}

/* ----------- DES FUNCTIONS ----------- */

/*
	#ip
	DES initial permutation
*/
void ip(char *array)
{
	int i;
	int p[] = {
		58, 50, 42, 34, 26, 18, 10, 2,
		60, 52, 44, 36, 28, 20, 12, 4,
		62, 54, 46, 38, 30, 22, 14, 6,
		64, 56, 48, 40, 32, 24, 16, 8,
		57, 49, 41, 33, 25, 17,  9, 1,
		59, 51, 43, 35, 27, 19, 11, 3,
		61, 53, 45, 37, 29, 21, 13, 5,
		63, 55, 47, 39, 31, 23, 15, 7
	};

	char a_cpy[64];
	for (i = 0; i < 64; i++)
		a_cpy[i] = array[i];
		
	for(i = 0; i < 64; i++)
		array[i] = a_cpy[p[i] - 1];
}

/*
	#inverse_ip
	DES final permutation
*/
void inverse_ip(char *array)
{
	int i;
	int p[] = {
		40, 8, 48, 16, 56, 24, 64, 32,
	  39, 7, 47, 15, 55, 23, 63, 31, 
	  38, 6, 46, 14, 54, 22, 62, 30, 
		37, 5, 45, 13, 53, 21, 61, 29, 
		36, 4, 44, 12, 52, 20, 60, 28, 
		35, 3, 43, 11, 51, 19, 59, 27, 
		34, 2, 42, 10, 50, 18, 58, 26, 
		33, 1, 41,  9, 49, 17, 57, 25
	};

	char a_cpy[64];
	for (i = 0; i < 64; i++)
		a_cpy[i] = array[i];
		
	for(i = 0; i < 64; i++)
		array[i] = a_cpy[p[i] - 1];
}

/*
	#pc1
	DES PC1
*/
void pc1(char *key64, char *key56)
{
	int i;
	int p[] = {
		57, 49, 41, 33, 25, 17,  9, 
		 1, 58, 50, 42, 34, 26, 18, 
		10,  2, 59, 51, 43, 35, 27, 
		19, 11,  3, 60, 52, 44, 36, 
		63, 55, 47, 39, 31, 23, 15, 
		 7, 62, 54, 46, 38, 30, 22, 
		14,  6, 61, 53, 45, 37, 29, 
		21, 13,  5, 28, 20, 12,  4
	};
		
	for(i = 0; i < 8 * 7; i++)
		key56[i] = key64[p[i] - 1];
}

/*
	#pc2
	DES PC2
*/
void pc2(char *key56, char *key48)
{
	int i;
	int p[] = {
		14, 17, 11, 24,  1,  5,
		 3, 28, 15,  6, 21, 10,
		23, 19, 12,  4, 26,  8,
		16,  7, 27, 20, 13,  2,
		41, 52, 31, 37, 47, 55,
		30, 40, 51, 45, 33, 48,
		44, 49, 39, 56, 34, 53,
		46, 42, 50, 36, 29, 32
	};
		
	for(i = 0; i < 48; i++)
		key48[i] = key56[p[i] - 1];
}

/*
	#expansion
	DES Expansion (function e)
*/
void expansion(char *text32, char *text48)
{
	int i;
	int p[] = {
		32,  1,  2,  3,  4,  5,
		 4,  5,  6,  7,  8,  9,
		 8,  9, 10, 11, 12, 13,
		12, 13, 14, 15, 16, 17,
		16, 17, 18, 19, 20, 21,
		20, 21, 22, 23, 24, 25,
		24, 25, 26, 27, 28, 29,
		28, 29, 30, 31, 32,  1
	};

	for(i = 0; i < 48; i++)
		text48[i] = text32[p[i] - 1];
}

/* -------------- S-BOXES -------------- */
/*
	#sn
	sbox n for the relative 6 bits partial
	returns 4 bits partial 
*/
void s1(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
		0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
		4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
		15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s2(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
		3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
		0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
		13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s3(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[3], b_c[5];
	int s[] = {
		10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
		13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
		13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
		1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12
	};
	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s4(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
		13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
		10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
		3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s5(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
		14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
		4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
		11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s6(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
		10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
		9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
		4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s7(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
		13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
		1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
		6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void s8(char *b6, char *b4) 
{
	int i, row, column;
	char b_r[2], b_c[4];
	int s[] = {
		13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
		1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
		7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
		2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11
	};

	b_r[0] = b6[0];
	b_r[1] = b6[5];
	b_r[2] = '\0';
	b_c[0] = b6[1];
	b_c[1] = b6[2];
	b_c[2] = b6[3];
	b_c[3] = b6[4];
	b_c[4] = '\0';
	row = binary_to_int(b_r);
	column = binary_to_int(b_c);

	strcpy(b4, int_to_binary(s[column + (row * 16)]));
}

void p(char *t32)
{
	int i;
	int p[] = {
		16, 7, 20, 21,
		29, 12, 28, 17,
		1, 15, 23, 26,
		5, 18, 31, 10,
		2, 8, 24, 14,
		32, 27, 3, 9,
		19, 13, 30, 6,
		22, 11, 4, 25
	};
	char aux[32];
	for(i = 0; i < 32; i++)
		aux[i] = t32[i];

	for(i = 0; i < 32; i++)
		t32[i] = aux[p[i] - 1];
	
}

/* --------- HELPER FUNCTIONS ---------- */

/*
	#join_sr
	joim sboxes' partials and concatenate them on "r"
*/
void join_sr(char *s1, char *s2, char *s3, char *s4, char *s5, char *s6, char *s7, char *s8, char *r)
{
	r[0] = '\0';
	strcat(r, s1);
	strcat(r, s2);
	strcat(r, s3);
	strcat(r, s4);
	strcat(r, s5);
	strcat(r, s6);
	strcat(r, s7);
	strcat(r, s8);
}

int binary_to_int(char *b)
{
	if (strcmp(b, "00") == 0)
		return 0;
	else if (strcmp(b, "01") == 0)
		return 1;
	else if (strcmp(b, "10") == 0)
		return 2;
	else if (strcmp(b, "11") == 0)
		return 3;
	else if (strcmp(b, "0000") == 0)
		return 0;
	else if (strcmp(b, "0001") == 0)
		return 1;
	else if (strcmp(b, "0010") == 0)
		return 2;
	else if (strcmp(b, "0011") == 0)
		return 3;
	else if (strcmp(b, "0100") == 0)
		return 4;
	else if (strcmp(b, "0101") == 0)
		return 5;
	else if (strcmp(b, "0110") == 0)
		return 6;
	else if (strcmp(b, "0111") == 0)
		return 7;
	else if (strcmp(b, "1000") == 0)
		return 8;
	else if (strcmp(b, "1001") == 0)
		return 9;
	else if (strcmp(b, "1010") == 0)
		return 10;
	else if (strcmp(b, "1011") == 0)
		return 11;
	else if (strcmp(b, "1100") == 0)
		return 12;
	else if (strcmp(b, "1101") == 0)
		return 13;
	else if (strcmp(b, "1110") == 0)
		return 14;
	else if (strcmp(b, "1111") == 0)
		return 15;
	else return -1;
}

char* int_to_binary(int n)
{
	switch(n)
	{
		case 0:
			return "0000";
		case 1:
			return "0001";
		case 2:
			return "0010";
		case 3:
			return "0011";
		case 4:
			return "0100";
		case 5:
			return "0101";
		case 6:
			return "0110";
		case 7:
			return "0111";
		case 8:
			return "1000";
		case 9:
			return "1001";
		case 10:
			return "1010";
		case 11:
			return "1011";
		case 12:
			return "1100";
		case 13:
			return "1101";
		case 14:
			return "1110";
		case 15:
			return "1111";
	}
}

/*
	#str_to_hex
	hex format: hex[letter][hex chars];

	the letter column will have 8 indexes, as the "hex chars"
	on will have 2: one for each hex character of the pair
*/
void str_to_hex(char *str, char **hex)
{
	int i;
	
	for (i = 0; i < 8; i++)
	{
		sprintf(hex[i], "%02X", str[i]);
	}
}

/*
  #hex_to_binary
	binary format: binary[64];

	the binary array will have 64 bits
*/
void hex_to_binary(char **hex, char *b, int n_bits)
{
	int i;
	char half_byte[4];

	for (i = 0; i < n_bits / 8; i++)
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

/*
	#binary_to_hex
	get hex string from bitset of n_bits size
*/
void binary_to_hex(char *b, char **hex, int n_bits)
{
	char h;
	char half_byte[4];
	int i;


	for (i = 0; i < n_bits; i = i+8)
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

/*
	#get_half_byte
	function that, given a hex char, returns 4 bits
	corresponding the hex value of the char passed
*/
char* get_half_byte(char hex)
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

/*
	#get_hex_single
	function that, given 4 bits, returns the hex char
	corresponding to the bits informed
*/
char get_hex_single(char *half_byte)
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

/*
	#printh
	hex format: hex[letter][hex chars];

	function to print a hex value of "s" pairs
*/
void printh(char **h, int s)
{
	int i;
	for (i = 0; i < s; i++)
		printf("%c%c ", h[i][0], h[i][1]);
}

/* -------- OPERATION FUNCTIONS -------- */

/*
	#split
	split a bitset of size s into 2 pieces, bl & br
*/
void split(char *b, char *bl, char *br, int s)
{
	int i;
	for (i = 0; i < s; i++)
	{
    if (i < s / 2)
    	bl[i] = b[i];
    else
    	br[i % (s / 2)] = b[i];
	}
}

/*
	#shift_block
	left shift a "r" block "n_shift" times. The "r"
	bitset is the shifted one
*/
void shift_block(char *s, char *r, int n_shift)
{
	char of;
	int i;

	for (i = 0; i < 28; i++)
		s[i] = r[i];

	while (n_shift > 0)
	{
		of = s[0];
		
		for (i = 1; i < 28; i++)
			s[i-1] = s[i];
		s[27] = of;

		n_shift--;
	}
}

/*
  #join
  join two bitset into one of size "s"
  the result is in "rb"
*/
void join(char *b1, char *b2, char *rb, int s)
{
	int i;
  for (i = 0; i < s; i++)
  {
  	if (i < s / 2)
  		rb[i] = b1[i];
  	else 
  		rb[i] = b2[i - (s / 2)];
  }
}

/*
  #xor
  xor operation of two bitset of size "s"
  the result is in "r"
*/
void xor(char *k, char *t, char *r, int s)
{
	int i;
	for (i = 0; i < s; i++)
	{
		if(t[i] == k[i])
			r[i] = '0';
		else
			r[i] = '1';
	}
}

/*
	#swap
	simply swap two bitset of site "s"
*/
void swap(char *l, char *r, int s)
{
	int i;
	char a;
	for (i = 0; i < s; i++)
	{
		a = l[i];
		l[i] = r[i];
		r[i] = a;
	}
}
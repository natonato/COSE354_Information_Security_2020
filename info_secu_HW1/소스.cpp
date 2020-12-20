
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


char str[] = "HRDKHUBHAAMAEQMTMZSHGBAKFUBHAASYRXUNKYUAATQCTLUTOGEWVAJGVEIIYTKIOTQRXXQVSQLISVVOCNGCUXPKPIUBOHTVKCFKWNJSEZYSSUTUOESIXKAPVFXNZHAOQTLCGYJVAEHLNNKEESQMKSHKKDFCNZSRHRDKHSDKFXVPTGMKRUPZBIKEVNYEKXMFXKFYMWYUDZDENEWNKDAOUXGPCXZDLCSNFGCMCSNUAOJDBLQTAHEWYZCHQJYKSNUWOKQKONZGOKDXGUXKEMWQMCFGUEAVKHDIIATCHVTGYMGKJMLNPCNAYKMIRWEETIYQKELEGLQOVKISFNUDAJQIQYBXQTMZSHGBAKFZRCNWRSODAFKKXWGAZGDBIUDDHCUDFRFOVSZXADSHYSGLTQBMNEMKDCFSOZSRDYLIHIAXCMGMFEIDNZKOVJEOIEFNWWQEDRLZYZIZXADSHYSGLJYFWDUAKSIOGOZOXWYPBUFEPNBIRJUJNDZJJYMURKNCIKPWLRMRIAGVSXTYNIWPROHLDHQOMBEKZURQCLQOVKISFNUAFQBHGPCLHZTPJVPXIZKLQSNVKIJAEITTNVSVWNFYVATDEMKDCTGIHKZTVGZYXTYQEDBACFMNCAHRDKHSDKFXZXXGMJOSLPSZBMOILMMWRALAFFMNXXDYFBIYQVVOHSWKGBIRJGTBYQLKIJAEQBTAXGFGAVUIJADHQKLFWRJXYFVIGGQZNBHSUIYOZALSKIABLWQNXNXKOAJAIKHXODXWORVDOGBMHOPLQJZALQJZALIKTKLENZHQAVYUEUFEVLUXHGOWNMGWXUIAHGQOMNCKFQLIPBNKVWDLNGMJCOBFKIGBYWPAHMMPQLUTOGECXITZVVAJEOIDCNWMFNLOBGQXCYFWQFWVXWRKWYGBFHJVLBAWBOUQEKHZHSZZIZARYITDCLQFPGBTJMQVSQLIHPEJONCYMZWTVJVZOBOMOHPSXMPUKVAGXIPOQUQUQBCKXZJSZAHEWYHAEMKOJCCCFBEUKVNCAWANSNXISVVOWHQGQFBGWKQEGBIFRGIZUJQWIMFANTGBHWGVAGXIPOQUQTTRMWDHDGRFENKYPZVCLNQAUBTZSRYGVGOWSVROENABMZTOHZRQFUEVPLLIODEYRYLUTOGPYAFHJFIVOSFMPBSHLEKWYWJYTFYETAZQCRFTFHOMACOQVTWKLKYMGIMQDSYNWMFNIEITWMBVVWANBQFVUSKZOTLCCWABAGHWZBZHRDKHDTUOMUUUGQICHNUUQFJYUCQUO";

FILE *fp = fopen("plain.txt", "w");

int test[1000000][2];

int len = strlen(str);

bool gcd(int a, int b) {
	if (b == 0) return false;
	else if (b == 1) return true;
	else gcd(b, a%b);

}

int key_a, key_b, key_c, key_d;

bool findans(int l1, int l2, int l3, int l4, int loc) {
	int a, b, c, d;
	int inverse = (((l1*l4 - l2 * l3) % 26)+26%26);
	int inverse_mod_26=0;



	for (int i = 0; i < 26; i++) {
		if ((i*inverse) % 26 == 1) inverse_mod_26 = (((i%26)+26)%26);
	}
	a = (l4 * inverse_mod_26) % 26;
	b = (((-l2 * inverse_mod_26) % 26) + 26) % 26;
	c = (((-l3 * inverse_mod_26) % 26) + 26) % 26;
	d = (l1 * inverse_mod_26) % 26;


	key_a = ((str[loc] - 'A') * a + (str[loc + 2] - 'A') * b) % 26;
	key_b = ((str[loc+1] - 'A') * a + (str[loc + 3] - 'A') * b) % 26;
	key_c = ((str[loc] - 'A') * c + (str[loc + 2] - 'A') * d) % 26;
	key_d = ((str[loc+1] - 'A') * c + (str[loc + 3] - 'A') * d) % 26;

	if (gcd(key_a*key_d - key_b * key_c, 26)) {
		return true;
	}
	return false;
	
}




bool test_str(int a, int b, int c, int d) {
	char* plain_str = (char*)calloc(300,sizeof(char));
	int chk[26] = { 0, };
	for (int i = 0; i < len; i += 2) {
		if (i < 280) {
			plain_str[i] = ((((str[i] - 'A') * a + (str[i + 1] - 'A') * c) % 26) + 26) % 26 + 'A';
			plain_str[i + 1] = ((((str[i] - 'A') * b + (str[i + 1] - 'A') * d) % 26) + 26) % 26 + 'A';
		}
		chk[((((str[i] - 'A') * a + (str[i + 1] - 'A') * c) % 26) + 26) % 26]++;
		chk[((((str[i] - 'A') * b + (str[i + 1] - 'A') * d) % 26) + 26) % 26]++;
	}

	if (chk['T' - 'A'] > len *(6.0 / 100.0) && chk['E' - 'A'] > len *(8.0 / 100.0)
		&& chk['Z' - 'A'] < len *(2.0 / 100.0) && chk['X' - 'A'] < len *(2.0 / 100.0)) {
		printf("%s\n\n\n", plain_str);
		return true;
	}

	return false;
}

void print_plain(int* arr, int size, char**plain_candidate) {
	int cnt = 0;
	char prev='0';
	for (int i = 0; i < len / size; i++) {
		for (int j = 0; j < size; j++) {
			if (plain_candidate[arr[j] - 1][i] == 'H' && prev == 'T') {
				cnt++;
			}
			prev = plain_candidate[arr[j] - 1][i];
		}
	}
	if (cnt > 30) {
		for (int i = 0; i < len / size; i++) {
			for (int j = 0; j < size; j++) {
				printf("%c", plain_candidate[arr[j] - 1][i]);
				fprintf(fp, "%c", plain_candidate[arr[j] - 1][i]);
			}
		}
		printf("\n\n\n");
		fprintf(fp, "\n\n\n");
	}
}

void per(int* arr, int loc, int size, char**plain_candidate) {
	if (loc == size) {
		print_plain(arr, size, plain_candidate);
	}
	else {
		for (int i = loc; i < size; i++) {
			int temp = arr[i];
			arr[i] = arr[loc];
			arr[loc] = temp;
			
			per(arr, loc+1, size, plain_candidate);
			
			temp = arr[i];
			arr[i] = arr[loc];
			arr[loc] = temp;
		}
	}
}





void main() {
	//chk frequency by n-size block
	int keysize = 5;
	char* countstr[1000];
	int count[1000] = { 0, };
	int num = 0;
	for (int i = 0; i < 1000; i++) {
		countstr[i] = (char*)calloc(keysize+1, sizeof(char));
	}
	for (int i = 0; i < len; i += keysize) {
		bool c = true;
		char* temp = (char*)calloc((keysize+1), sizeof(char));
		strncpy(temp, &str[i], keysize);
		for (int j = 0; j < num; j++) {
			if (strcmp(countstr[j], temp)==0) {
				count[j]++;
				c = false;
			}
		}
		if (c) {
			strcpy(countstr[num], temp);
			count[num] = 1;

			num++;
		}

	}
	for (int i = 0; i < num; i++) {
		if (count[i] > 1) {
			//printf("%s : %d\n", countstr[i], count[i]);
		}
	}


	/*
	//print all 2by2 case
	int chk[26][26] = { 0, };
	for (int i = 0; i < len; i += 2) {
		chk[str[i] - 'A'][str[i + 1] - 'A']++;
	}
	printf("  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z\n");
	for (int i = 0; i < 26; i ++) {
		printf("%c ", 'A' + i);
		for (int j = 0; j < 26; j++) {
			printf("%d ", chk[i][j]);
		}
		printf("\n");
	}
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			if (chk[i][j] >= 5) {
				printf("%c%c : %f\n", 'A' + i, 'A' + j, (double)chk[i][j]);
			}
		}
	}*/
	
	//5by5
	keysize = 5;
	int plain_size = len / keysize;
	int count_temp[26];
	int sum;

	char* plain_candidate[100];
	for (int i = 0; i < 100; i++) {
		plain_candidate[i] = (char*)malloc(sizeof(char)*(plain_size+1));
	}
	int plain_cnt = 0;
	
	for (int a11 = 0; a11 < 26; a11++) {
		for (int a21 = 0; a21 < 26; a21++) {

			for (int a31 = 0; a31 < 26; a31++) {

				for (int a41 = 0; a41 < 26; a41++) {

					for (int a51 = 0; a51 < 26; a51++) {

						for (int i = 0; i < 26; i++) {
							count_temp[i] = 0;
						}

						for (int i = 0; i < len - keysize; i += keysize) {
							for (int j = i; j < i + keysize; j++) {
								sum = (str[i] - 'A') * a11 + (str[i + 1] - 'A') * a21 + (str[i + 2] - 'A') * a31 + (str[i + 3] - 'A') * a41 + (str[i + 4] - 'A') * a51;
							}
							count_temp[sum % 26]++;
						}
						
						if (count_temp['I' - 'A'] > plain_size*(5.0 / 100.0) && count_temp['A' - 'A'] > plain_size*(6.0 / 100.0) &&
							count_temp['E' - 'A'] > plain_size*(7.0 / 100.0) && count_temp['T' - 'A'] > plain_size*(7.0 / 100.0) &&
							count_temp['J' - 'A'] < plain_size*(1.0 / 100.0) && count_temp['Z' - 'A'] < plain_size*(1.0 / 100.0) &&
							count_temp['X' - 'A'] < plain_size*(1.0 / 100.0) && count_temp['Q' - 'A'] < plain_size*(1.0	 / 100.0)) {

							for (int i = 0; i < len; i += keysize) {
								for (int j = i; j < i + keysize; j++) {
									sum = (str[i] - 'A') * a11 + (str[i + 1] - 'A') * a21 + (str[i + 2] - 'A') * a31 + (str[i + 3] - 'A') * a41 + (str[i + 4] - 'A') * a51;
								}
								plain_candidate[plain_cnt][i/keysize] = sum % 26 + 'A';

							}
							plain_candidate[plain_cnt][len / keysize] = '\0';
							plain_cnt++;
						}
					}
				}
			}
		}
	}
	
	int* arr = (int*)malloc(sizeof(int)*plain_cnt);

	for (int i = 0; i < plain_cnt; i++)arr[i] = i + 1;
	per(arr, 0, 5, plain_candidate);



	/*
	//2by2 find with quadgram
	int char1 = 'T';
	int char2 = 'H';
	int char3 = 'A';
	int char4 = 'T';
	for (int i = 0; i < len - 4; i+=2) {
		if (findans(char1 - 'A', char2 - 'A', char3 - 'A', char4 - 'A', i)) {
			//find inverse key
			int inverse = ((key_a*key_d - key_b * key_c) % 26 + 26) % 26;
			int inverse_mod_26 = 0;
			for (int j = 0; j < 26; j++) {
				if ((j*inverse) % 26 == 1) inverse_mod_26 = (((j % 26) + 26) % 26);
			}
			if (inverse_mod_26 != 0) {
				int dec_key_a = key_d * inverse_mod_26 % 26;
				int dec_key_b = (((-key_b * inverse_mod_26) % 26) + 26) % 26;
				int dec_key_c = (((-key_c * inverse_mod_26) % 26) + 26) % 26;
				int dec_key_d = key_a * inverse_mod_26 % 26;
				test_str(dec_key_a, dec_key_b, dec_key_c, dec_key_d);
			}
		}
	}*/



	
	/*
	//2by2 brute force
	for (int a = 0; a < 26; a++) {

		for (int b = 0; b < 26; b++) {

			for (int c = 0; c < 26; c++) {

				for (int d = 0; d < 26; d++) {
					if (gcd((a*d-b*c),26)) {
						test_str(a, b, c, d);
					}
				}
			}
		}
	}*/
	
	system("pause");
}
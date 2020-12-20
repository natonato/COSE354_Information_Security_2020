#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>




unsigned int circular_shift(unsigned int a, int shift) {
	unsigned int temp = a & 0x80000000;
	temp = temp >> 31;
	a = a << 1;
	a = a | temp;
	shift -= 1;
	if (shift == 0) {
		return a;
	}
	else return circular_shift(a, shift);
}

unsigned int inv_sbox[256] = { 0x52, 0x9, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb, 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb, 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e, 0x8, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25, 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92, 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84, 0x90, 0xd8, 0xab, 0x0, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x5, 0xb8, 0xb3, 0x45, 0x6, 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x2, 0xc1, 0xaf, 0xbd, 0x3, 0x1, 0x13, 0x8a, 0x6b, 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73, 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e, 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b, 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4, 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x7, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f, 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef, 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61, 0x17, 0x2b, 0x4, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

unsigned int inv_MDS_cal(unsigned int S1, unsigned int S2, unsigned int S3, unsigned int S4) {
	//S1*14 + S2*11 + S3*13 + S4*9
	return S1 << 3 ^ S1 << 2 ^ S1 << 1 ^ S2 << 3 ^ S2 << 1 ^ S2 ^ S3 << 3 ^ S3 << 2 ^ S3  ^  S4 << 3 ^ S4;
}
unsigned int inv_MDS(unsigned int S) {
	unsigned int S1 = (S & 0xFF000000) >> 24;
	unsigned int S2 = (S & 0xFF0000) >> 16;
	unsigned int S3 = (S & 0xFF00) >> 8;
	unsigned int S4 = (S & 0xFF);
	unsigned int temp_MDS = 0;
	temp_MDS |= inv_MDS_cal(S1, S2, S3, S4);
	temp_MDS = temp_MDS << 8;
	temp_MDS |= inv_MDS_cal(S2, S3, S4, S1);
	temp_MDS = temp_MDS << 8;
	temp_MDS |= inv_MDS_cal(S3, S4, S1, S2);
	temp_MDS = temp_MDS << 8;
	temp_MDS |= inv_MDS_cal(S4, S1, S2, S3);

	return temp_MDS;
}


unsigned int inv_SBOX_cal(unsigned int S) {
	return inv_sbox[(S & 0xFFFF0000) * 16 + (S & 0xFFFF)];
}

unsigned int reverse_SBOX(unsigned int S) {
	unsigned int S1 = (S & 0xFF000000) >> 24;
	unsigned int S2 = (S & 0xFF0000) >> 16;
	unsigned int S3 = (S & 0xFF00) >> 8;
	unsigned int S4 = (S & 0xFF);
	unsigned int temp_MDS = 0;

	temp_MDS |= inv_SBOX_cal(S1);
	temp_MDS = temp_MDS << 8;
	temp_MDS |= inv_SBOX_cal(S2);
	temp_MDS = temp_MDS << 8;
	temp_MDS |= inv_SBOX_cal(S3);
	temp_MDS = temp_MDS << 8;
	temp_MDS |= inv_SBOX_cal(S4);

	return temp_MDS;
}


void main() {

	unsigned int output[8];
	int R;	//round

	//get output
	for (int i = 0; i < 8; i++) {
		scanf("%u ", &output[i]);
	}
	scanf("%d ", &R);

	//input(256)+wi(32*@) = X
	unsigned int input[8] = { 0, };
	unsigned int* wi = (unsigned int*)malloc(sizeof(unsigned int)*(R));
	unsigned int Xi[8] = { 0, };


	int BF_R = R > 8 ? R - 8 : R;
	if (R <= 16) {
		//setup init Xi and input
		//set random Xi
		
		srand(time(NULL) * 3 % 377);
		for (int i = 0; i < 8; i++) {
			Xi[i]=rand();
		}
		
		for (int i = 0; i < BF_R; i++) {
			input[i] = output[i] ^ Xi[i];
		}
		for (int i = BF_R; i < 8; i++) {
			Xi[i] = input[i - BF_R];
			input[i] = output[i] ^ Xi[i];
		}
		
		//first print
		printf("Each 32bit block is devided by blank\n");
		printf("Round %2d / X : ", R);
		for (int i = 0; i < 8; i++) {
			printf("%x ", Xi[i]);
		}printf("\n");

		//common part of round 1~16
		for (int i = 0; i < BF_R; i++) {
			unsigned int T = Xi[0] ^ input[(8-BF_R)+i]; // Hi+1 xor Hi

			for (int j = 7; j >= 1; j--) {
				T = inv_MDS(T);
				T = reverse_SBOX(T);
				T = T ^ Xi[j]; // Gi~Ai
			}
			wi[BF_R-i-1] = T;

			
			for (int j = 0; j < 7; j++) {
				Xi[j] = Xi[j + 1];
			} Xi[7] = input[(8 - BF_R) + i];

			printf("Round %2d / X : ", R-1-i);
			for (int j = 0; j < 8; j++) {
				printf("%x ", Xi[j]);
			}
			for (int j = i; j >= 0; j--) {
				printf("%x ", wi[BF_R - j - 1]);
			}printf("\n");

		}
		//if round is 9~16
		if (R > 8) {
			//shift wi 8 block
			for (int i = 0; i < BF_R; i++) {
				wi[i + 8] = wi[i];
				wi[i] = 0;
			}
			for (int i = 7; i >=0; i--) {
				unsigned int T = 0;

				for (int j = 7; j >= 1; j--) {
					T = inv_MDS(T);
					T = reverse_SBOX(T);
					T = T ^ Xi[j]; // Gi~Ai
				}
				wi[i] = T;

				unsigned int temp = Xi[0];
				for (int j = 0; j < 7; j++) {
					Xi[j] = Xi[j + 1];
				} Xi[7] = temp;


				printf("Round %2d / X : ", i);
				for (int j = 0; j < 8; j++) {
					printf("%x ", Xi[j]);
				}
				for (int j = i; j <= 7 + BF_R; j++) {
					printf("%x ", wi[j]);
				}printf("\n");
			}

		}


		printf("\nFinal X : ");
		for (int j = 0; j < 8; j++) {
			printf("%x ", Xi[j]);
		}

		for (int j = 0; j < (R < 16 ? R : 16); j++) {
			printf("%x ", wi[j]);
		}printf("\n");

	}
	else {//R>16
		BF_R = R - 16;//round to brute-force
		unsigned int* T_temp = (unsigned int*)malloc(sizeof(unsigned int)*(BF_R));
		for (int i = 0; i < BF_R; i++) {
			T_temp[i] = 0;
		}
		while (T_temp[0] != 0xFFFFFFFF) {
			//create every T case for each round - brute-force
			for (int i = 1; i < BF_R; i--) {
				if (T_temp[BF_R-i] == 0xFFFFFFFF) {
					T_temp[BF_R - i] = 0;
					T_temp[BF_R - i - 1]++;
				}
			} T_temp[BF_R - 1]++;
			
			for (int i = 0; i < 8; i++) {
				Xi[i] = rand();
			}
			
			for (int i = 0; i < 8; i++) {
				input[i] = output[i] ^ Xi[i];
			}

			//round after 16
			for (int i = BF_R-1; i >= 0; i--) {
				unsigned int Hi = Xi[0] ^ T_temp[i]; // Hi+1 xor T

				unsigned int T = T_temp[i];
				for (int j = 7; j >= 1; j--) {
					T = inv_MDS(T_temp[i]);
					T = reverse_SBOX(T);
					T = T ^ Xi[j]; // Gi~Ai
				}wi[i + 16] = T;


				for (int j = 0; j < 7; j++) {
					Xi[j] = Xi[j + 1];
				} Xi[7] = Hi;


				printf("Round %2d / X : ", R+i);
				for (int j = 0; j < 8; j++) {
					printf("%x ", Xi[j]);
				}
				for (int j = BF_R-1; j >= i; j--) {
					printf("%x ", wi[j+16]);
				}printf("\n");
			}
			//round 16->8
			for (int i = 0; i < 8; i++) {
				unsigned int T = Xi[0] ^ input[i]; // Hi+1 xor Hi

				for (int j = 7; j >= 1; j--) {
					T = inv_MDS(T);
					T = reverse_SBOX(T);
					T = T ^ Xi[j]; // Gi~Ai
				}
				wi[15-i] = T;


				for (int j = 0; j < 7; j++) {
					Xi[j] = Xi[j + 1];
				} Xi[7] = input[(8 - BF_R) + i];

				printf("Round %2d / X : ", R - 1 - i);
				for (int j = 0; j < 8; j++) {
					printf("%x ", Xi[j]);
				}
				for (int j = 0; j <= i; j++) {
					printf("%x ", wi[15 - j]);
				}printf("\n");

			}
			//Round 8->0
			for (int i = 7; i >= 0; i--) {
				unsigned int T = 0;

				for (int j = 7; j >= 1; j--) {
					T = inv_MDS(T);
					T = reverse_SBOX(T);
					T = T ^ Xi[j]; // Gi~Ai
				}
				wi[i] = T;

				unsigned int temp = Xi[0];
				for (int j = 0; j < 7; j++) {
					Xi[j] = Xi[j + 1];
				} Xi[7] = temp;


				printf("Round %2d / X : ", i);
				for (int j = 0; j < 8; j++) {
					printf("%x ", Xi[j]);
				}
				for (int j = i; j <= 16 + BF_R; j++) {
					printf("%x ", wi[j]);
				}printf("\n");
			}
			//end of compute input

			//compute & compare Wi
			unsigned int* wi_com = (unsigned int*)malloc(sizeof(unsigned int)*(BF_R));
			for (int i = 0; i < BF_R; i++) {
				wi_com[i] = wi[i + 16] ^ circular_shift(wi[i + 13],1) ^ circular_shift(wi[i + 8], 6) ^
					circular_shift(wi[i + 2], 11);
			}
			bool check = true;
			for (int i = 0; i < BF_R; i++) {
				//if wi computed by other wi == wi used to make input data, then input & wi is valid
				if (wi_com[i] != wi[i]) {
					check = false; break;
				}
			}

			//if valid, print & end
			if (check == true) {
				printf("\nFinal X : ");
				for (int j = 0; j < 8; j++) {
					printf("%x ", Xi[j]);
				}
				for (int j = 0; j < 16; j++) {
					printf("%x ", wi[j]);
				}printf("\n");

				break;
			}


		}
	}


}

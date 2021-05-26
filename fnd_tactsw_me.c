#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<asm/ioctls.h>

#define tact_d "/dev/tactsw"
#define fnd_d "/dev/fnd"

void calc(int* num, char op, int* i);
void PrintFnd(int rst, int k, unsigned char* fnd_data);

int main() {
	int tact, fnd, sum = 0, i = 0;
	int add_date[4];
	unsigned char c;
	unsigned char fnd_data[4];
	
	//fnd 0���� ����
	fnd_data[0] = 0xFF;
	fnd_data[1] = 0xFF;
	fnd_data[2] = 0xFF;
	fnd_data[3] = 0xFF;

	fnd = open(fnd_d, O_RDWR); //fnd ����
	tact = open(tact_d, O_RDWR); //tactsw ����

	write(fnd, &fnd_data, sizeof(fnd_data)); //fnd�� ��� �� 0���� ǥ��

	if (tact < 0) {
		printf("tact : open failed!\n");
	}
	while (1) {
		if (i > 3) return; //4�ڸ����� �� �ԷµǸ� ����
		while (1) {
			read(tact, &c, sizeof(c)); //tact ����ġ���� �ϳ� �б�
			usleep(100000);
			if (c) break;
		}
		switch (c) //���� ���� ���� ǥ��
		{
		case 1: printf("7 �Է�\n"); add_date[i] = 7; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 2: printf("8 �Է�\n"); add_date[i] = 8; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 3: printf("9 �Է�\n"); add_date[i] = 9; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 4: printf("4 �Է�\n"); add_date[i] = 4; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 5: printf("5 �Է�\n"); add_date[i] = 5; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 6: printf("6 �Է�\n"); add_date[i] = 6; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 7: printf("1 �Է�\n"); add_date[i] = 1; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 8: printf("2 �Է�\n"); add_date[i] = 2; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 9: printf("3 �Է�\n"); add_date[i] = 3; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 10: printf("0 �Է�\n"); i--; add_date[i] = 0; PrintFnd(add_date[i], i, fnd_data); break; //����
		case 11: printf("0 �Է�\n"); add_date[i] = 0; PrintFnd(add_date[i], i, fnd_data); i++; break;
		case 12: printf("Shutdown!\n"); return; //����
		}
		write(fnd, &fnd_data, sizeof(fnd_data));
		usleep(100000);
	}
	usleep(100000);
	close(tact);
	return 0;
}

void PrintFnd(int rst, int k, unsigned char* fnd_data) { //fnd�� ǥ�õǴ� 0~9���� ���� ǥ��
	if (rst == 0) fnd_data[k] = 0xC0;
	if (rst == 1) fnd_data[k] = 0xF9;
	if (rst == 2) fnd_data[k] = 0xA4;
	if (rst == 3) fnd_data[k] = 0xB0;
	if (rst == 4) fnd_data[k] = 0x99;
	if (rst == 5) fnd_data[k] = ~0x6D;
	if (rst == 6) fnd_data[k] = ~0x7D;
	if (rst == 7) fnd_data[k] = ~0x07;
	if (rst == 8) fnd_data[k] = ~0x7F;
	if (rst == 9) fnd_data[k] = 0x10;
}
#include<termios.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<asm/ioctls.h>
#include<math.h>

#define dip "/dev/dipsw"
#define tact_d "/dev/tactsw"
#define fnd_d "/dev/fnd"
#define dot "/dev/dot"
#define led "/dev/led"
#define clcd "/dev/clcd"


//���� to ���
int lunarDayOfMonth[] = { 29,30,58,59,59,60 };

int lunarDayOfMonthFrac[] = { 0,0,29,30,30,30 };

char lunarDayOfMonthIndex[][12] = {
    // 2001 ~ 2010
    {1,1,1,2,1,0,0,1,0,1,0,1},{1,1,0,1,0,1,0,0,1,0,1,0},
    {1,1,0,1,1,0,1,0,0,1,0,1},{0,4,1,1,0,1,0,1,0,1,0,1},
    {0,1,0,1,0,1,1,0,1,1,0,0},{1,0,1,0,1,0,4,1,1,0,1,1},
    {0,0,1,0,0,1,0,1,1,1,0,1},{1,0,0,1,0,0,1,0,1,1,0,1},
    {1,1,0,0,4,0,1,0,1,0,1,1},{1,0,1,0,1,0,0,1,0,1,0,1},
    // 2011 ~ 2020
    {1,0,1,1,0,1,0,0,1,0,1,0},{1,0,5,1,0,1,0,0,1,0,1,0},
    {1,0,1,1,0,1,0,1,0,1,0,1},{0,1,0,1,0,1,0,1,4,1,0,1},
    {0,1,0,0,1,0,1,1,1,0,1,1},{0,0,1,0,0,1,0,1,1,0,1,1},
    {1,0,0,1,2,1,0,1,0,1,1,1},{0,1,0,1,0,0,1,0,1,0,1,1},
    {1,0,1,0,1,0,0,1,0,1,0,1},{1,0,1,4,1,0,0,1,0,1,0,1},
    // 2021 ~ 2030
    {0,1,1,0,1,0,1,0,1,0,1,0},{1,0,1,0,1,1,0,1,0,1,0,1},
    {0,4,1,0,1,0,1,1,0,1,0,1},{0,1,0,0,1,0,1,1,0,1,1,0},
    {1,0,1,0,0,4,1,0,1,1,1,0},{1,0,1,0,0,1,0,1,0,1,1,1},
    {0,1,0,1,0,0,1,0,0,1,1,1},{0,1,1,0,4,0,1,0,0,1,1,0},
    {1,1,0,1,1,0,0,1,0,0,1,1},{0,1,0,1,1,0,1,0,1,0,1,0},
    // 2031 ~ 2040
    {1,0,4,1,0,1,1,0,1,0,1,0},{1,0,0,1,0,1,1,0,1,1,0,1},
    {0,1,0,0,1,0,4,1,1,1,0,1},{0,1,0,0,1,0,1,0,1,1,1,0},
    {1,0,1,0,0,1,0,0,1,1,0,1},{1,1,0,1,0,3,0,0,1,0,1,1},
    {1,1,0,1,0,0,1,0,0,1,0,1},{1,1,0,1,0,1,0,1,0,0,1,0},
    {1,1,0,1,4,1,0,1,0,1,0,0},{1,0,1,1,0,1,1,0,1,0,1,0},
    // 2041 ~ 2050
    {1,0,0,1,0,1,1,0,1,1,0,1},{0,4,0,1,0,1,0,1,1,0,1,1},
    {0,1,0,0,1,0,0,1,1,0,1,1},{1,0,1,0,0,1,2,1,0,1,1,1},
    {1,0,1,0,0,1,0,0,1,0,1,1},{1,0,1,1,0,0,1,0,0,1,0,1},
    {1,0,1,1,3,0,1,0,0,1,0,1},{0,1,1,0,1,1,0,1,0,0,0,0},
    {1,0,1,0,1,1,0,1,1,0,1,0},{1,0,3,0,1,0,1,1,0,1,1,0}
};

int lunarDayOfYear[] = {
    384,354,355,384,354,385,354,354,384,354,    // 2001 ~ 2010
    354,384,355,384,355,354,384,354,354,384,    // 2011 ~ 2020
    354,355,384,354,384,355,354,383,355,354,    // 2021 ~ 2030
    384,355,384,354,354,384,354,354,384,355,    // 2031 ~ 2040
    355,384,354,384,354,354,384,353,355,384     // 2041 ~ 2050
};

char solarDayNum[] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

int getSolarDayOfMonth(int y, int m) {    // ���� �ϼ� ���
    if (m != 2) return solarDayNum[m - 1];

    if ((y % 400 == 0) || ((y % 100 != 0) && (y % 4 == 0))) return 29;
    else return 28;
}

int getSolarDayOfYear(int y) { // �⺰ �ϼ� ���
    if ((y % 400 == 0) || ((y % 100 != 0) && (y % 4 == 0))) return 366;
    else return 365;
}

// ��� 2001/01/24 = ���� 2001/01/01
char lunarBasis[] = { 1, 1, 1 };

int getTotalDayLunar(int y, int m, int d) { // ������ ���� �� �ϼ� ���
    int i, ret = 0;
    int year = y - 2000;

    for (i = lunarBasis[0] - 1; i < year - 1; i++) {
        ret += lunarDayOfYear[i];
        //printf("year %d : %d\n", i, lunarDayOfYear[i]);
    }
    for (i = 0; i < m - 1; i++) {
        ret += lunarDayOfMonth[lunarDayOfMonthIndex[year - 1][i]];
        //printf("month %d : %d\n", i, lunarDayOfMonth[lunarDayOfMonthIndex[y-1][i]]);
    }
    ret += d - 1;

    //printf("%d\n", ret);

    return ret;
}

void getSolarDate(int totalDay, int result[3]) { //�� �ϼ��� �޾� ������� ��ȯ
    int y = 0, m = 0, d = 0;

    totalDay += 24;
    while (totalDay >= getSolarDayOfYear(++y)) totalDay -= getSolarDayOfYear(y);
    while (totalDay >= getSolarDayOfMonth(y, ++m)) totalDay -= getSolarDayOfMonth(y, m);
    d = totalDay;
    result[0] = y; result[1] = m; result[2] = d;
}

// �⺻���� ���� Ȯ��
struct existHoliday
{
    int month;
    int day;
    int lun; //���� ���� : 1, ��� ���� : 0
};

struct existHoliday holiday[] = { {1, 1, 0}, {3, 1, 0}, {5, 5, 0}, {6, 6, 0}, {8, 15, 0}, {10, 3, 0}, {10, 9, 0}, {12, 25, 0}, {1, 1, 1}, {4, 8, 1}, {8, 15, 1} };

int yearHoliday[30][2];
int p = 15;

void lun2sol(int yearHoliday[30][2], int year) {
    int j = 0;
    int i;
    for (i = 0; i < sizeof(holiday) / sizeof(struct existHoliday); i++) {
        if (holiday[i].lun == 1) {
            int result[3];
            if (holiday[i].month == 1 || holiday[i].month == 8) { //�� �Ǵ� �߼�
                getSolarDate(getTotalDayLunar(year, holiday[i].month, holiday[i].day), result);

                //����
                yearHoliday[j][0] = result[1];
                yearHoliday[j][1] = result[2] - 1;
                if (yearHoliday[j][1] <= 0) {
                    yearHoliday[j][0] -= 1;
                    yearHoliday[j][1] = solarDayNum[yearHoliday[j][0]-1] + yearHoliday[j][1]; 
                }

                //����
                yearHoliday[++j][0] = result[1];
                yearHoliday[j][1] = result[2];

                //�㳯
                yearHoliday[++j][0] = result[1];
                yearHoliday[j][1] = result[2] + 1;
                if (yearHoliday[j][1] >= solarDayNum[yearHoliday[j][0]-1]) {
                    yearHoliday[j][1] = yearHoliday[j][1] - solarDayNum[yearHoliday[j][0] - 1];
                    yearHoliday[j][0] += 1;
                }
            }
            else { //��ó�� ���ô� ��
                getSolarDate(getTotalDayLunar(year, holiday[i].month, holiday[i].day), result);
                yearHoliday[j][0] = result[1];
                yearHoliday[j][1] = result[2];
            }
        }
        else { // ��� ������
            yearHoliday[j][0] = holiday[i].month;
            yearHoliday[j][1] = holiday[i].day;
        }
        j += 1;
    }
}

void addHoliday(int m, int d) {
    yearHoliday[p][0] = m;
    yearHoliday[p][1] = d;
    p += 1;
}

/*���� ����*/
void bubble_sort_modi(int arr[30][2], int size) {
    int temp[2], i, j;
    //int flag = 0;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size - 1; j++) {

            if (arr[j][0] > arr[j + 1][0]) {
                //printf("!111111\n");
                temp[0] = arr[j][0];
                temp[1] = arr[j][1];
                arr[j][0] = arr[j + 1][0];
                arr[j][1] = arr[j + 1][1];
                arr[j + 1][0] = temp[0];
                arr[j + 1][1] = temp[1];
            }
            else if (arr[j][0] == arr[j + 1][0] && arr[j][1] > arr[j + 1][1]) {
                //printf("2222222\n");
                temp[0] = arr[j][0];
                temp[1] = arr[j][1];
                arr[j][0] = arr[j + 1][0];
                arr[j][1] = arr[j + 1][1];
                arr[j + 1][0] = temp[0];
                arr[j + 1][1] = temp[1];
            }
        }
    }
}

/*���� Ȯ��*/
int SumOfDays(int year, int month, int day) // �� �ϼ� ���ϱ� 
{
    int num, sum = 0, months[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    for (num = 1; num < year; num++)
        sum += (365 + LeapYear(num));

    for (num = 0; num < month - 1; num++)
    {
        sum += months[num];
    }
    if (month > 2)
        sum += LeapYear(year);
    sum += day;
    return sum;
}

int LeapYear(int year) // ���� ���
{
    if (year % 4 == 0)
    {
        if (year % 100 == 0)
        {
            if (year % 400 == 0)
                return 1;
            else
                return 0;
        }
        else
            return 1;
    }
    else
        return 0;
}

int DayOfWeek(sum) // ���� ���
{
    return sum % 7;
}

int date[4];

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

int tact2fnd() {
	int tact, fnd, sum = 0, i = 0;
	//int add_date[4];
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
        if (i > 3) { close(fnd); close(tact); return; } //4�ڸ����� �� �ԷµǸ� ����
		while (1) {
			read(tact, &c, sizeof(c)); //tact ����ġ���� �ϳ� �б�
			usleep(100000);
			if (c) break;
		}
		switch (c) //���� ���� ���� ǥ��
		{
		case 1: printf("7 �Է�\n"); date[i] = 7; PrintFnd(date[i], i, fnd_data); i++; break;
		case 2: printf("8 �Է�\n"); date[i] = 8; PrintFnd(date[i], i, fnd_data); i++; break;
		case 3: printf("9 �Է�\n"); date[i] = 9; PrintFnd(date[i], i, fnd_data); i++; break;
		case 4: printf("4 �Է�\n"); date[i] = 4; PrintFnd(date[i], i, fnd_data); i++; break;
		case 5: printf("5 �Է�\n"); date[i] = 5; PrintFnd(date[i], i, fnd_data); i++; break;
		case 6: printf("6 �Է�\n"); date[i] = 6; PrintFnd(date[i], i, fnd_data); i++; break;
		case 7: printf("1 �Է�\n"); date[i] = 1; PrintFnd(date[i], i, fnd_data); i++; break;
		case 8: printf("2 �Է�\n"); date[i] = 2; PrintFnd(date[i], i, fnd_data); i++; break;
		case 9: printf("3 �Է�\n"); date[i] = 3; PrintFnd(date[i], i, fnd_data); i++; break;
		case 10: printf("0 �Է�\n"); i--; date[i] = 0; PrintFnd(date[i], i, fnd_data); break; //����
		case 11: printf("0 �Է�\n"); date[i] = 0; PrintFnd(date[i], i, fnd_data); i++; break;
        case 12: printf("Shutdown!\n"); i = 4; break; //����
		}
		write(fnd, &fnd_data, sizeof(fnd_data));
		usleep(100000);
	}
	//usleep(100000);
    /*close(fnd);
	close(tact);*/
	return 0;
}


/*tact=>tnd*/

void set_date(int position, char *month, char *day) {
  
    date[0] = yearHoliday[position][0] / 10;
    date[1] = yearHoliday[position][0] % 10;
    date[2] = yearHoliday[position][1] / 10;
    date[3] = yearHoliday[position][1] % 10;
}

void check_holiday(int wDay, int position, int *week) {
    int week_date[7][2];
    int i, j;
    for (i = 0; i < 7; i++) {
        week_date[i][0] = yearHoliday[position][0];
        week_date[i][1] = yearHoliday[position][1]-(wDay-i);
    }
    for (i = 0; i < sizeof(week_date) / sizeof(week_date[0]); i++) {
        for (j = 0; j < sizeof(yearHoliday) / sizeof(yearHoliday[0]); j++) {
            if (week_date[i][0] == yearHoliday[j][0] && week_date[i][1] == yearHoliday[j][1]) {
                week[i] = 1;
            }
        }
    }
    week[0] = 1;
    week[6] = 1;
}

int change10To16(int* week) {
    int value = 2, result = 0;
    int i;
    for (i = 0; i < 7; i++) {
        if (week[i] == 1)
        {
            result += value;
        }
        value *= 2;
    }
    return 255 - result;
}

void clcd_input(char clcd_text[]) {
    int clcd_d;

    clcd_d = open(clcd, O_RDWR);
    if (clcd_d < 0) { printf("clcd error\n"); }// ����ó��

    write(clcd_d, clcd_text, strlen(clcd_text)); // �ι�°���� ���� ���ڿ�, ���ڿ� ũ��
    close(clcd_d);
}


int main() {
    /*add holiday*/
    printf("If you want to add holiday?\n");
    printf("Yes : write holiday / No : click TactSwitch 12\n");
    clcd_input("Add Holiday");
    tact2fnd();
    int monthToholi, dayToholi, i;
    monthToholi = date[0] * 10 + date[1];
    dayToholi = date[2] * 10 + date[3];
    addHoliday(monthToholi, dayToholi);
    /*add holiday*/

    /*Input year by Tact Switch*/
    clcd_input("Input Year");
    int year;
    tact2fnd();
    year = date[0] * 1000 + date[1] * 100 + date[2] * 10 + date[3];
    /*Input year by Tact Switch*/

    /*Ouput holiday - FND(month, day), LCD(holiday name), DotMatric(weekday), LED(light on holiday)*/

    /*sorting holiday*/
    clcd_input("Holiday");
    lun2sol(yearHoliday, year);
    bubble_sort_modi(yearHoliday, sizeof(yearHoliday) / sizeof(yearHoliday[0]));
    /*sorting holiday*/

    char charDay[7][3] = { "��", "��", "ȭ", "��", "��", "��", "��" }; // 3 * ������ ��  = 21 �̹Ƿ� [7][3] ������
    /*int sum, wDay;
    for (i = 0; i < sizeof(yearHoliday) / sizeof(yearHoliday[0]); i++) {
        if (yearHoliday[i][0] != 0) {
            sum = SumOfDays(year, yearHoliday[i][0], yearHoliday[i][1]);
            wDay = DayOfWeek(sum);
            printf("%d�� ���� : %d �� %d �� %s����\n", year, yearHoliday[i][0], yearHoliday[i][1], charDay[wDay]);
        }
    }*/

    /*start holiday*/
    int startposition = 0;
    char monthToDate[3], dayToDate[3];
    for (startposition = 0; startposition < sizeof(yearHoliday) / sizeof(yearHoliday[0]); startposition++) {
        if (yearHoliday[startposition][0] != 0) {
            set_date(startposition, monthToDate, dayToDate);
            break;
        }
    }

    printf("%d\n", startposition);

    int nowposition = startposition; //array yearHoliday position

    int sum, wDay;

    //char charDay[7][3] = { "��", "��", "ȭ", "��", "��", "��", "��" }; // 3 * ������ ��  = 21 �̹Ƿ� [7][3] ������

    sum = SumOfDays(year, yearHoliday[nowposition][0], yearHoliday[nowposition][1]);
    wDay = DayOfWeek(sum);

    int week[7] = { 0, };
    int week_holi;
    check_holiday(wDay, nowposition, week);
    week_holi = change10To16(week);

    int tact, fnd, dot_d, dev;
    unsigned char fnd_data[4];
    unsigned char c;
    unsigned char data;

    unsigned char alpha[7][8] =
    {
        {0xf0, 0x80, 0xf0, 0x10, 0xf0, 0x05, 0x05, 0x07}, //��
        {0xd8, 0xa8, 0x88, 0x88, 0x00, 0x07, 0x05, 0x07}, //��
        {0xf8, 0x20, 0x20, 0x20, 0x09, 0x09, 0x09, 0x15}, //ȭ
        {0x88, 0x88, 0xa8, 0xdf, 0x05, 0x07, 0x04, 0x07}, //��
        {0xf8, 0x20, 0x20, 0x20, 0x08, 0x0e, 0x0a, 0x0a}, //��
        {0xf0, 0x80, 0xf0, 0x80, 0x80, 0x07, 0x04, 0x04}, //��
        {0xf0, 0x80, 0xf0, 0x17, 0xf1, 0x07, 0x05, 0x07} //��
    };

    tact = open(tact_d, O_RDWR); //tactsw ����
    
    while (1)
    {
        if (nowposition == 30 || nowposition < startposition){ //����
            break;
        }

        /*output devices*/
        /*FND*/
        fnd = open(fnd_d, O_RDWR); //fnd ����
        for (i = 0; i < 4; i++) {
            PrintFnd(date[i], i, fnd_data);
        }
        write(fnd, &fnd_data, sizeof(fnd_data));
        usleep(1000000);
        close(fnd);
        /*FND*/

        /*DOT*/
        dot_d = open(dot, O_RDWR); // dotmatrix ����
        write(dot_d, &alpha[wDay], sizeof(alpha[wDay]));
        usleep(1000000);
        close(dot_d);
        /*DOT*/

        /*LED*/
        dev = open(led, O_RDWR); //led ����
        data = week_holi;
        write(dev, &data, sizeof(unsigned char));
        usleep(1000000);
        close(dev);
        /*LED*/
        /*output devices*/

        while (1) {
            read(tact, &c, sizeof(c)); //tact ����ġ���� �ϳ� �б�
            usleep(10000);
            if (c) break;
        }

        switch (c) //���� ���� ���� ǥ��
        {
        case 10: { // ���� �����Ϸ� �̵�
            printf("Preview\n"); 
            nowposition -= 1;
            printf("%d �� %d ��\n", yearHoliday[nowposition][0], yearHoliday[nowposition][1]);
            set_date(nowposition, monthToDate, dayToDate);
            sum = SumOfDays(year, yearHoliday[nowposition][0], yearHoliday[nowposition][1]);
            wDay = DayOfWeek(sum);
            int week[7] = { 0, };
            check_holiday(wDay, nowposition, week);
            week_holi = change10To16(week);
            break; 
        }
        case 12: {
            printf("Next!\n");
            nowposition += 1;
            set_date(nowposition, monthToDate, dayToDate);
            printf("%d �� %d ��\n", yearHoliday[nowposition][0], yearHoliday[nowposition][1]);
            sum = SumOfDays(year, yearHoliday[nowposition][0], yearHoliday[nowposition][1]);
            wDay = DayOfWeek(sum);
            int week[7] = { 0, };
            check_holiday(wDay, nowposition, week);
            week_holi = change10To16(week);
            break;
        }
        }
    }
}
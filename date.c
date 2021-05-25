#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ioctls.h>
#include <sys/stat.h>

#define dot "/dev/dot" //Dot Matrix
#define clcd "/dev/clcd" //LCD
#define fnd "/dev/fnd" //FND
#define led "/dev/led" //LED
#define tact_d "dev/tactsw" //Tact Switch
#define dip "/dev/dipsw" //DIP Switch

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

struct existHoliday
{
    int month;
    int day;
    int lun; //음력 휴일 : 1, 양력 휴일 : 0
};

struct existHoliday holiday[] = { {1, 1, 0}, {3, 1, 0}, {5, 5, 0}, {6, 6, 0}, {8, 15, 0}, {10, 3, 0}, {10, 9, 0}, {12, 25, 0}, {1, 1, 1}, {4, 8, 1}, {8, 15, 1} };

int yearHoliday[30][2];
int p = 15;

unsigned char fnd_arr[] = {
            0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,
            0x83,0xC6,0xA1,0x86,0x8E,0xC0,0xF9,0xA4,0xB0,0x99,0x89
};

int getSolarDayOfMonth(int y, int m) {    // 월별 일수 계산
    if (m != 2) return solarDayNum[m - 1];

    if ((y % 400 == 0) || ((y % 100 != 0) && (y % 4 == 0))) return 29;
    else return 28;
}

int getSolarDayOfYear(int y) { // 년별 일수 계산
    if ((y % 400 == 0) || ((y % 100 != 0) && (y % 4 == 0))) return 366;
    else return 365;
}

// 양력 2001/01/24 = 음력 2001/01/01
char lunarBasis[] = { 1, 1, 1 };

int getTotalDayLunar(int y, int m, int d) { // 음력의 차이 총 일수 계산
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

void getSolarDate(int totalDay, int result[3]) { //총 일수를 받아 양력으로 변환
    int y = 0, m = 0, d = 0;

    totalDay += 24;
    while (totalDay >= getSolarDayOfYear(++y)) totalDay -= getSolarDayOfYear(y);
    while (totalDay >= getSolarDayOfMonth(y, ++m)) totalDay -= getSolarDayOfMonth(y, m);
    d = totalDay;
    result[0] = y; result[1] = m; result[2] = d;
}

void bubble_sort_modi(int arr[30][2], int size) {
    int temp[2];
    //int flag = 0;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {

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

void lun2sol(int yearHoliday[30][2], int year) {
    int j = 0;
    for (int i = 0; i < sizeof(holiday) / sizeof(struct existHoliday); i++) {
        if (holiday[i].lun == 1) {
            int result[3];
            if (holiday[i].month == 1 || holiday[i].month == 8) { //설 또는 추석
                getSolarDate(getTotalDayLunar(year, holiday[i].month, holiday[i].day), result);

                //전날
                yearHoliday[j][0] = result[1];
                yearHoliday[j][1] = result[2] - 1;

                //당일
                yearHoliday[++j][0] = result[1];
                yearHoliday[j][1] = result[2];

                //담날
                yearHoliday[++j][0] = result[1];
                yearHoliday[j][1] = result[2] + 1;
            }
            else { //부처님 오시는 날
                getSolarDate(getTotalDayLunar(year, holiday[i].month, holiday[i].day), result);
                yearHoliday[j][0] = result[1];
                yearHoliday[j][1] = result[2];
            }
        }
        else { // 양력 공휴일
            yearHoliday[j][0] = holiday[i].month;
            yearHoliday[j][1] = holiday[i].day;
        }
        j += 1;
    }
}

void addHoliday(int m, int d) {
    yearHoliday[p][0] = m;
    yearHoliday[p][1] = d;
}

void addDate() {
    int dip_d, tact, fnd_d;
    unsigned char add, date_num;
    unsigned char fnd_data[4]; // date 표시
    int add_date[4];
    dip_d = open(dip, O_RDWR);
    tact = open(tact_d, O_RDWR);

    while (1) {
        read(dip_d, &add, sizeof(add));
        if (add > 0) {
            int date_position = 3; //date 배열의 
            while (1) {
                if (date_position < 0) break;
                while (1) {
                    read(tact, &date_num, sizeof(date_num));
                    usleep(100000);
                    if (date_num) break;
                }
                switch (date_num)
                {
                case 1: {
                    add_date[date_position] = 7;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 2: {
                    add_date[date_position] = 8;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 3: {
                    add_date[date_position] = 9;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 4: {
                    add_date[date_position] = 4;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 5: {
                    add_date[date_position] = 5;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 6: {
                    add_date[date_position] = 6;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 7: {
                    add_date[date_position] = 1;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 8: {
                    add_date[date_position] = 2;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 9: {
                    add_date[date_position] = 3;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 10: {
                    add_date[date_position] = 0;
                    date_position++;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 11: {
                    add_date[date_position] = 0;
                    date_position--;
                    fnd_data[0] = fnd_arr[add_date[0]];
                    fnd_data[1] = fnd_arr[add_date[1]];
                    fnd_data[2] = fnd_arr[add_date[2]];
                    fnd_data[3] = fnd_arr[add_date[3]];
                    write(fnd_d, fnd_data, sizeof(fnd_data));
                    break;
                }
                case 12: break;
                }
            }
        }
        if (add) break;
    }
}

int main() {
    int dip_d, tact, fnd_d;
    unsigned char add, date_num;
    unsigned char fnd_data[4]; // date 표시
    int add_date[4];
    dip_d = open(dip, O_RDWR);
    tact = open(tact_d, O_RDWR);

    char charDay[7][3] = { "일", "월", "화", "수", "목", "금", "토" }; // 3 * 문자의 수  = 21 이므로 [7][3] 선언함
	int sum, wDay;
	int year = 2021; //입력
	lun2sol(yearHoliday, year);
	bubble_sort_modi(yearHoliday, sizeof(yearHoliday) / sizeof(yearHoliday[0]));
    addDate();
    //while (1) {
    //    read(dip_d, &add, sizeof(add));
    //    if (add>0) {
    //        int date_position = 3; //date 배열의 
    //        while (1){
    //            if (date_position<0) break;
    //            while (1) {
    //                read(tact, &date_num, sizeof(date_num));
    //                usleep(100000);
    //                if (date_num) break;
    //            }
    //            switch (date_num)
    //            {
    //                case 1: {
    //                    add_date[date_position] = 7; 
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 2: {
    //                    add_date[date_position] = 8;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                     break;
    //                }
    //                case 3: {
    //                    add_date[date_position] = 9;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 4: {
    //                    add_date[date_position] = 4;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 5: {
    //                    add_date[date_position] = 5;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 6: {
    //                    add_date[date_position] = 6;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 7: {
    //                    add_date[date_position] = 1;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 8: {
    //                    add_date[date_position] = 2;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 9: {
    //                    add_date[date_position] = 3;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 10: {
    //                    add_date[date_position] = 0;
    //                    date_position++;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 11: {
    //                    add_date[date_position] = 0;
    //                    date_position--;
    //                    fnd_data[0] = fnd_arr[add_date[0]];
    //                    fnd_data[1] = fnd_arr[add_date[1]];
    //                    fnd_data[2] = fnd_arr[add_date[2]];
    //                    fnd_data[3] = fnd_arr[add_date[3]];
    //                    write(fnd_d, fnd_data, sizeof(fnd_data));
    //                    break;
    //                }
    //                case 12: break;
    //            }
    //        }
    //    }
    //    if (add) break;
    //}
}
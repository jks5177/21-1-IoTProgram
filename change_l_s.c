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

int getSolarDayOfMonth(int y, int m) {    // ???? ???? ????
    if (m != 2) return solarDayNum[m - 1];

    if ((y % 400 == 0) || ((y % 100 != 0) && (y % 4 == 0))) return 29;
    else return 28;
}

int getSolarDayOfYear(int y) { // ???? ???? ????
    if ((y % 400 == 0) || ((y % 100 != 0) && (y % 4 == 0))) return 366;
    else return 365;
}

// ???? 2001/01/24 = ???? 2001/01/01
char lunarBasis[] = { 1, 1, 1 };

int getTotalDayLunar(int y, int m, int d) { // ?????? ???? ?? ???? ????
    int i, ret = 0;
    int year = y - 2000;

    for (i = lunarBasis[0] - 1; i < year - 1; i++) {
        ret += lunarDayOfYear[i];
        //printf("year %d : %d\n", i, lunarDayOfYear[i]);
    }
    for (i = 0; i < m - 1; i++) {
        ret += lunarDayOfMonth[lunarDayOfMonthIndex[year-1][i]];
        //printf("month %d : %d\n", i, lunarDayOfMonth[lunarDayOfMonthIndex[y-1][i]]);
    }
    ret += d-1;

    //printf("%d\n", ret);

    return ret;
}

void getSolarDate(int totalDay, int result[3]) { //?? ?????? ???? ???????? ????
    int y = 0, m = 0, d = 0;

    totalDay += 24;
    while (totalDay >= getSolarDayOfYear(++y)) totalDay -= getSolarDayOfYear(y);
    while (totalDay >= getSolarDayOfMonth(y, ++m)) totalDay -= getSolarDayOfMonth(y, m);
    d = totalDay;
    result[0] = y; result[1] = m; result[2] = d;
}
/*
void main() {
    int	year, month, day;
    int result[3];

    printf("????(y), ??(m), ??(d)?? ???? : ");
    scanf_s("%d %d %d", &year, &month, &day);

    getSolarDate(getTotalDayLunar(year, month, day), result);

    printf("%d %d %d", result[0], result[1], result[2]);
}*/
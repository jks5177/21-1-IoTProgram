#include <stdio.h>
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
	for (int i = 0; i < sizeof(holiday) / sizeof(struct existHoliday); i++) {
		if (holiday[i].lun == 1) {
			int result[3];
			if (holiday[i].month == 1 || holiday[i].month == 8) { //�� �Ǵ� �߼�
				getSolarDate(getTotalDayLunar(year, holiday[i].month, holiday[i].day), result);

				//����
				yearHoliday[j][0] = result[1];
				yearHoliday[j][1] = result[2]-1;

				//����
				yearHoliday[++j][0] = result[1];
				yearHoliday[j][1] = result[2];

				//�㳯
				yearHoliday[++j][0] = result[1];
				yearHoliday[j][1] = result[2]+1;
			}
			else{ //��ó�� ���ô� ��
				getSolarDate(getTotalDayLunar(year, holiday[i].month, holiday[i].day), result);
				yearHoliday[j][0] = result[1];
				yearHoliday[j][1] = result[2];
			}
		}
		else{ // ��� ������
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

//void main()
//{
//	char charDay[7][3] = { "��", "��", "ȭ", "��", "��", "��", "��" }; // 3 * ������ ��  = 21 �̹Ƿ� [7][3] ������
//	//int yearHoliday[30][2]
//	int sum, wDay;
//	int year = 2021;
//	lun2sol(yearHoliday, year);
//	bubble_sort_modi(yearHoliday, sizeof(yearHoliday) / sizeof(yearHoliday[0]));
//	for (int i = 0; i < sizeof(yearHoliday) / sizeof(yearHoliday[0]); i++) {
//		if (yearHoliday[i][0] != 0) {
//			sum = SumOfDays(year, yearHoliday[i][0], yearHoliday[i][1]);
//			wDay = DayOfWeek(sum);
//			printf("%d�� ���� : %d �� %d �� %s����\n", year, yearHoliday[i][0], yearHoliday[i][1], charDay[wDay]);
//		}
//		
//	}
//}
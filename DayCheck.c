#include <stdio.h>

int SumOfDays(int year, int month, int day); // 총 몇 일인지 계산
int LeapYear(int year); // 윤년 계산
int DayOfWeek(int sum); // 요일 계산

void main__()
{
	int	year, month, day, sum, wDay;
	char charDay[7][3] = { "일", "월", "화", "수", "목", "금", "토" }; // 3 * 문자의 수  = 21 이므로 [7][3] 선언함

	printf("연도(year), 월(month), 일(day)를 입력 : ");
	scanf_s("%d %d %d", &year, &month, &day);
	sum = SumOfDays(year, month, day);
	/// <summary>
	/// printf("서기 1년 1월 1일부터 %d년 %d월 %d일까지의 \n", year, month, day);
	/// printf("총일수는 %d일이고, ", sum);
	/// </summary>
	wDay = DayOfWeek(sum);
	printf("지정한 날은 %s요일이다.\n", charDay[wDay]);
}

int SumOfDays(int year, int month, int day) // 총 일수 구하기 
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

int LeapYear(int year) // 윤년 계산
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

int DayOfWeek(sum) // 요일 계산
{
	return sum % 7;
}
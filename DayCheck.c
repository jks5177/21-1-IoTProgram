#include <stdio.h>

int SumOfDays(int year, int month, int day); // �� �� ������ ���
int LeapYear(int year); // ���� ���
int DayOfWeek(int sum); // ���� ���

void main__()
{
	int	year, month, day, sum, wDay;
	char charDay[7][3] = { "��", "��", "ȭ", "��", "��", "��", "��" }; // 3 * ������ ��  = 21 �̹Ƿ� [7][3] ������

	printf("����(year), ��(month), ��(day)�� �Է� : ");
	scanf_s("%d %d %d", &year, &month, &day);
	sum = SumOfDays(year, month, day);
	/// <summary>
	/// printf("���� 1�� 1�� 1�Ϻ��� %d�� %d�� %d�ϱ����� \n", year, month, day);
	/// printf("���ϼ��� %d���̰�, ", sum);
	/// </summary>
	wDay = DayOfWeek(sum);
	printf("������ ���� %s�����̴�.\n", charDay[wDay]);
}

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
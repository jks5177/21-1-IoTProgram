#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
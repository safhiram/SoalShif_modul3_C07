#include <termios.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

int getch(void);

void main(){
	
	key_t key = 737;
	int *food_stock;
	int input;
	int shmid= shmget(key, sizeof(int), IPC_CREAT| 0777);
	food_stock = shmat(shmid, NULL, 0);
	*food_stock= 0;
	
	while (1){
		system("clear");
		printf("Shop\n");
		printf("Stock : %d\n", *food_stock);
		printf("Input : \n");
		printf("1. Restock\n");
		printf("2. Exit\n");
		
		input = getch();
		
		if (input =='1'){
			*food_stock += 50;
		}
	else if (input =='2'){
		break;
	}
	}
	
	shmdt(food_stock);
	shmctl(shmid, IPC_RMID, NULL);
}

int getch(void){
	struct termios oldt, newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag = ~( ICANON | ECHO);
	tcsetattr ( STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
	return ch;
}


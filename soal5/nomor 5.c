
#include <termios.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <ctype.h>

int heal = 300, hung = 200, hyg = 100, food = 50, cooldown_bath = 0;
int enemy = 100, food_stock = 20, standb = 1, regen =1, mati =0;


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


void* hunger(void* arg){
	
	while(1){
		if(standb ==1){
			sleep(10);
			hung -=5;
		}
	}
}

void* hygiene(void* arg){
	while(1){
		if (standb==1){
			sleep(30);
			hyg -=10;
		}
	}
}

void* tambah_darah(void* arg){
	while(1){
		if (regen ==1){
			sleep(10);
			heal += 5;
			heal -= heal%300;
		}
	}
}

void* cooldown_mandi(void* arg){
	while(1){
		if (standb ==1){
			if (cooldown_bath >0){
				sleep(1);
				cooldown_bath--;
			}
		}
	}
}

void battle(){
	char input;
	standb = 0;
	regen = 0;
	while (1){
		if(enemy <=0){
			printf("winn!!\n");
			enemy =100;
			break;
		}
	printf("battle mode\n");
	printf("your health : %d\n", heal);
	printf("enemy's health : %d\n", enemy);
	printf("input :\n");
	printf("1. attack\n");
	printf("2. run\n");
	
	input= getch();
	
	if (input == '1'){
		enemy -=20;
		if (enemy >0){
			heal -=20;
			if (heal <= 0){
				system("clear");
				mati =1;
				break;
			}
		}
	system("clear");
	}
else if (input =='2'){
	enemy =100;
	system("clear");
	break;
}		
	}
}

void shop(){
	key_t key = 737;
	int *food_stock;
	int input;
	int shmid= shmget(key, sizeof(int), IPC_CREAT| 0777);
	food_stock = shmat(shmid, NULL, 0);
	
	while (1){
		system("clear");
		
		 regen = 0;
		printf("shop mode\n");
		printf("shop's food stock : %d\n", *food_stock);
		printf("your   food stock : %d\n", food);
		printf("input\n1. Buy\n2. Back\n");
		input = getch();
		if (input =='1'){
			if (*food_stock >=5){
				food += 5;
				*food_stock -=5;
			}
		else{
			system("clear");
			printf("insufficient stock\n");
			sleep(1);
		}
		}
	else if (input =='2'){
		system("clear");
		break;
	}	
	}
	shmdt(food_stock);
	shmctl(shmid, IPC_RMID, NULL);
}

void mode_standby(){
	char input;
	while (2){
		standb =1;
		regen =1;
		system("clear");
	
	if (mati ==1){
		printf("GAME OVER\n");
		sleep(1);
		break;
	}
	
	printf("mode standby");
	printf("Heatlh : %d\n", heal);
	printf("Hunger : %d\n", hung);
	printf("Hygiene : %d\n", hyg);
	printf("Food  left : %d\n", food);
	if (cooldown_bath>0){
		printf("Bath will be ready in %ds\n", cooldown_bath );
		
	}
	else{
		printf("Bath is ready\n");
		}
		printf("input : \n");
		printf("1. Eat\n");
		printf("2. Bath\n");
		printf("3. Battle\n");
		printf("4. Shop\n");
		printf("5. Exit\n");
		
		if (hung <=0 || hyg <= 0){
			printf("GAME OVER\n");
			break;
		}
		input = getch();
		if (input =='1'){
			if (food >0){
				food -=1;
				hung += 15;
				hung -= hung%200;
			}
		}
		
	else	if (input == '2'){
			if (cooldown_bath== 0){
				hyg +=30;
				cooldown_bath =20;
				hyg -= hyg%100;
				
			}
		}
	else if (input  == '3'){
		system("clear");
		battle();
	}	
	else if (input =='4'){
		system("clear");
		shop();
			}
	else if (input=='5'){
		break;
	}
	}
}

int main(){
	char monster[50];
	pthread_t tid[5];
	
	system("clear");
	printf("Monster name : ");
	scanf("%s", monster);
	sleep(1);
		pthread_create(&(tid[0]), NULL, hunger, NULL);
		pthread_create(&(tid[1]), NULL, hygiene, NULL);
		pthread_create(&(tid[2]), NULL, tambah_darah, NULL);
		pthread_create(&(tid[3]), NULL, cooldown_mandi, NULL);
		system("clear");
		mode_standby();
}

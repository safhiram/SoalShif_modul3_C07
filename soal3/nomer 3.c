

#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int wakeup_status = 0, spirit_status = 100, input_agmal= 0;
int input_iraj = 0, count_agmal = 0, count_iraj = 0;
int game_on = 1;

void *agmal(){

while (wakeup_status <100){

	if (input_agmal == 1 && count_iraj!=3){
	
		wakeup_status += 15;
		input_agmal = 0;
		
		if (wakeup_status >= 100)
	{	
		printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
		game_on =0;
			exit(1);
	} 
								}
else
	if (count_iraj ==3){
		count_iraj = 0;
			printf("Agmal Ayo Bangun disabled 10 s\n");
		for (int i=0; i<10; i++){
			if (input_agmal ==1)
			{printf("Agmal Ayo Bangun disabled 10 s\n");
			input_agmal =0;
				}
			sleep(1);	
		}
	}	
	
	}
}


void *iraj(){
	
	while (spirit_status >0){
	
		if (input_iraj == 1 && count_agmal!=3){

		spirit_status -= 20;
		input_iraj = 0;
		
		if (spirit_status <= 0)
	{	
		printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
		game_on =0;
			exit(1);
	}
									}
else
	if (count_agmal ==3){
		count_agmal = 0;
			printf("Iraj ayo tidur disabled 10 s\n");
		for (int i=0; i<10; i++){
			if (input_iraj ==1)
			{printf("Iraj Ayo Tidur disabled 10 s\n");
			input_iraj =0;
				}
			sleep(1);	
		}
	}
}
}


int main(){
	
	pthread_t thread[2];
	
	int cek1, cek2; 
	 if(pthread_create(&thread[0],NULL,iraj,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&thread[1],NULL,agmal,NULL)){
        printf("failed\n");
    }
	
	char input[122];

	while (game_on ==1){
		scanf("%s", input);
		if (strcmp(input, "all") ==0){
			printf("Agmal WakeUp_Status = %d\n", wakeup_status);
			printf("Iraj Spirit_Status = %d\n", spirit_status);
		}
	else if (strcmp(input, "agmal") == 0)
		{input_agmal =1; count_agmal++;}
	else if (strcmp(input, "iraj") == 0)
		{input_iraj=1; count_iraj++;}
			}
	 

	exit(1);
	return 0;
}


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


int agmal_bangun, iraj_tidur = 0,  status_bangun = 0, status_spirit = 100;
int game_over = 0, iraj_count=0, agmal_count=0;



void *agmal(void *ptr){

    while(status_bangun < 100 && status_spirit > 0){

        if(agmal_bangun && iraj_count< 3){
            agmal_bangun = 0;
            status_bangun += 15;
            agmal_count++;
            if(status_bangun >= 100){
                printf("Agmal Terbangun,mereka bangun pagi dan berolahraga\n");
                exit(1);
            }
        }


    if(iraj_count >= 3){

        printf("Agmal Ayo Bangun disabled 10 s\n");
        for(int i=0; i<10; i++){

        if(iraj_count >= 6){
            iraj_count = 3;
                i = 0;
            printf("Agmal Ayo Bangun disabled 10 s\n");
                }

          if(agmal_bangun){
              agmal_bangun = 0;
                  printf("Agmal Ayo Bangun disabled 10 s\n");
                }

             sleep(1);
            }

        iraj_count = 0;
        agmal_bangun = 0;
               printf("Fitur agmal aktif kembali\n");
                   }
                                   }

    game_over = 1;
    
    
}

void *iraj(void *ptr){

    while(status_bangun < 100 && status_spirit > 0){

        if(iraj_tidur && agmal_count < 3){
            iraj_tidur = 0;
            status_spirit -= 20;
            iraj_count++;
            if(status_spirit <= 0){
                printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                exit(1);
            }
        }


        if(agmal_count >= 3){

            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
            for(int i=0; i<10; i++){

                if(agmal_count >= 6){
                    agmal_count = 3;
                    i=0;
                    printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
                }

                if(iraj_tidur){
                    iraj_tidur = 0;
                    printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
                }
                sleep(1);
            }

            iraj_tidur = 0;
            agmal_count = 0;

            printf("Fitur Iraj aktif kembali\n");

        }
        
            }

    game_over = 1;

}


int main(){

    pthread_t thread[3];

    if(pthread_create(&thread[0],NULL,iraj,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&thread[1],NULL,agmal,NULL)){
        printf("failed\n");
    }
    
    
   char input[121];

    while(game_over==0){

        scanf("%s", input);

        if(strcmp(input,"all")==0){
            printf("Agmal WakeUp_Status = %d\n",status_bangun);
            printf("Iraj Spirit_Status = %d\n",status_spirit);
        }else if(strcmp(input,"agmal")==0){
            agmal_bangun = 1;
        }else if(strcmp(input,"iraj")==0){
            iraj_tidur = 1;
        }
  }

     exit(1);
    return 0;
}

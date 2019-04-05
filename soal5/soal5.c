#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/ioctl.h>


int scene = 1;
int isBath = 0;
int hunger_status = 200;
int hygiene_status = 100;
int health_status = 300;
int bathCooldown=20;
int gameOver = 0;
char monsterName[100];
int playerFoodStock=10;
int enemyHealth = 100;
int *shopFoodStock;
int maxHunger = 200;





void *hunger(void *ptr){
    while(1){
        sleep(10);
        if(scene == 1){
            hunger_status -= 5;
        }
    }
}

void *hygiene(void *ptr){
    while(1){
        sleep(30);
        if(scene == 1){
            hygiene_status -= 10;
        }
    }
}

void *health(void *ptr){
    while(1){
        sleep(10);
        if(scene == 1){
            health_status += 5;
        }
    }
}

void *bath(void *ptr){

    while(1){
        if(isBath == 0){
            for(int i=0; i<20; i++){
                bathCooldown--;
                sleep(1);
            }
            isBath = 1;
        }
    }

}

void *scenePlay(void *ptr){

    while(gameOver==0){
        if(scene == 1){

            printf("StandbyMode\n");
            printf("Health : %d\n",health_status);
            printf("Hunger : %d\n",hunger_status);
            printf("Hygiene : %d\n",hygiene_status);
            printf("Food left : %d\n",playerFoodStock);
            if(isBath == 0){
                printf("Bath will be ready in %ds\n",bathCooldown);
            }else{
                printf("Bath is ready\n");
            }
            printf("Choices\n");
            printf("1. Eat\n");
            printf("2. Bath\n");
            printf("3. Battle\n");
            printf("4. Shop\n");
            printf("5. Exit\n");

        }else if(scene == 2){

            printf("Battle Mode\n");
            printf("%s's Health : %d\n",monsterName,health_status);
            printf("Enemy's Health : %d\n",enemyHealth);
            printf("Choices\n");
            printf("1. Attack\n");
            printf("2. Run\n");

        }else if(scene == 3){
            printf("Shop Mode\n");
            printf("Shop food stock : %d\n",*shopFoodStock);
            printf("Your food stock : %d\n",playerFoodStock);
            printf("Choices\n");
            printf("1. Buy\n");
            printf("2. Back\n");
        }

        if(hunger_status<=0){
            printf("%s died because hungry\n",monsterName);
        
            exit(1);
        }else if(hygiene_status<=0){
            printf("%s died because dirty\n",monsterName);
     
            exit(1);
        }

        sleep(1);
        system("clear");
    
    }

}

void input(){

    char input;
 
    while (1) {
        
        if(!2){
            
            input = getchar();
            if(scene == 1){

                if(input == '1'){
                    playerFoodStock--;
                    hunger_status+=15;
                    if(hunger_status>=maxHunger){
                        hunger_status = 200;
                    }
                }else if(input == '2'){
                    if(isBath){
                        hygiene_status+=30;
                        bathCooldown=20;
                        isBath = 0;
                    }
                }else if(input == '3'){
                    scene = 2;
                }else if(input == '4'){
                    scene = 3;
                }else if(input == '5'){
                 
                    exit(1);
                }

            }else if(scene == 2){

                if(input == '1'){
                    health_status-=20;
                    enemyHealth-=20;

                    if(health_status<=0){
                        printf("%s has been killed by enemy\n",monsterName);
                      
                        exit(1);
                    }else if(enemyHealth <= 0){
                        scene = 1;
                        enemyHealth = 100;
                        printf("%s win the battle\n",monsterName);
                    }

                }else if(input == '2'){
                    scene = 1;
                }


            }else if(scene == 3){
               if(input == '1'){
                   playerFoodStock++;
                   *shopFoodStock=*shopFoodStock-1;
               }else if(input == '2'){
                   scene = 1;
               }
            }

        }
        

    }

}


int main(){

    system("clear");
    pthread_t thread[5];

    key_t key = 1234;
    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);


    *shopFoodStock = 100;

    {printf("Whats your monster name : ");
	scanf("%s", monsterName);
    system("clear");
  


    if(pthread_create(&thread[0],NULL,scenePlay,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&thread[1],NULL,hunger,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&thread[2],NULL,hygiene,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&thread[3],NULL,health,NULL)){
        printf("failed\n");
    }

    if(pthread_create(&thread[4],NULL,bath,NULL)){
        printf("failed\n");
    }


    input();

    shmdt(shopFoodStock);
    shmctl(shmid, IPC_RMID, NULL);


        
    exit(1);
    return 0;
}


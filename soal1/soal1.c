
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<math.h>
#include <sys/ipc.h>



int bil[55];
pthread_t tid[55];
int x;


void* faktorial(){
     int hasil = 1;
        for(int i=1; i<=bil[x]; ++i)
        {
            hasil *= i;
        }
    printf("%d! = %u \n",bil[x],hasil);
}



int main(int argc, char** argv)
{

        int temp, coba;
        for (int i=0; i<argc-1; i++){
                bil[i]=atoi(argv[i+1]);
        }

for (int i=0; i<argc-1; ++i){
     for (int j=0; j<argc-i-1; ++j){
        if (bil[j] < bil[j-1]){
        temp = bil[j-1];
        bil[j-1]= bil[j];
        bil[j]= temp;
        }
}
}


for(x=0;x<argc-1;x++)
        {
                coba=pthread_create(&(tid[x]),NULL,&faktorial,NULL);
                if(coba!=0)
                {
                        printf("\nerror");
                }

                pthread_join(tid[x],NULL);

        }
        return 0;

}


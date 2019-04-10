
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
#define MAX 500

int input[777];
pthread_t tid[777];
int k;


int multiply(int x, int res[], int res_size) 
{ 
    int carry = 0;  
  
  
    for (int i=0; i<res_size; i++) 
    { 
        int prod = res[i] * x + carry; 

        res[i] = prod % 10;   
        carry  = prod/10;     
    } 
    while (carry) 
    { 
        res[res_size] = carry%10; 
        carry = carry/10; 
        res_size++; 
    } 
    return res_size; 
} 
  
void factorial(int n) 
{ 
    int res[MAX]; 
  
    
    res[0] = 1; 
    int res_size = 1; 
  
  
    for (int x=2; x<=n; x++) 
        res_size = multiply(x, res, res_size); 
  
for (int i=res_size-1; i>=0; i--) 
       printf("%d", res[i]);;
        
		printf("\n");
} 

  
  void* hasil(){
	printf("%d! = ", input[k]);  
   	 factorial(input[k]);
}

   int main(int argc, char** argv){
		
		int	temp ;
		for (int i =0; i<argc-1; i++){
			input[i]= atoi(argv[i+1]);
		}
		
	for(int i=0; i<argc-2; i++){
		int index_min = i;
		for (int j=i+1; j<argc-1; j++){
				if (input[j] < input[index_min])
				index_min = j;
			}
			temp = input[index_min];
			input[index_min] = input[i];
			input[i] = temp;
			}
	
	
	

	
	for ( k =0; k<argc-1; k++){
		int coba = pthread_create(&(tid[k]), NULL, &hasil, NULL);
		
		if(coba != 0)
			printf("\nerror");
			
			pthread_join(tid[k], NULL);
	}
	
	return 0;	
   } 

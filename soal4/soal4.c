#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>

struct Nilai{
	int n;
};

pthread_t tid[4];
char perintah[100];
int listnya=10;
int status=0;
int angka;
void *unzipya(void *args)
{
	pthread_t id = pthread_self();
	int iter;
        struct Nilai *nam = (struct Nilai*)args;
        if (pthread_equal(id,tid[1]) || pthread_equal(id,tid[2]))
       {
                        	sprintf(perintah,"unzip -qd ~/Documents/FolderProses%d ~/Documents/FolderProses%d/KompresProses%d.zip",nam->n,nam->n,nam->n);
                        	system(perintah);
				status=1;
				while(status!=1){}
				sprintf(perintah,"rm ~/Documents/FolderProses%d/KompresProses%d.zip",nam->n,nam->n);
				system(perintah);
				status=0;
        }

}

void *unzipya2(void *args)
{
	pthread_t id = pthread_self();
	int iter;
        struct Nilai *nam = (struct Nilai*)args;
        if (pthread_equal(id,tid[1]) || pthread_equal(id,tid[2]))
       {
                        	sprintf(perintah,"unzip -qd ~/Documents/FolderProses%d ~/Documents/FolderProses%d/KompresProses%d.zip",nam->n,nam->n,nam->n);
                        	system(perintah);
				status=1;
				while(status!=1){}
				sprintf(perintah,"rm ~/Documents/FolderProses%d/KompresProses%d.zip",nam->n,nam->n);
				system(perintah);
				status=0;
        }

}

void *jalanperintah(void *args)
{
	pthread_t id = pthread_self();
	int iter;
        struct Nilai *nam = (struct Nilai*)args;
	if(pthread_equal(id,tid[1]) || pthread_equal(id,tid[2]))
	{
	//struct Nilai *nam = (struct Nilai*)args;
	//create folder
	snprintf(perintah, sizeof(perintah), "mkdir /home/safhiram/Documents/FolderProses%d",nam->n);
	system(perintah);

	//menampilkan list
	snprintf(perintah,sizeof(perintah), "ps aux | head -n %d > /home/safhiram/Documents/FolderProses%d/SimpanProses%d.txt",listnya,nam->n,nam->n);
	system(perintah);


	//zipfile
	snprintf(perintah,sizeof(perintah), "zip -qmj ~/Documents/FolderProses%d/KompresProses%d ~/Documents/FolderProses%d/SimpanProses%d.txt",nam->n,nam->n,nam->n,nam->n);
	system(perintah);

	//delete file simpanproses
	//snprintf(perintah, sizeof(perintah), "rm /home/safhiram/Documents/FolderProses%d/SimpanProses%d.txt",nam->n,nam->n);
	//system(perintah);
	}

}

int main()
{
	struct Nilai yap;
	yap.n=0;
	int i=1;
	while(i<3)
	{
		yap.n=i;
		pthread_create(&tid[i],NULL, jalanperintah,(void *)&yap);
		pthread_join(tid[i],NULL);
		i++;
	}
	i=1;
	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
	sleep(15);
	yap.n=1;
	pthread_create(&tid[1],NULL, unzipya,(void *)&yap);
	pthread_join(tid[1],NULL);

	yap.n=2;
	pthread_create(&tid[2],NULL, unzipya2,(void *)&yap);
	pthread_join(tid[2],NULL);
exit(0);
}

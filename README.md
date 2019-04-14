
<h3>NOMER1</h3>
<p>Buatlah program C yang bisa menghitung faktorial secara parallel lalu menampilkan hasilnya secara berurutan
Contoh:
	./faktorial 5 3 4
	3! = 6
	4! = 24
	5! = 120</p>
<h3>JAWABAN</h3>

<p>1. buat array untuk menyimpan input</p>

```
        int temp, coba;
        for (int i=0; i<argc-1; i++){
                bil[i]=atoi(argv[i+1]);
        }
```

<p>2. sort input dari nilai yang terkecil ke yang terbesar</p>

```
for (int i=0; i<argc-1; ++i){
     for (int j=0; j<argc-i-1; ++j){
        if (bil[j] < bil[j-1]){
        temp = bil[j-1];
        bil[j-1]= bil[j];
        bil[j]= temp;
        }
}
```

<p>3. buat thread untuk menghitung nulai faktorial agar bisa dilakukan secara bersamaan</p>

```
coba=pthread_create(&(tid[x]),NULL,&faktorial,NULL);

//fungsi faktorial
void* faktorial(){
     int hasil = 1;
        for(int i=1; i<=bil[x]; ++i)
        {
            hasil *= i;
        }
    printf("%d! = %u \n",bil[x],hasil);
}
```

<h3>NOMER2</h3>
<p>Pada suatu hari ada orang yang ingin berjualan 1 jenis barang secara private, dia memintamu membuat program C dengan spesifikasi sebagai berikut:
Terdapat 2 server: server penjual dan server pembeli
1 server hanya bisa terkoneksi dengan 1 client
Server penjual dan server pembeli memiliki stok barang yang selalu sama
Client yang terkoneksi ke server penjual hanya bisa menambah stok
Cara menambah stok: client yang terkoneksi ke server penjual mengirim string “tambah” ke server lalu stok bertambah 1
Client yang terkoneksi ke server pembeli hanya bisa mengurangi stok
Cara mengurangi stok: client yang terkoneksi ke server pembeli mengirim string “beli” ke server lalu stok berkurang 1
Server pembeli akan mengirimkan info ke client yang terhubung dengannya apakah transaksi berhasil atau tidak berdasarkan ketersediaan stok
Jika stok habis maka client yang terkoneksi ke server pembeli akan mencetak “transaksi gagal”
Jika stok masih ada maka client yang terkoneksi ke server pembeli akan mencetak “transaksi berhasil”
Server penjual akan mencetak stok saat ini setiap 5 detik sekali
Menggunakan thread, socket, shared memory</p>
<h4>JAWABAN</h4>
<p>*Server Penjual</p>
<p>1. buat variabel shared memory stok untuk mengetahui jumlah stok saat ini.</p>

```
	key_t key = 1234;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stok = shmat(shmid,NULL,0);
```

<p>2. buat thread untuk menampikan jumlah stok setiap 5 detik sekali. dan buatlah fungsi untuk menampilkan jumlah stok tersebut.</p>

```
 err=pthread_create(&(tid1),NULL,cekstok,NULL);

 //fungsi cekstok
void* cekstok(void *arg)
{
	pthread_t id = pthread_self();
	unsigned long long int iter;
		for(iter=0;iter<10000000;iter++)
		{
			printf("Stok = %d\n",*stok);
			sleep(5);	
		}
} 

```

<p>3. setiap adanya input yang masuk maka buat sebuah thread untuk menjalankan fungsi tambah stok. Di dalam fungsi tambahstok, awalnya kita cek dulu apakah input == "tambah". Jika iya, maka melakukan penambahakan stok.</p>

```
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))) {
	int *now=malloc(sizeof(*now));
	*now=new_socket;
	pthread_t tid;
	if(new_socket>0)
	{
       	err=pthread_create(&(tid),NULL,tambahstok,now);
	}

	//fungsi tambah stok
	void* tambahstok(void* sock){
	int valread;
	int yap=*(int*)sock;
	char buffer[1024]={0};
	while(valread=read(yap,buffer,1024))
	{
	    	if(strcmp(buffer,"tambah")==0){
				*stok+=1;
	    	}
		memset(buffer,0,1024);
	}
}
```

<p>*Client Penjual</p>
<p>1. masukkan input berupa string</p>

```
    scanf("%s",bisa);
```

<p>2. lakukan pengiriman hasil input tersebut ke server</p>

```
    while(send(sock, bisa, strlen(bisa),0))
	{
    		memset(bisa, 0 , sizeof(bisa));
		scanf("%s",bisa);
	}
```

<p>*Server Pembeli</p>
<p>1. buat variabel shared memory stok untuk mengetahui jumlah stok saat ini.</p>

```
	key_t key = 1234;
	int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
	stok = shmat(shmid,NULL,0);
```

<p>2. setiap adanya input yang masuk maka buat sebuah thread untuk menjalankan fungsi tambah stok. Di dalam fungsi tambahstok, awalnya kita cek dulu apakah input == "beli". Jika iya, maka melakukan pengurangan stok. Namun sebelumnya cek stok, jika stok=0 send pesan ke client 'Transasi Gagal!' , jika stok !=0 send pesan ke client 'Transaksi Berhasil!'.</p>

```
    while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))) {
	int *now=malloc(sizeof(*now));
	*now=new_socket;
	pthread_t tid;
	if(new_socket>0)
	{
       	err=pthread_create(&(tid),NULL,tambahstok,now);
	}

	//fungsi kurang stok
	void* sock){
	int valread;
	int yap=*(int*)sock;
	char buffer[1024]={0};
	while(valread=read(yap,buffer,1024))
	{
	    	if(strcmp(buffer,"beli")==0){
			if(*stok==0)
			{
				strcat(message,"Transaksi Gagal!");
				status=1;
				send(yap,message,strlen(message),0);
			}
			else
			{
				*stok-=1;
				strcat(message,"Transaksi Berhasil");
				send(yap,message,strlen(message),0);
				status=1;
			}
	    	}
		memset(buffer,0,1024);
		memset(message,0,1024);
	}
}
}
```

<p>*Client Pembeli</p>
<p>1. masukkan input berupa string</p>

```
    scanf("%s",bisa);
```

<p>2. lakukan pengiriman hasil input tersebut ke server</p>

```
    while(send(sock, bisa, strlen(bisa),0))
	{
    		memset(bisa, 0 , sizeof(bisa));
		scanf("%s",bisa);
	}
```

<p>3. jika ada pesan yang masuk , maka akan di read lalu ditampilkan</p>

```
    			valread=read(sock,buffer,1024);
    			printf("%s\n",buffer);
```

<h3>NOMER3</h3>	
<p>Agmal dan Iraj merupakan 2 sahabat yang sedang kuliah dan hidup satu kostan, sayangnya mereka mempunyai gaya hidup yang berkebalikan, dimana Iraj merupakan laki-laki yang sangat sehat,rajin berolahraga dan bangun tidak pernah kesiangan sedangkan Agmal hampir menghabiskan setengah umur hidupnya hanya untuk tidur dan ‘ngoding’. Dikarenakan mereka sahabat yang baik, Agmal dan iraj sama-sama ingin membuat satu sama lain mengikuti gaya hidup mereka dengan cara membuat Iraj sering tidur seperti Agmal, atau membuat Agmal selalu bangun pagi seperti Iraj. Buatlah suatu program C untuk menggambarkan kehidupan mereka dengan spesifikasi sebagai berikut:
Terdapat 2 karakter Agmal dan Iraj
Kedua karakter memiliki status yang unik
Agmal mempunyai WakeUp_Status, di awal program memiliki status 0
Iraj memiliki Spirit_Status, di awal program memiliki status 100
Terdapat 3 Fitur utama
All Status, yaitu menampilkan status kedua sahabat
Ex: Agmal WakeUp_Status = 75 
      Iraj Spirit_Status = 30
“Agmal Ayo Bangun” menambah WakeUp_Status Agmal sebesar 15 point
“Iraj Ayo Tidur” mengurangi Spirit_Status Iraj sebanyak 20 point
Terdapat Kasus yang unik dimana:
Jika Fitur “Agmal Ayo Bangun” dijalankan sebanyak 3 kali, maka Fitur “Iraj Ayo Tidur” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Fitur Iraj Ayo Tidur disabled 10 s”)
Jika Fitur  “Iraj Ayo Tidur” dijalankan sebanyak 3 kali, maka Fitur “Agmal Ayo Bangun” Tidak bisa dijalankan selama 10 detik (Dengan mengirim pesan ke sistem “Agmal Ayo Bangun disabled 10 s”)
Program akan berhenti jika Salah Satu :
WakeUp_Status Agmal >= 100 (Tampilkan Pesan “Agmal Terbangun,mereka bangun pagi dan berolahraga”)
Spirit_Status Iraj <= 0 (Tampilkan Pesan “Iraj ikut tidur, dan bangun kesiangan bersama Agmal”)

Syarat Menggunakan Lebih dari 1 Thread</p>

<h3>JAWABAN</h3>
<p>1. buat dua thread yaitu thread untuk agmal dan iraj</p>
<p>2. di dalam fungsi agmal </p>
<p>set status_bangun=0 dan status spirit=100</p>

```
int status_bangun = 0, status_spirit = 100;
```

<p>setiap agmal dijalankan, maka akan tersimpan nilai count, jika agmal dijalankan kurang dari tiga kali, maka
status_bangun akan bertambah 15, jika status bangun lebih dari 100 maka print 'Agmal Terbangun,mereka bangun pagi dan berolahraga'</p>

```
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
```

<p>jika iraj_count sudah dijalankan lebih dari tiga kali maka agmal_bangun tidak bisa dijalankan selama 10 detik</p>

```
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
```

<p>jika iraj_count dan agmal_bangun sama dengan 0, maka fitur agmal akan aktif kembali.</p>

```
        iraj_count = 0;
        agmal_bangun = 0;
               printf("Fitur agmal aktif kembali\n");
```

<p>3. di dalam fungsi iraj</p>
<p>set status bangun =0 dan status spirit=100.</p>
<p>setiap iraj_tidur dijalankan, maka count iraj_tidur akan bertambah. dan nilai status spirit akan berkurang 20</p>

```
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
```

<p>jika agmal count sudah dijalankan 3 kali maka iraj_tidur tidak bisa dijalankan selama 10 detik.
setelah sleep selesai maka Fitur Iraj aktif kembali.</p>

```
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
```

<p>4. jika terjadi game over, maka input suatu string</p>
<p>jika string bernilai 'all', maka</p>

```
            printf("Agmal WakeUp_Status = %d\n",status_bangun);
            printf("Iraj Spirit_Status = %d\n",status_spirit);
```

<p>jika string bernilai agmal, maka ubah nilai agmal_bangun=1</p>

```
        }else if(strcmp(input,"agmal")==0){
            agmal_bangun = 1;
```

<p>jika string bernilai iraj, maka ubah nilai iraj_tidur=1</p>

```
        }else if(strcmp(input,"iraj")==0){
            iraj_tidur = 1;
        }
```

<h3>NOMER4</h3>	
<p>Buatlah sebuah program C dimana dapat menyimpan list proses yang sedang berjalan (ps -aux) maksimal 10 list proses. Dimana awalnya list proses disimpan dalam di 2 file ekstensi .txt yaitu  SimpanProses1.txt di direktori /home/Document/FolderProses1 dan SimpanProses2.txt di direktori /home/Document/FolderProses2 , setelah itu masing2 file di  kompres zip dengan format nama file KompresProses1.zip dan KompresProses2.zip dan file SimpanProses1.txt dan SimpanProses2.txt akan otomatis terhapus, setelah itu program akan menunggu selama 15 detik lalu program akan mengekstrak kembali file KompresProses1.zip dan KompresProses2.zip 
Dengan Syarat : 
Setiap list proses yang di simpan dalam masing-masing file .txt harus berjalan bersama-sama
Ketika mengkompres masing-masing file .txt harus berjalan bersama-sama
Ketika Mengekstrak file .zip juga harus secara bersama-sama
Ketika Telah Selesai melakukan kompress file .zip masing-masing file, maka program akan memberi pesan “Menunggu 15 detik untuk mengekstrak kembali”
Wajib Menggunakan Multithreading
Boleh menggunakan system</p>
<h3>JAWABAN</h3>
<p>1. buat thread untuk melakukan beberapa perintah</p>

```
	while(i<3)
	{
		yap.n=i;
		pthread_create(&tid[i],NULL, jalanperintah,(void *)&yap);
		pthread_join(tid[i],NULL);
		i++;
	}
```

<p>2. buat fungsi untuk melakukan perintah-perintah tersebut</p>

```
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
	}

}

```

<p>3. sleep selama 15 detik</p>

```
	printf("Menunggu 15 detik untuk mengekstrak kembali\n");
	sleep(15);
```

<p>4. buat thread untuk melakukan unzip. dan buat fungsi unzipnya</p>

```
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
```

<h3>NOMER5</h3>	
<p>Angga, adik Jiwang akan berulang tahun yang ke sembilan pada tanggal 6 April besok. Karena lupa menabung, Jiwang tidak mempunyai uang sepeserpun untuk membelikan Angga kado. Kamu sebagai sahabat Jiwang ingin membantu Jiwang membahagiakan adiknya sehingga kamu menawarkan bantuan membuatkan permainan komputer sederhana menggunakan program C. Jiwang sangat menyukai idemu tersebut. Berikut permainan yang Jiwang minta. 
Pemain memelihara seekor monster lucu dalam permainan. Pemain dapat  memberi nama pada monsternya.
Monster pemain memiliki hunger status yang berawal dengan nilai 200 (maksimalnya) dan nanti akan berkurang 5 tiap 10 detik.Ketika hunger status mencapai angka nol, pemain akan kalah. Hunger status dapat bertambah 15 apabila pemain memberi makan kepada monster, tetapi banyak makanan terbatas dan harus beli di Market.
Monster pemain memiliki hygiene status yang berawal dari 100 dan nanti berkurang 10 tiap 30 detik. Ketika hygiene status mencapai angka nol, pemain akan kalah. Hygiene status' dapat bertambah 30 hanya dengan memandikan monster. Pemain dapat memandikannya setiap 20 detik(cooldownnya 20 detik).
Monster pemain memiliki health status yang berawal dengan nilai 300. Variabel ini bertambah (regenerasi)daa 5 setiap 10 detik ketika monster dalam keadaan standby.
Monster pemain dapat memasuki keadaan battle. Dalam keadaan ini, food status(fitur b), hygiene status'(fitur c), dan ‘regenerasi’(fitur d) tidak akan berjalan. Health status dari monster dimulai dari darah saat monster pemain memasuki battle. Monster pemain akan bertarung dengan monster NPC yang memiliki darah 100. Baik monster pemain maupun NPC memiliki serangan sebesar 20. Monster pemain dengan monster musuh akan menyerang secara bergantian. 
Fitur shop, pemain dapat membeli makanan sepuas-puasnya selama stok di toko masih tersedia.
Pembeli (terintegrasi dengan game)
Dapat mengecek stok makanan yang ada di toko.
Jika stok ada, pembeli dapat membeli makanan.
Penjual (terpisah)
Bisa mengecek stok makanan yang ada di toko
Penjual dapat menambah stok makanan.
	Spesifikasi program:
Program mampu mendeteksi input berupa key press. (Program bisa berjalan tanpa perlu menekan tombol enter)
Program terdiri dari 3 scene yaitu standby, battle, dan shop.
Pada saat berada di standby scene, program selalu menampilkan health status, hunger status, hygiene status, stok makanan tersisa, dan juga status kamar mandi (“Bath is ready” jika bisa digunakan, “Bath will be ready in [bath cooldown]s” jika sedang cooldown). Selain itu program selalu menampilkan 5 menu, yaitu memberi makan, mandi, battle, shop, dan exit. Contoh :

Standby Mode
Health : [health status]
Hunger : [hunger status]
Hygiene : [hygiene status]
Food left : [your food stock]
Bath will be ready in [cooldown]s
Choices
Eat
Bath
Battle
Shop
Exit

Pada saat berada di battle scene, program selalu menampilkan health status milik pemain dan monster NPC. Selain itu, program selalu menampilkan 2 menu yaitu serang atau lari. Contoh :

Battle Mode
Monster’s Health : [health status]
Enemy’s Health : [enemy health status]
Choices
Attack
Run

Pada saat berada di shop scene versi pembeli, program selalu menampilkan food stock toko dan milik pemain. Selain itu, program selalu menampilkan 2 menu yaitu beli dan kembali ke standby scene. Contoh :

		Shop Mode
		Shop food stock : [shop food stock]
		Your food stock : [your food stock]
		Choices
Buy
Back

Pada program penjual, program selalu menampilkan food stock toko. Selain itu, program juga menampilkan 2 menu yaitu restock dan exit. Contoh :

Shop
Food stock : [shop food stock]
Choices
Restock
Exit

Pastikan terminal hanya mendisplay status detik ini sesuai scene terkait (hint: menggunakan system(“clear”))</p>
<p> Jawaban </p>

<p>1. Program ini memuat 4 fungsi utama yang nantinya akan dibuat melalui thread, agar bisa jalan di luar program yang sedang berlangsung, yaitu fungsi 'hunger', 'hygiene', 'tambah_darah', dan 'cooldown_mandi'</p>

```
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
```
<p>2. Keempat fungsi diatas dipanggil melalui thread yang kita buat di fungsi main(), dengan mendeklarasikan array threadnya terlebih dahulu, dan memasukan nama monster kita </p> 

```
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
```

<p>3. Ada 3, fungsi mode yang kita gunakan di program ini, untuk menampilkan interface saat program dijalankan dan menu menu yang dipilih oleh user</p>
<p> a. mode_standby. Mode ini adalah yang pertama kali muncul saat menjalankan program, disini ada 5 menu(input) yang bisa kita pilih, yaitu eat, bath, battle, shop, dan exit. Dimana eat dan bath untuk menambah stats sedangkan battle dan shop untuk pindah ke mode lain </p> 
```
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
``` 
<p> b. shop. Mode ini digunakan untuk membeli food stock yang kita punya, disini variabel fodd_stock di share isinya dengan program lain yang bernama ./shopee melalui share memory, supaya ketika food_stock yang ada di program utama bertambah atau berkurang, maka food_stock yang ada di program ./shopee juga berkurang. </p>

```
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
```
<p> c. battle(), mode ini dipanggil saat user ingin melakukan battle, hanya akan terjadi pengurangan health disini, yang apabila health kita habis, maka game akan berakhir </p>

``` 
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
```
<p>4. Diawal di deklarasikan variable variable yang akan kita gunakan di program ini, yaitu :</p>
<p> a. heal, hung, hyg, adalah status dari moster yang bisa bertambah dan berkurang </p>
<p> b. cooldown_bath, batas waktu mandi yang berjumlah 20 detik setiap kali kita mandi, dan bekrurang selama mode standby</p>
<p> c. enemy, adalah heatlh bar enemy saat battle. Yang awalnya 100, akan berkurang selama battle berlangsung. Dan akan kembali menjadi 100 saat battle selesai</p>
<p> d. food, jumlah makanan yang kita punya saat ini. Food_stock, jumlah makanan yang dimiliki toko, dan kita share variablenya dengan program ./shopee</p>
<p> e. standb, untuk mengecek apakah kita sedang ada di mode standby atau tidak. <p>
<p> f. regen, untuk mengecek apakah regen diperbolehkan  atau tidak. Regen menjadi = 0 ketika kita sedang ada dalam mode battle atau sudah mati</p>
<p> g. mati, sebagai checking yang apabila mati==0, maka program berhenti berjalan</p>

```
int heal = 300, hung = 200, hyg = 100, food = 50, cooldown_bath = 0;
int enemy = 100, food_stock = 20, standb = 1, regen =1, mati =0;
```

<p>5. ./shopee, adalah program lain diluar program utama yang dijalankan. Disini kita bisa menambah jumlah food_stock yang dimiliki toko makanan monster. Variabel food_stock di share dengan program utama menggunakan shared memory. Sama dengan pendeklarasian di fungsi 'shop' di program utama, menggunakan variable key dan shmid yang sama supaya bisa dihubungkan.</p>''

``` 	
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
```

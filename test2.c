#ifdef RaspberryPi

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include <mariadb/mysql.h>

char device[]="/dev/ttyACM0";

int fd;
int i = 0;
int q = 0;

unsigned long baud = 115200;
unsigned long timer = 0;

char Stringtemp[20] = {'\0',};
char buffer[100] = {'\0',};

char phbuf[5] = {'\0',};
char ecbuf[5] = {'\0',};
char tpbuf[6] = {'\0',};

int main();
void loop();
void setup();

typedef struct timetype {
   int hour, min;
}timetype;

timetype gettime() {
   timetype type;
   time_t     current_time;
   struct tm *today;
   time(&current_time);
   today = localtime(&current_time);
   printf("%2d:%2d:%2d\n", today->tm_hour, today->tm_min, today->tm_sec);
   type.hour = today->tm_hour;
   type.min = today->tm_min;
   return type;
}

void *itoa2(int number, char *str, int radix){
   int digits = 0;
   int divider = 1;
   int index = 0;

   while ((number / divider) > 0)
   {
      digits++;
      divider *= radix;
   }
   divider /= radix;
   for (; digits > 0; digits--)
   {
      *(str + index) = (number / divider) + '0';
      number -= ((number / divider) * divider);
      index++;
      divider /= radix;
   }
   *(str + index) = '\0';
   return 0;
}

void logger(timetype temp) {
   char hour[2];
   char min[2];
   itoa2(temp.hour, hour, 10);
   itoa2(temp.min, min, 10);
   if(hour[0]=='\0') {hour[0] = '0'; hour[1]='0';}
   if(min[0]=='\0') {min[0] = '0'; min[1] = '0';}
   printf("%s:%s\n", hour, min);
   if (hour[1] != '\0'&&min[1] != '\0') {
      Stringtemp[0] = 'O';
      Stringtemp[1] = 'u';
      Stringtemp[2] = 't';
      Stringtemp[3] = '/';
      Stringtemp[4] = hour[0];
      Stringtemp[5] = hour[1];
      Stringtemp[6] = 'h';
      Stringtemp[7] = min[0];
      Stringtemp[8] = min[1];
      Stringtemp[9] = 'm';
      Stringtemp[10] = '.';
      Stringtemp[11] = 't';
      Stringtemp[12] = 'x';
      Stringtemp[13] = 't';
      Stringtemp[14] = '\0';
   }
   else if(hour[1] == '\0'&&min[1]!='\0'){
      Stringtemp[0] = 'O';
      Stringtemp[1] = 'u';
      Stringtemp[2] = 't';
      Stringtemp[3] = '/';
      Stringtemp[4] = hour[0];
      Stringtemp[5] = 'h';
      Stringtemp[6] = min[0];
      Stringtemp[7] = min[1];
      Stringtemp[8] = 'm';
      Stringtemp[9] = '.';
      Stringtemp[10] = 't';
      Stringtemp[11] = 'x';
      Stringtemp[12] = 't';
      Stringtemp[13] = '\0';
   }else if(hour[1]=='\0'&&min[1]=='\0'){
      Stringtemp[0] = 'O';
      Stringtemp[1] = 'u';
      Stringtemp[2] = 't';
      Stringtemp[3] = '/';
      Stringtemp[4] = hour[0];
      Stringtemp[5] = 'h';
      Stringtemp[6] = min[0];
      Stringtemp[7] = 'm';
      Stringtemp[8] = '.';
      Stringtemp[9] = 't';
      Stringtemp[10] = 'x';
      Stringtemp[11] = 't';
      Stringtemp[12] = '\0';
   }if (hour[1] != '\0'&&min[1]=='\0') {
      Stringtemp[0] = 'O';
      Stringtemp[1] = 'u';
      Stringtemp[2] = 't';
      Stringtemp[3] = '/';
      Stringtemp[4] = hour[0];
      Stringtemp[5] = hour[1];
      Stringtemp[6] = 'h';
      Stringtemp[7] = min[0];
      Stringtemp[8] = 'm';
      Stringtemp[9] = '.';
      Stringtemp[10] = 't';
      Stringtemp[11] = 'x';
      Stringtemp[12] = 't';
      Stringtemp[13] = '\0';
   }
  
   printf("%s\n",Stringtemp);
   //FILE *fp;
   //fp = fopen(Stringtemp, "wb");
   //fputs(Stringtemp,fp);
   //fclose(fp);
}

void setup(){
	printf("%s \n","Start");
	fflush(stdout);

	if ((fd = serialOpen ( device, baud )) < 0){
	 fprintf (stderr,"Unable to open device: %s\n", strerror (errno));
	 exit(1);
	}

	if (wiringPiSetup () == -1){
	 fprintf(stdout, "Unable to start wiringPi: %s\n", strerror (errno));
	 exit(1);
	}

}

char newChar;

void loop(){
	if(millis()-timer>=3000){
	// serialPuts (fd, "Pong!\n");
	// serialPutchar (fd,65);
	 timer=millis();
	}
	
	if(serialDataAvail (fd)){
	 newChar = serialGetchar(fd);
	 //delay();
	 printf("%c",newChar);
	 fflush(stdout);
	}
}


int Write(){
	logger(gettime());
	FILE *fp = fopen(Stringtemp,"w");
	//FILE *fp = fopen("Out/bla.txt","w");
	setup();
	while(1){
		loop();
		if(newChar=='$') {q++; printf("action change%d\n",q);}
		if(q==1) if(newChar!='\0'||newChar!='\n'){
		       	//if(buffer[i]!=newChar) buffer[i+1] = newChar;
			fputc(newChar,fp);
			delay(10);
			//i++;
		}
		if(q==2) {q=0;break;}
		newChar = '\0';
	}
	fclose(fp);
	fflush(stdout);
}

void separateVal(char buf[]){
	phbuf[0] = buf[1]; phbuf[1] = buf[2]; phbuf[2] = buf[3]; phbuf[3] = buf[4]; phbuf[4] = '\0';
	ecbuf[0] = buf[5]; ecbuf[1] = buf[6]; ecbuf[2] = buf[7]; ecbuf[3] = buf[8]; phbuf[4] = '\0';
	tpbuf[0] = buf[9]; tpbuf[1] =buf[10]; tpbuf[2] =buf[11]; tpbuf[3] =buf[12]; tpbuf[4] = buf[13]; tpbuf[5] = '\0';
}

int main(){
	FILE *fps;
	MYSQL *con = mysql_init(NULL);
        MYSQL_RES *sql_result;
        char query[1024]={'\0',};
        char temp[80]={'\0',};
        printf("MySQL client Version %s\n",mysql_get_client_info());
	if (mysql_real_connect(con, "220.69.240.122", "root", "000000","vision", 0, NULL, 0) == NULL){
                fprintf(stderr, "%s\n", mysql_error(con));
                mysql_close(con);
                exit(1);
        }
        else {
                printf("You complete Connect!\n");
        }

	while(1){
		Write();
		fps = fopen(Stringtemp,"r");
		fgets(buffer,80,fps);
		separateVal(buffer);
		
		sprintf(query,"insert into data (phval,ecval,tpval) values('%s','%s','%s')",phbuf,ecbuf,tpbuf);
	        printf("query is %s\n",query);
        	
		if(mysql_query(con,query)==0){
               		printf("data inserted!\n");
        	}else{
                	printf("Insert failure!\n");
        	}

	}
	mysql_close(con);
	serialClose(fd);
	return 0;
}

#endif 

#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE *fpp=fopen("example.txt","w");
	FILE *fps;
	char exmp[] = "helpME!\nrecue!";
	char temp[20] = {'\0',};
	fputs(exmp,fpp);
	fclose(fpp);

	fps = fopen("example.txt","r");
	fgets(temp,20,fps);
	printf("%s",temp);
	fclose(fps);
	return 0;
}

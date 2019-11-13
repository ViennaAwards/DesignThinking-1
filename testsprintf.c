#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){
	char temp1[] = "bla1";
	char temp2[] = "bla2";
	char temp3[20];
       sprintf(temp3,"bla3 %s %s",temp1,temp2);
	printf("%s",temp3);       
}

#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main(int argc, char **argv){
	MYSQL *con = mysql_init(NULL);
	MYSQL_RES *sql_result;
	printf("MySQL client Version %s\n",mysql_get_client_info());
	if (con == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "220.69.240.122", "root", "000000","vision", 0, NULL, 0) == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	else {
		printf("You complete Connect!\n");
	}
	if(mysql_query(con,"insert into login (id,pw) values('GeeksWay2','testPW2')")==0){
		printf("data inserted!\n");
	}else{
		printf("failure!\n");
	}
	if(mysql_query(con, "select * from login")==0){
		sql_result = mysql_store_result(con);
		printf("%c\n",sql_result);
	}else{
		printf("Select failure\n");
	}
	if(mysql_query(con, "delete from login")==0){
		printf("delete complete!\n");
	}else{
		printf("delete FAILURE!!!\n");
	}
	mysql_close(con);
	return 0;
}

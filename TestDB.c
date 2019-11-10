#include <stdio.h>
#include <stdlib.h>
#include <mariadb/mysql.h>

int main(int argc, char **argv){
	MYSQL *con = mysql_init(NULL);
	MYSQL_RES *sql_result;
	if (con == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "220.69.240.122", "root", "000000",NULL, 0, NULL, 0) == NULL){
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		exit(1);
	}
	else {
		printf("You complete Connect!\n");
	}
	if(mysql_query(con,"insert into 'vision'.'login' ('id','pw') values('GeeksWay','testPW')")!=0){
		printf("data inserted!\n");
	}else{
		printf("failure!\n");
	}
	if(mysql_query(con, "select * from 'vision'.'login'")!=0){
		sql_result = mysql_store_result(con);
		printf("%s\n",sql_result);
	}else{
		printf("Select failure\n");
	}
	if(mysql_query(con, "DELETE FROM 'vision'.'login'")!=0){
		printf("delete complete!\n");
	}else{
		printf("delete FAILURE!!!\n");
	}
	mysql_close(con);
	return 0;
}

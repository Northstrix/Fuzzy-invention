// Encrypted note storage
// Distributed under the MIT License
// © Copyright Maxim Bortnikov 2021
// For more information please visit
// https://github.com/Northstrix/Fuzzy-invention
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sqlite3.h" 

static int csv(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   int c = 0;
   FILE *fp;
   fp = fopen("Content.csv", "a+");
   for(i = 0; i<argc; i++) {
      fprintf(fp, "%s,%s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	c++;
      if(c%2 == 0){
      	fprintf(fp, "\n");
	  }
   }
   fclose(fp);
   return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void create_table(){
	   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("notes.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }

   /* Create SQL statement */
   sql = "CREATE TABLE DATA("  \
      "LABEL           TEXT    NOT NULL," \
      "CONTENT        TEXT     NOT NULL);" ;

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Table created successfully\n");
   }
   sqlite3_close(db);
	return;
}

void insert(){
	   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;

   /* Open database */
   rc = sqlite3_open("notes.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }
	char label[1000], content[25000];
	fprintf(stdout, "Enter the label: ");
    scanf("%s", label);
	fprintf(stdout, "Enter the content: ");
    scanf("%s", content);
    char sql[26000] = "INSERT INTO DATA (LABEL,CONTENT)VALUES ('";
	char sql2[] = "', '";
	char sql3[] = "');";
	strcat(sql, label);
	strcat(sql, sql2);
	strcat(sql, content);
	strcat(sql, sql3);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Records created successfully\n");
   }
   sqlite3_close(db);
   return;
}

void disp(){
	sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("notes.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   } else {
      fprintf(stderr, "The content of the database:\n\n");
   }

   /* Create SQL statement */
   sql = "SELECT * from DATA";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "///The end///\n");
   }
   sqlite3_close(db);
   return;
}

void write_csv(){
	sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("notes.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }
   /* Create SQL statement */
   sql = "SELECT * from DATA";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, csv, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {

   }
   sqlite3_close(db);
   return;
}

void remove_record(){
	sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("notes.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }
	char rm[100];
	fprintf(stdout, "Enter the label: ");
    scanf("%s", rm);
   /* Create merged SQL statement */
   char sql[100] = "DELETE from DATA where LABEL='";
   char sql1[] = "'; ";
   	strcat(sql, rm);
	strcat(sql, sql1);
   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   sqlite3_close(db);
   return;
}

void update_content(){
	sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   const char* data = "Callback function called";

   /* Open database */
   rc = sqlite3_open("notes.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

   } 
   /* Create merged SQL statement */
   char sql[150] = "UPDATE DATA set CONTENT = '";
   char sql1[] = "' where LABEL='";
   char sql2[] = "'; ";
   	char nm[100], am[10];
	fprintf(stdout, "Enter the label: ");
    scanf("%s", nm);
	fprintf(stdout, "Enter the content: ");
    scanf("%s", am);
   	strcat(sql, am);
	strcat(sql, sql1);
	strcat(sql, nm);
	strcat(sql, sql2);

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
   if( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   sqlite3_close(db);
}

int main(int argc, char* argv[]) {
	   int x = 0;
	   while (x !=7) {
		   printf("\nWhat do you want to do?\n1.Create the table\n2.Add record\n3.Remove record\n4.Display all records\n5.Export all records to the Content.csv\n6.Update the content of a record\n7.Quit\n");
		   scanf("%d", &x);
	   
	   		   if (x == 1) {
			   create_table();
		   }
		   
		   if (x == 2) {
			   insert();
		   }
		   
		   if (x == 3) {
			   remove_record();
		   }
		   
		   if (x == 4) {
			   disp();
		   }
		   
		   if (x == 5) {
			   write_csv();
		   }
		   
		   if (x == 6){
		   	update_content();
		   }

	   
	}
}
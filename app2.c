#include "mariadb/mysql.h"
#include "mysql/mysql.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define DIR_TYPE 4
#define FILE_TYPE 8
#define FILE_LINK_TYPE 10

char **fileList = NULL;
int testingMysql(void);
int searchFiles(char ***fileList, DIR *dir, char *mainDir, char *filename, int parent_lvl);

int main(int argc, char *argv[]) {

  if(argc < 3){
    printf("You must pass the folder and file to search\n");
    printf("The call must have 2 parameters\n");
    return EXIT_FAILURE;
  }
  
  fileList = (char**)calloc(1,sizeof(char*));

  if( searchFiles(&fileList, opendir(*(argv+1)), *(argv+1), *(argv+2), 0) == 1){
    while(*fileList != NULL){
      printf("%s\n", *fileList);
      fileList++;
    }
  }

  return EXIT_SUCCESS;

}

int searchFiles(char ***fileList, DIR *dir, char *mainDir, char *filename, int parent_lvl){
  int result = 0;
  if(dir == NULL){
    return 0;
  }
  struct dirent *dIrent;
  while((dIrent = readdir(dir)) != NULL){
    if(( strcmp(dIrent->d_name, "..") != 0 ) && ( strcmp(dIrent->d_name, ".") != 0 )){
      char *str_dir = (char*)malloc(strlen(mainDir)+2+strlen(dIrent->d_name));
      if(dIrent->d_type == DIR_TYPE){
        strcpy(str_dir, mainDir);
        strcat(str_dir, "/");
        strcat(str_dir, dIrent->d_name);
        DIR *currentDir = opendir(str_dir);
        searchFiles(fileList, currentDir, str_dir, filename , parent_lvl++);
        closedir(currentDir);
      }
      if(dIrent->d_type == FILE_TYPE && ( strstr(dIrent->d_name, filename) != NULL)){
        strcpy(str_dir, mainDir);
        strcat(str_dir, "/");
        strcat(str_dir, dIrent->d_name);

        int size = 0;
        while((*fileList)[size] != NULL){
          size++;
        }

        char **newFileList = (char**)realloc(*fileList, sizeof(char*) * ( size + 2));


        *fileList = newFileList;
        (*fileList)[size] = (char*)malloc(strlen(str_dir) + 1 );

        strcpy((*fileList)[size], str_dir);
        (*fileList)[size + 1] = NULL;

        result = 1;
      }
    }
  }
  return result;
}

int testingMysql(void){
  int *a = (int*)malloc(sizeof(int)*2);

  *a = 10;
  a++;
  *a = 12;


  printf("%d\n", a[0]);


  printf("%d\n", a[1]);

  char *str = "teste";
  printf("%s\n", str);


  MYSQL mysql = *mysql_init(&mysql);

  mysql_optionsv(&mysql, MYSQL_READ_DEFAULT_GROUP, "group1");

  if (!mysql_real_connect(&mysql, "127.0.0.1", "pedro", "pedro", "dbteste", 0,
                          NULL, 0)) {
    fprintf(stderr, "Failed to connect to database: Error: %s\n",
            mysql_error(&mysql));
    return EXIT_FAILURE;
  }

  printf("%s\n", mysql_stat(&mysql));

  mysql_query(&mysql, "SHOW DATABASES;");
  MYSQL_RES *result = mysql_store_result(&mysql);

  int numFields;
  if (result) {
    numFields = mysql_num_fields(result);
    MYSQL_ROW row;
    MYSQL_FIELD *fields = mysql_fetch_fields(result);
    while ((row = mysql_fetch_row(result))) {
      unsigned long *lengths;
      lengths = mysql_fetch_lengths(result);
      for (int i = 0; i < numFields; i++) {
        printf("%s", fields[i].name);
        printf("[%.*s] ", (int)lengths[i], *( row + i ) ? row[i] : "NULL");
      }
      printf("\n");
    }
  }

  printf("%d", mysql_field_count(&mysql));

  mysql_close(&mysql);

  return 0;
}

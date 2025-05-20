#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <pthread.h>

#define THREAD_COUNT 4
#define ASCII_VAL_STAR 42
#define ASCII_VAL_NEWLINE 10

pthread_t thread[THREAD_COUNT];
char** save = NULL;

char** openHeaderFile(char* Hpath){
  FILE* head = fopen(Hpath, "r");
  if(head == NULL){
    fprintf(stderr, "\nFailed to open header file %s.\n", Hpath);
    perror("");
    exit(1);
  }
  char ch;
  int len = 0;
  do{
    ch = fgetc(head);
    if((int)ch == ASCII_VAL_STAR) len++;
  } while (ch != EOF);
  char** buff = (char**)malloc(sizeof(char*) * len);
  rewind(head);
  for (int i = 0; i < len; i++){
    buff[i] = (char*)calloc(FILENAME_MAX, sizeof(char));
    int ii = 0;
    do{
      ch = fgetc(head);
        buff[i][ii++] = ch;
    } while (ch != ASCII_VAL_STAR);
  }
  for (int i = 0; i < len; i++){
    for (int ii = 0; ii < FILENAME_MAX; ii++){
      if((int)buff[i][ii] == ASCII_VAL_NEWLINE){
        for (int iii = ii; iii < FILENAME_MAX-ii; iii++){
          buff[i][iii] = buff[i][iii+1]; 
        }
      }
      if ((int)buff[i][ii] == ASCII_VAL_STAR){
        for (int iii = ii; iii < FILENAME_MAX-ii; iii++){
          buff[i][iii] = buff[i][iii+1]; 
        }
      }
    }
  }
  fclose(head);
  return buff;
}
void readfile(const char* fname){
  FILE* file = fopen64(fname, "r");
  printf("%s\n", fname);



  fclose(file);
}
int fexists(const char* fname){
  FILE* file;
  if ((file = fopen(fname, "r"))){
    fclose(file);
    return 1;
  }
  return 0;
}
char* FormateStr(const char* str1, const char* str2){
  int len1 = 0;
  int len2 = 0;
  while(str1[len1])len1++;
  while(str1[len2])len2++;
  char* buff = (char*)malloc(sizeof(char) * (len1+len2+2));
  for (int i = 0; i < len1; i++) buff[i] = str1[i];
  buff[len1++] = 92; // "\" 
  for (int i = 0; i < len2; i++) buff[i+len1] = str2[i];
  buff[len1+len2] = (char)0x0;
  return buff;
}
void locate(const char* name){
  DIR* d = opendir(name);
  struct dirent* entry = readdir(d);
  struct dirent* back = readdir(d);
  while((entry = readdir(d)) != NULL){
    char buff[260];
    sprintf(buff, "%s\\%s", name, entry->d_name);
    if(fexists(buff)){
      readfile(buff);

      continue;
    }
    locate(buff);
  }
}
int main(void){
  printf("\n");

  char home[] = "F:\\";
  locate(home);


  printf("\n");
  return 0;
}

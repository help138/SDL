#include <stdio.h>
#include <stdlib.h>

#define ASCII_VAL_STAR 42
#define ASCII_VAL_NEWLINE 10

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
    do {
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
int main(void){
  printf("\n");


  printf("\n");
  return 0;
}

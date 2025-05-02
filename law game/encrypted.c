#include <stdio.h>

char tn[] = "C:\\coding projects\\SDL\\law game\\outs\\test.txt";


void compressFile(char* name){
  FILE* file = fopen64(name, "r");

  int i = 0;
  FILE* out = NULL;
  do{ 
    char buff[20];
    sprintf(buff, "out#%d.txt", i++);
    out = fopen(buff, "w");
    printf("%s\n", buff);
    if(out == NULL) break;
    fclose(out);
  } while(i < 100);

  fclose(out);
  fclose(file);
}
int main(int argc, char const *argv[]){
  printf("\n");

  if(argc != 1){
    for (int i = 1; i < argc; i++){
      printf("%d: %s\n", i, argv[i]);

    }    
  } 


  printf("\n");
  return 0;
}
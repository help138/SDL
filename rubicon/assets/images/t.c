#include <stdio.h>

int main(){
  for (int i = 1; i < 17; i++){
    printf("\n%-10d| `1` |\n", i);
    for (int ii = 0; ii < 17; ii++) printf("_");
  }
  
  return 0;
}

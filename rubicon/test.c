#include <stdio.h>


void drawSin(float x, float y, float w, float h, int samples){
  float s[samples];
  float ratio = (float)1/samples;
  for (int i = 0; i < samples; i++){
    s[i] = (float)ratio*i;
  }
  
}
int main(){
  printf("\n");

  drawSin(100, 100, 10, 10, 10);
  
  printf("\n");
  return 0;
}

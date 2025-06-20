#define SDL_MAIN_USE_CALLBACKS 1
#include <time.h>
#include <iostream>
#include "inc/SDL.h"
#include "classes.hpp" //TODO: move to inc
#include "inc/SDL_main.h"

#define DEFAULT_WINDOW_WIDTH 640 
#define DEFAULT_WINDOW_HIGHT 480

const double c1 = SDL_PI_D*2/3;
const double c2 = SDL_PI_D*4/3;
int wind_width = DEFAULT_WINDOW_WIDTH;
int wind_hight = DEFAULT_WINDOW_HIGHT; 
SDL_Window* wind = NULL;
SDL_Renderer* render = NULL;
bool* stars = NULL;


void test(){
  size_t count;
  count = SDL_GetTicks();
  double now = (double)(count/1000.0);
  const float r = (float)(0.5 + 0.5 * SDL_sin(now));
  const float b = (float)(0.5 + 0.5 * SDL_sin(now + c1));
  const float g = (float)(0.5 + 0.5 * SDL_sin(now + c2));

  SDL_FRect rect;

  SDL_SetRenderDrawColorFloat(render, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);  
  rect.x = 0;    
  rect.y = 0;    
  rect.w = wind_width/2;
  rect.h = wind_hight/2;
  SDL_RenderFillRect(render, &rect); 

  SDL_SetRenderDrawColorFloat(render, b, r, g, SDL_ALPHA_OPAQUE_FLOAT);  
  rect.x = wind_width/2;    
  rect.y = 0;    
  rect.w = wind_width/2;
  rect.h = wind_hight/2;
  SDL_RenderFillRect(render, &rect);

  SDL_SetRenderDrawColorFloat(render, g, b, r, SDL_ALPHA_OPAQUE_FLOAT);  
  rect.x = 0;
  rect.y = wind_hight/2;
  rect.w = wind_width/2;
  rect.h = wind_hight/2;
  SDL_RenderFillRect(render, &rect);

  SDL_SetRenderDrawColorFloat(render, 255, 255, 255, SDL_ALPHA_OPAQUE_FLOAT);
  rect.x = wind_width/2;
  rect.y = wind_hight/2;
  rect.w = wind_width/2;
  rect.h = wind_hight/2;
  SDL_RenderFillRect(render, &rect);
}
void initStars(int freq){
  SDL_srand(time(NULL) ^ SDL_GetTicksNS());
  int len = wind_hight * wind_width;
  stars = (bool*)calloc(sizeof(bool), len);
  freq = (freq % len);
  if(freq == 0 || freq == len) freq = len/(SDL_rand(10)+2);
  for (int i = 0; i < freq; i++) stars[SDL_rand(len)] = 1;
  
}
void template_drawStars(){
  int len = wind_hight * wind_width;
  SDL_FPoint arr[len];
  int count = 0;
  for (int i = 0; i < len; i++)
    if(stars[i] == 1) arr[count++] = { (float)(i % wind_width), (float)(i % wind_hight) };
  SDL_SetRenderDrawColorFloat(render, 255, 255, 255, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderPoints(render, arr, count);
}
void drawStars(){
  int len = wind_hight * wind_width;
  for (int i = 0; i < len; i++)
    if(stars[i] == 1) {
      SDL_SetRenderDrawColor(render, 255, 255, 255, SDL_ALPHA_OPAQUE);
      SDL_RenderPoint(render, (float)(i % wind_width), (float)(i % wind_hight));
    }
}
void draw(){
  SDL_SetRenderDrawColorFloat(render, 0, 0, 0, SDL_ALPHA_OPAQUE_FLOAT);

  SDL_RenderClear(render);
  
  drawStars();
}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  SDL_CreateWindowAndRenderer("callback demo", wind_width, wind_hight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OCCLUDED /*| SDL_WINDOW_MAXIMIZED | SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_FULLSCREEN*/, &wind, &render);
  SDL_srand(time(NULL)); 
  initStars(100);
  
  if (wind == NULL){
    printf("failed to create window: %s\n", SDL_GetError());
    return SDL_APP_FAILURE; 
  }
  if (render == NULL){
    printf("failed to render: %s\n", SDL_GetError());
    return SDL_APP_FAILURE;  
  }
	
  return SDL_APP_CONTINUE;  
}
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event){
  switch (event->type){
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS; 
    case SDL_EVENT_KEY_DOWN:
      if (event->key.key == SDLK_ESCAPE) return SDL_APP_SUCCESS; 
          break;
    case SDL_EVENT_WINDOW_RESIZED:
      wind_width = event->window.data1;
      wind_hight = event->window.data2;
      printf("hight: %d, width: %d, %d\n", wind_hight, wind_width, (wind_hight/wind_width));
      free(stars);
      initStars((wind_hight/wind_width)*10);
        break;
    default:
        return SDL_APP_CONTINUE;
  }
  return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void *appstate){

  draw();

  SDL_RenderPresent(render); 

  return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result){
  if(stars != NULL) free(stars);
  //SDL will handle most of the cleanup
}
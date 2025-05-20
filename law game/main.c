#define SDL_MAIN_USE_CALLBACKS 1
#include <stdio.h>
#include "include/SDL.h"
#include "include/SDL_main.h"

#define DEFAULT_WINDOW_WIDTH 640 
#define DEFAULT_WINDOW_HIGHT 480

int wind_width = DEFAULT_WINDOW_WIDTH;
int wind_hight = DEFAULT_WINDOW_HIGHT; 
SDL_Window* wind = NULL;
SDL_Renderer* render = NULL;


void draw(){

}
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]){
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
  SDL_CreateWindowAndRenderer("callback demo", wind_width, wind_hight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OCCLUDED | SDL_WINDOW_MAXIMIZED /*| SDL_WINDOW_MOUSE_GRABBED | SDL_WINDOW_FULLSCREEN*/, &wind, &render);
  
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
        break;
    default:
        return SDL_APP_CONTINUE;
  }
  return SDL_APP_CONTINUE;
}
SDL_AppResult SDL_AppIterate(void *appstate){
  size_t count;
  const double c1 = SDL_PI_D*2/3;
  const double c2 = SDL_PI_D*4/3;
  count = SDL_GetTicks();
  double now = (double)(count/1000.0);
  const float r = (float)(0.5 + 0.5 * SDL_sin(now));
  const float b = (float)(0.5 + 0.5 * SDL_sin(now + c1));
  const float g = (float)(0.5 + 0.5 * SDL_sin(now + c2));
  SDL_SetRenderDrawColorFloat(render, r, g, b, SDL_ALPHA_OPAQUE_FLOAT);  
  SDL_RenderClear(render);

  SDL_RenderPresent(render); 

  return SDL_APP_CONTINUE;
}
void SDL_AppQuit(void *appstate, SDL_AppResult result){
  //SDL will handle most of the cleanup
}
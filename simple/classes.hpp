
#ifndef GAME_CLASS_h__
#define GAME_CLASS_h__

class projectile{
  private:

  public:
    float x, y, h, w;
    int speed;
    projectile(int cx, int cy, int sh, int sw);
    ~projectile(){}
    // void drawProjectile(unsigned char r, unsigned char g, unsigned char b){
    //   SDL_SetRenderDrawColor(render, r, g, b, SDL_ALPHA_OPAQUE);  
    //   SDL_FRect rect = (SDL_FRect){ x, y, h, w};
    //   SDL_RenderFillRect(render, &rect); 
    // }
  };

  projectile::projectile(int cx, int cy, int sh, int sw){
    speed = 0;
    x = (float)cx;
    y = (float)cy;
    h = (float)sh;
    w = (float)sw;
  }

  class entity{
  private:

  public:
    float x, y;
    int speed;
    int hp;
    entity(/* args */);
    ~entity(){}
  };
  
  entity::entity(/* args */){

  }

#endif



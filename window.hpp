#include<stdio.h>
#include<stdlib.h> 
#include"inc/SDL.h"  
#include "inc/SDL_image.h"
#include "vector.hpp"
#include  <string>
#include  <iostream>

using  namespace  std; 


 Background  *bg; 
 Button      *btn; 
 Doodle      *doodle; 
 Plateform   *plateform; 
 GameOver    *gamOver;

class  Window   
{
protected :   

    // variable  asset  games    
 
     unsigned  int   HeightWindow   =   500;
     unsigned  int  WidthWindow     =   300; 
     int  flagsPosXWindow           =   SDL_WINDOWPOS_CENTERED;  
     int flagsPosYWindow            =   SDL_WINDOWPOS_CENTERED;   
     int score_game                 =   0;
     const  char  *title            =   "Doodle Jump";  
     const  char  *path_icon        =   "./res/img/doodler-guy.png";  

    //    variable  SDL  game window 

     SDL_Renderer  *rendu           =  nullptr;  
     SDL_Window    *fenetre         =  nullptr; 
     SDL_Color      color_window    =  {255,255,255,255}; 
     SDL_Surface    *icon_surf      =   NULL;
     SDL_Event      evt;  
     SDL_Rect       window_rect;
     
    //    variable  game  loop  window 

    bool  isRunning                     =  true;  
    const  unsigned   int    delay_time =   45;  
    std::string   score_text;
    const Uint8  *keybord_state         =  SDL_GetKeyboardState(NULL);    
    
    //  state  game  
    enum  State   { MENU,PLAYING,  GAMEOVER}; 
    int stategame   = 0; 
    int start       = 0;  
    int ends        = 0;
    int delays      = 0;
    int LIMITS      = 60;

 public :  
    Window();  
   ~Window();  
 protected :     
    bool   isRun();  
    void  HandleEvent();  
    void  RenderClear();  
    void  update();  
    void  RefeshPresent(); 
    void  clean();
 public :  
   friend   void  loopGame(Window  *wind);   
};   


Window::Window()  
{
     if(SDL_Init(SDL_INIT_VIDEO) <   0) 
     {
           SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur  %s" , SDL_GetError());
           IMG_Init(IMG_INIT_JPG  |  IMG_INIT_PNG);
     }else  printf(" la  SDL a  ete bien  initalisee !\n");    

     if(SDL_CreateWindowAndRenderer(this->WidthWindow , this->HeightWindow , 0 ,&this->fenetre,&this->rendu) <  0) 
     {
           SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur  %s" , SDL_GetError()); 
     }else    printf(" Wouah ma jolie  petite  fenetre !\n");    

     SDL_SetWindowTitle(this->fenetre ,this->title); 

     if(this->icon_surf == NULL)  
     {
          this->icon_surf =   IMG_Load("./res/img/doodler-guy.png");
          SDL_SetWindowIcon(this->fenetre , this->icon_surf); 
     }  
     SDL_FreeSurface(icon_surf); 

     bg        =  new  Background(this->rendu);  
     btn       =  new  Button(this->rendu); 
     doodle    =  new   Doodle(this->rendu);  
     plateform =  new Plateform(this->rendu);  
     gamOver   =  new  GameOver(this->rendu);

      doodle->pos_doodler.y =   plateform->plateform_arr[2].y - doodle->pos_doodler.h;

      doodle->vy =   doodle->vsaut;

     doodle->pos_doodler.x =   plateform->plateform_arr[2].x + (plateform->plateform_arr[2].w /  2   - (doodle->pos_doodler.w/2)); 

   

} 


Window::~Window()  
{     
    gamOver->DestroyAll();
    plateform->freeRessourcePlateform(); 
    doodle->cleanRessourceDoodle(); 
    btn->cleanAllRessource();
    bg->freeRessource();
    this->clean(); 
    IMG_Quit(); 
    SDL_Quit();
}    


bool  Window::isRun()  
{
       return  Window::isRunning;
} 


void  Window::HandleEvent()  
{
       SDL_PollEvent(&this->evt); 

       if(this->evt.type ==   SDL_QUIT)  
       {
          this->isRunning  =  false;   
       }else if(this->evt.type ==  SDL_KEYDOWN) 
       {
           if(this->evt.key.keysym.sym ==  SDLK_p)  
           {
               doodle->isJumping =  true;
           }
       }
}  

void  Window::RenderClear()  
{
  SDL_SetRenderDrawColor(this->rendu ,  this->color_window.r , this->color_window.g ,this->color_window.b, this->color_window.a); 
  SDL_RenderClear(this->rendu);    
  if(this->stategame ==  0)  
  {
  bg->drawBackground(this->rendu); 
  btn->DrawButton(this->rendu);
  }else if(this->stategame == 1) 
  {
  bg->drawBackground(this->rendu); 
  doodle->drawDoodle(this->rendu);  
  plateform->drawPlateform(this->rendu);
  }else if(this->stategame ==  2) 
  {
    gamOver->DrawGamover(this->rendu);
  }
} 

void Window::RefeshPresent() 
{
    SDL_RenderPresent(this->rendu);  
    SDL_Delay(this->delay_time);
}   

void  Window::update() 
{   
   
      if(doodle->isJumping) 
      {
          doodle->pos_doodler.y  += doodle->vy;
          doodle->vy+=doodle->gravity;
      }  

      for(auto  &p  :  plateform->plateform_arr)  
      {
           if(SDL_HasIntersection(&doodle->pos_doodler , &p))  
           {
              doodle->pos_doodler.y =   p.y - doodle->pos_doodler.h;
              doodle->vy =  doodle->vsaut;
           }   
           if(doodle->pos_doodler.y  +   doodle->pos_doodler.h <= this->HeightWindow  /2)  
           {
                plateform->isplateformgoingDown =  true;
           }  

           if(plateform->isplateformgoingDown)  
           {
               p.y+=1*2; 

               if(p.y  +  p.h   >=  this->HeightWindow)  
               {
                     p.y =   0  + p.h  +  45;  
                     p.x =  rand()  %  (plateform->espaceborderXmax   -  plateform->espaceborderXMin);
               }
           }
      }   

      this->start =  SDL_GetTicks(); 
      this->delays =  this->start  -  this->ends;
      
      if(doodle->pos_doodler.y  +  doodle->pos_doodler.h   <=   this->HeightWindow)  
       {   
          if(this->delays  >  1000 / this->LIMITS)  
          {
              this->score_game+=1;   
          }
       }else  
       {
           this->stategame  =  GAMEOVER;
       }
       this->score_text ="Doodle Jump s : " + std::to_string(score_game);
       SDL_SetWindowTitle(this->fenetre,this->score_text.c_str());
    

      btn->HandleBUttonClicked(evt,stategame);
      doodle->MoveDoodle(this->keybord_state);
}

void Window::clean() 
{    
     delete  gamOver; 
     delete  plateform; 
     delete  doodle;
     delete  btn;
     delete  bg;
    if(this->rendu    !=  nullptr)  SDL_DestroyRenderer(this->rendu); 
    if(this->fenetre  !=  nullptr)  SDL_DestroyWindow(this->fenetre); 
}
 


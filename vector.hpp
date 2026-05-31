#include<stdio.h>
#include<stdlib.h>  
#include<vector>
#include"inc/SDL.h"  
#include "inc/SDL_image.h"
#include<time.h> 
#include "textureManagement.hpp"

using  namespace  std;

class  Vector2 
{
    public :  
       float posX; 
       float posy;   
       float speed = 0.3;    
    public :  
    Vector2(float px ,  float py) : posX(px) , posy(py) {}  

    Vector2 operator  +(Vector2  other) 
    {
        Vector2(posX +  other.posX ,   posy + other.posy);
    }

    Vector2& operator +=(Vector2  other)  
    {        
         posy += other.posy *  speed; 
         posX += other.posX;
          return *this;
    } 
       Vector2& operator -=(Vector2  other)  
    {        
         posy -= other.posy; 
         posX -= other.posX;
          return *this;
    }   
};
  

class Background  
{ 
    public :  
      Background(SDL_Renderer *rendu); 
      ~Background();  
   public :  
    SDL_Surface *background_surf = NULL;  
    SDL_Texture *background_text = NULL;   
    SDL_Rect  background_pos_rect =  {0,0,300,500}; 
    const char *path_bg =  "./res/img/background.png"; 
    public :   
    void  drawBackground(SDL_Renderer *rendu);  
    void  freeRessource();
};   


Background::Background(SDL_Renderer *rendu) 
{
      Vector2  background_pos(0,0);

      this->background_pos_rect.x =  background_pos.posX;  
      this->background_pos_rect.y =  background_pos.posy;
      this->background_text  =   Texture_Handle(this->background_surf ,  this->background_text).Handle_texture(this->path_bg , rendu);  

      if(this->background_text  ==  NULL) 
      {
          SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur %s",SDL_GetError());
      }else  printf("j'ai  un jolie  background  qui  s'affiche \n");
}   
Background::~Background() 
{
      this->freeRessource();
}  

void Background::drawBackground(SDL_Renderer *rendu) 
{
  SDL_SetRenderTarget(rendu,this->background_text);
  SDL_RenderCopy(rendu , this->background_text ,NULL , &this->background_pos_rect);
} 

void  Background::freeRessource()  
{
       SDL_DestroyTexture(this->background_text);
}  

class  Button   
{
        public  :      
          Button(SDL_Renderer*  rendu);
         ~Button();  
        public:  
         SDL_Surface  *btn_surf =  NULL;  
         SDL_Texture  *btn_text =  NULL;  
         SDL_Rect    btn_rect =  {0,0,123,89};  
         const char *path_btn =  "./res/img/buttonPlay.png";  
         SDL_Rect      mouse_clicked =  {0,0,73,39}; 
         public :  
            void  DrawButton(SDL_Renderer  *rendu);   
            void  HandleBUttonClicked(SDL_Event  evt ,  int &stateg); 
            void  cleanAllRessource();         
};  


Button::Button(SDL_Renderer  *rendu)  
{    
     Vector2  btn_pos_init(0,0)  ,   btn_pos_final(300,500);    
     this->btn_text  =   Texture_Handle(this->btn_surf ,  this->btn_text).Handle_texture(this->path_btn , rendu);  

     if(this->btn_text ==  NULL)  
     {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur  %s",SDL_GetError());  
     }else  printf("wouah   j'ai  un jolie  petit button  sur ma  fenetre  qui  s'affiche !!\n");   

     this->btn_rect.x =  (btn_pos_init.posX  +  (btn_pos_final.posX  /  2))  - (this->btn_rect.w  /2);
     this->btn_rect.y  = (btn_pos_init.posy  +  (btn_pos_final.posy  /2))  - (this->btn_rect.h/2);

} 
Button::~Button() 
{
     this->cleanAllRessource();
}    
void Button::DrawButton(SDL_Renderer *rendu) 
{
       SDL_SetRenderTarget(rendu,this->btn_text);
       SDL_RenderCopy(rendu,this->btn_text , NULL,&this->btn_rect);
}  
void Button::HandleBUttonClicked(SDL_Event  evt  ,  int  &stateg)  
{
      if(evt.type ==  SDL_MOUSEBUTTONDOWN)  
      {
            this->mouse_clicked.x =   evt.button.x;  
            this->mouse_clicked.y =   evt.button.y;
          
            if(SDL_HasIntersection(&this->mouse_clicked , &this->btn_rect))   
            {
                   stateg = 1;
            }
      }  
}  
void  Button::cleanAllRessource() 
{
   SDL_DestroyTexture(this->btn_text);
}  

class  Doodle  
{
      public  :  
        Doodle(SDL_Renderer  *rendu); 
       ~Doodle(); 
       public:   
         SDL_Rect pos_doodler     = {0,23,29,29}; 
         SDL_Surface *surf_doodle =  NULL;  
         SDL_Texture *text_doodle =  NULL;   
         const char  *str_path    =  "./res/img/doodler-guy.png"; 
         bool  isJumping          =  false; 
         float  vy                =   0;
         float vsaut              =   -7.6;  
         float  gravity           =    0.9;
      public :  
       void  drawDoodle(SDL_Renderer  *rendu);  
       void  MoveDoodle(const Uint8  *keyBoard);  
       void cleanRessourceDoodle();
}; 

Doodle::Doodle(SDL_Renderer  *rendu)  
{
    this->text_doodle =  Texture_Handle(this->surf_doodle ,  this->text_doodle).Handle_texture(this->str_path,rendu);  

    if(this->text_doodle ==  NULL) 
    {
          SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur %s",SDL_GetError());
    }else   printf("mon  doodle   s'affiche  il  est  joli  lui   ! \n"); 

    vy =  vsaut;
} 
Doodle::~Doodle() 
{
  this->cleanRessourceDoodle();
}    
void Doodle::drawDoodle(SDL_Renderer  *rendu) 
{  
     SDL_SetRenderTarget(rendu,this->text_doodle);  
     SDL_RenderCopy(rendu,this->text_doodle , NULL,&this->pos_doodler);
} 
void Doodle::MoveDoodle(const  Uint8 *keyboard) 
{
   if(keyboard[SDL_SCANCODE_LEFT])   
   {
       this->pos_doodler.x -= 1 *2;      
   }else if(keyboard[SDL_SCANCODE_RIGHT])  
   {
      this->pos_doodler.x += 1 * 2;  
   }
} 
void  Doodle::cleanRessourceDoodle() 
{
    SDL_DestroyTexture(this->text_doodle);
}


class  Plateform    
{
     public :   
       Plateform(SDL_Renderer *rendu);  
      ~Plateform();
     public  :       
     std::vector<SDL_Rect>plateform_arr;
     SDL_Rect   pos_plateform         = {0,1,67,18};  
     SDL_Surface  *plateform_surf     =  NULL;  
     SDL_Texture   *plateform_text    =  NULL;  
     const  char  *path_plateform     =  "./res/img/platform.png";
     unsigned   int  espaceborderXmax =  300 -  25  - pos_plateform.w; 
     unsigned   int  espaceborderXMin =  2; 
     unsigned   int  ecartYmin        =  7;  
     unsigned   int  ecartYmax        =  30;   
     bool       isplateformgoingDown  =  false;   

     public :   
      void  drawPlateform(SDL_Renderer  *rendu);
      void  freeRessourcePlateform(); 
     
};  

Plateform::Plateform(SDL_Renderer * rendu)  
{    
      srand((unsigned)time(NULL)); 

     this->espaceborderXmax  =   300  -   this->pos_plateform.w  - 2;  

     for(size_t  i  =  1;  i <=  14;  i++)  
     {
        this->pos_plateform.y  +=  rand()  %  (this->ecartYmax   -  this->ecartYmin)  +  this->pos_plateform.h;
        this->pos_plateform.x =  rand()  %  (this->espaceborderXmax   - this->espaceborderXMin);   
        this->plateform_arr.push_back(this->pos_plateform);   
     }  


     this->plateform_text =   Texture_Handle(this->plateform_surf ,  this->plateform_text).Handle_texture(this->path_plateform , rendu);

     if(this->plateform_text ==  NULL)  
     {
         SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur %s",SDL_GetError());
     }else  printf("Voila  la  condition a  ete  remplie  les  plateformes pourront s'afficher \n");
} 
Plateform::~Plateform() 
{
   this->freeRessourcePlateform();
} 
void Plateform::drawPlateform(SDL_Renderer  *rendu)
{
    SDL_SetRenderTarget(rendu,this->plateform_text);  
    std::for_each(this->plateform_arr.begin() ,  this->plateform_arr.end() , [&](auto &p) 
    {
        SDL_RenderCopy(rendu,this->plateform_text ,NULL,&p);
    });
} 
void Plateform::freeRessourcePlateform() 
{
   SDL_DestroyTexture(this->plateform_text);
}   


class GameOver  
{
   public :   
     SDL_Rect  gameover_rect    ={0,0,124,67};
     SDL_Surface *gameover_surf =NULL;  
     SDL_Texture *gameover_text =  NULL;  
   public :  
     GameOver(SDL_Renderer *rendu);  
     ~GameOver();
   public:  
     void  DrawGamover(SDL_Renderer  *rendu);  
     void  DestroyAll();  
};  

GameOver::GameOver(SDL_Renderer  *rendu) 
{
     this->gameover_text =  Texture_Handle(this->gameover_surf , this->gameover_text).Handle_texture("./res/img/game.png",rendu); 

     if(this->gameover_text ==  NULL) 
     {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION , "erreur %s",SDL_GetError());
     }    

     this->gameover_rect.x = (300/2) - (this->gameover_rect.w /2);  
     this->gameover_rect.y = (500/2)  -(this->gameover_rect.h/2);
} 
GameOver::~GameOver() 
{
   this->DestroyAll();
}  
void GameOver::DrawGamover(SDL_Renderer  *rendu) 
{
    SDL_SetRenderTarget(rendu,this->gameover_text); 
   SDL_RenderCopy(rendu,this->gameover_text,NULL,&this->gameover_rect);
}  
void GameOver::DestroyAll() 
{
  SDL_DestroyTexture(this->gameover_text);
}

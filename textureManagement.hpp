#include<stdio.h>
#include<stdlib.h> 
#include"inc/SDL.h"  
#include "inc/SDL_image.h"



class  Texture_Handle 
{
      public :  
          SDL_Surface  *surf_handle =  NULL;  
          SDL_Texture  *text_handle =  NULL;  
      public :  
         Texture_Handle(SDL_Surface * surf_h , SDL_Texture  * text_h)  :  surf_handle(surf_h) , text_handle(text_h)  {} 
      public :  
         SDL_Texture * Handle_texture(const char *str_path ,  SDL_Renderer *r)  
         {
               if(surf_handle  ==  NULL)  { surf_handle = IMG_Load(str_path);}  
               if(text_handle  ==  NULL)  {text_handle =  SDL_CreateTextureFromSurface(r,surf_handle);  SDL_FreeSurface(surf_handle);}  

               return  text_handle;
          }
 };
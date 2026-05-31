#include<stdio.h>
#include<stdlib.h>  
#include<iostream>
#include"inc/SDL.h"  
#include "inc/SDL_image.h"
#include<vector>
#include<string>
#include"inc/SDL_ttf.h"
#include<algorithm>
#include<deque> 
#include<time.h> 
#include<fstream>
#include<strstream>  
#include "./window.hpp"



using  namespace  std; 


 Window *gameWind =   new  Window();  
 void loopGame(Window  *wind) 
 { 
     do 
     { 
       wind->HandleEvent();          
       wind->update();
       wind->RenderClear(); 
       wind->RefeshPresent();
     }while(wind->isRun() != false); 

     wind->clean();
 }

int main(int argc  ,  char  *argv[]) 
{    
     
      loopGame(gameWind); 
   
    
      return 1;
}
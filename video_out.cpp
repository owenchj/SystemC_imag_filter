
#include <cstdio>
#include <sstream>
#include <iomanip>
#include <string>  
#include "video_out.h"


/***************************************************************************
 *      gen_sorties()
 ***************************************************************************/

void VIDEO_OUT::gen_in()
{
if(reset_n == false)
  {
// Reset : on remet tous les paramètres à  zéro
current_image_number = 0;
counter = 0;
cout << "module: " << name() << "... reset!" << endl;
}
// boucle infinie
while(1){
  
unsigned int flag  = width * height ; 
     

if(counter < flag){    
if(href){
image.pixel[counter++] = pixel_in; 
}

wait();   
}

 else{
// On a fini une image, on passe à la suivante
current_image_number ++;
display_image();
//   wait();
counter = 0;
} 
}
}



/***********************************
 *  Autres méthodes
 ***********************************/

void VIDEO_OUT::display_image()
{
std::ostringstream name_o;
name_o << base_name << std::setfill('0') << std::setw(2) << current_image_number << ".png";
std::cout << name() << " Write image " << name_o.str() << endl;
image_write(&image, name_o.str().c_str());
}



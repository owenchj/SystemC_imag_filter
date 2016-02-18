
#include <cstdio>
#include <sstream>
#include <iomanip>
#include "zoom.h"
#include <string>  

/***************************************************************************
 *      gen_in()
 ***************************************************************************/

void ZOOM::gen_in()
{
  if(reset_n == false)
    {
      current_image_number = 0;
      counter = 0;
      z_c = 0;
      read_ready = 0;
      cout << "module: " << "ZOOM::gen_in" << "... reset!" << endl;
    }
  // boucle infinie
  
  while(reset_n == true)    wait();   
  
  while(1){
    unsigned int total  = width * height; 
    int x, y;
  
    wait();
    
    z_c0 = z_c;
    counter0 = counter;
    read_ready = 0;    
    if(counter < total){
      
      if(href_in){
	x = counter / 720;
	y = counter - x * 720;
	  if ( z_c < (width * height / 4 ) && (width / 4) <= y  &&  y < (width / 4 *3) && x >= (height / 4) && x < (height / 4 * 3)){
	  x = z_c / (width / 2); 
	  y = z_c - x * width / 2;
	  z_c++;
	  
	  buffer[x][y] = pixel_in;
	 
	  }
	  
	  if(z_c == (width * height  / 4 -1) )     read_ready = 1;
	  counter++;
      }  
    }
      else{
	// On a fini une image, on passe à la suivante
	current_image_number ++;
	counter = 0;
	z_c = 0;
      } 
    }
  }
  


  void ZOOM::gen_out(){
    if(reset_n == false)
      {
	current_image_number = 0;
	counter_out = 0;
	href_out = 0;
	vref_out = 0;
	cout << "module: " <<"ZOOM::gen_out" << "... reset!" << endl;
      }

    while(1){
      int x,y;
      int i,j;
     
      while(read_ready == false) wait();   

 for(i=0; i<625; i++)
	for(j=0; j<874; j++)
	  {
	    counter_out0 =counter_out;     
	    // on attend le prochain coup d'horloge
	    wait();
	    // Si on est dans la fenêtre active, on sort le pixel courant
	    // Rappel : une trame vidéo fait 874*625, l'image active est de 720*576
	    
	    if((i<576) && (j<720)){
	      x = counter_out / 720;
	      y = counter_out - (counter_out / 720) * 720;
	      
	      counter_out++;
	      
	      pixel_out= buffer[ x / 2][ y / 2];
	    }
	    else
	      pixel_out = 0;
	    
	    // Génération de HREF
	    // HREF est actif pendant la sortie des pixels actifs
	    href_out = (i<576) && (j<720);
	    
	    // Génération de VREF
	    // VREF est actif pendant les 3 premières lignes d'une image
	    vref_out = (i<3);
	  }
 counter_out =0 ;
    }
  }


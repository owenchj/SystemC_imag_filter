
#include <cstdio>
#include <sstream>
#include <iomanip>
#include "filter.h"
#include <string>  

/***************************************************************************
 *      gen_in()
 ***************************************************************************/

void FILTER::gen_in()
{
  if(reset_n == false)
    {
      current_image_number = 0;
      counter = 0;
      read_ready = 0;
      cout << "module: " << "FILTER::gen_in" << "... reset!" << endl;
    }
  // boucle infinie
  
   while(reset_n == true)    wait();   
  
  while(1){
    
    unsigned int total  = 720 * 576 ; 
    int x, y;
 
    counter0=counter;
    wait();
    read_ready = 0;    
    if(counter < total){
      
      if(href_in){
	x= (counter / 720) % 3;
	y= counter - (counter / 720) * 720;
	//	cout<<x<<" "<<counter<<"|  ";

	buffer[x][y] = pixel_in;
	if(x==0)buffer0=pixel_in;
	if(x==1)buffer1=pixel_in;
	if(x==2)buffer2=pixel_in;
 	
	if(counter == 720 * 2-1)	  read_ready = 1;
	counter++;

      }
    }  else{
      // On a fini une image, on passe à la suivante
      current_image_number ++;
      counter = 0;
    } 
  }
}



void FILTER::gen_out()
{
  if(reset_n == false)
    {
      current_image_number = 0;
      counter_out = 0;
      href_out = 0;
      vref_out = 0;
      cout << "module: " <<"FILTER::gen_out" << "... reset!" << endl;
    }

  while(1){
  
    int x,y;
    int i,j;
    int sum = 0;
    while(read_ready == false) wait();   
    
  
    for(i=0; i<625; i++)
      for(j=0; j<874; j++)
	{
	  sum = 0;
	  // on attend le prochain coup d'horloge
	  wait();
	  // Si on est dans la fenêtre active, on sort le pixel courant
	  // Rappel : une trame vidéo fait 874*625, l'image active est de 720*576
	  
	  if((i<576) && (j<720)){
	    x = (counter_out / 720) % 3;
	    y = counter_out - (counter_out / 720) * 720;
	    //  cout<<counter_out<<"|  ";
	    counter_out0=counter_out;
	    counter_out++;
	

	    for(int m=x-1; m< x+2; m++)
	      for(int n=y-1; n< y+2; n++){
            	sum += buffer[(m+3) % 3][(n+720) % 720];
	      }
	    pixel_out= sum / 9  ;
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
    counter_out = 0;
  }    
}


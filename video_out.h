#ifndef VIDEO_OUT_H
#define VIDEO_OUT_H

#include "systemc.h"
#include "image.h"


SC_MODULE(VIDEO_OUT) {

   // IO PORTS

   sc_in<bool>         clk;
   sc_in<bool>         reset_n;
   sc_in<bool>        href;
   sc_in<bool>        vref;

   sc_in<unsigned char> pixel_in;
    
 

   /***************************************************
    *  constructor
    **************************************************/

   SC_CTOR(VIDEO_OUT):base_name("receive_wallace")
   {

      cout << "Instanciation de " << name() <<" ..." <<  endl;

      SC_THREAD (gen_in);
      sensitive << clk.pos();
      async_reset_signal_is(reset_n,false);
      dont_initialize();
      
      current_image_number = 0;
      image.width  = width;
      image.height = height;
      image.pixel =new unsigned char[height * width];
      cout << "... réussie" << endl;
	
   }

   /***************************************************
    *  methods and structural parameters
    **************************************************/
   private:
   
   void gen_in();
   void display_image();
   
   const std::string   base_name;              // nom de base des images d'entree
   int                    current_image_number;   // numero de l'image courante
   Image               image;
   unsigned int     counter;
   const static int width = 720, height = 576;   
};

#endif

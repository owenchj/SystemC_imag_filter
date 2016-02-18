#ifndef  ZOOM_H
#define ZOOM_H

#include <systemc.h>
#include "image.h"


/***************************************
 *  structure definition
 **************************************/

SC_MODULE(ZOOM) {

   // IO PORTS

   sc_in<bool>         clk;
   sc_in<bool>         reset_n;

   sc_out<bool>        href_out;
   sc_out<bool>        vref_out;
   sc_in<bool>        href_in;
   sc_in<bool>        vref_in;
   
   sc_in<unsigned char> pixel_in;
   sc_out<unsigned char> pixel_out;
   
  
   sc_out<bool> read_ready;
   sc_out<unsigned int> counter0;
   sc_out<unsigned int> counter_out0;
   sc_out<unsigned int> z_c0;
   
   

   /***************************************************
    *  constructor
    **************************************************/

 SC_CTOR(ZOOM):base_name("zoom")
   {

      cout << "Instanciation de " << name() <<" ..." <<  endl;

      SC_THREAD (gen_in);
      sensitive << clk.pos();

      SC_THREAD (gen_out); 
      sensitive << clk.pos();
      
      async_reset_signal_is(reset_n,false);
      dont_initialize();
      
      counter = 0;
      counter_out = 0;
      z_c = 0;	  
      
      cout << "... réussie" << endl;
   }

   /***************************************************
    *  methods and structural parameters
    **************************************************/
   private:

   void gen_in();
   void gen_out();
   void change();

   const std::string   base_name;              // nom de base des images d'entree
   int                 current_image_number;   // numero de l'image courante

  
   //bool read_ready;
   const static int width = 720, height = 576; 
   unsigned char   buffer[height/ 2][width / 2];  
   unsigned int counter; 
   unsigned int counter_out;
   unsigned int z_c; 
};

#endif


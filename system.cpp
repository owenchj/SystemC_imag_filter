/**********************************************************************
 * File : system.cpp
 * Date : 12/03/2008
 * Author :  Alexis Polti
 * 
 * This program is released under the GNU public license
 * Télécom ParisTECH
 *
 * Testbench pour video_in
 **********************************************************************/

#include "systemc.h"
#include "video_in.h"
#include "video_out.h"
#include "filter.h"
#include "zoom.h"
#include <cstdio>
#include <cstdlib>


/***************************************************
 *	MAIN
 **************************************************/

int sc_main (int argc, char *argv[])
{

    int	ncycles;

    /* La periode de l'horloge du signal video */
    sc_time pix_period(74, SC_NS);


    if (argc == 2) {
        ncycles = atoi(argv[1]);
    } else{
        printf("\n");
        printf("The number of simulation cycles must be defined in the command line (-1 for unlimited)\n\n");
        exit(1);
    }



    /******************************************************
     *      Declaration des signaux
     *****************************************************/

    sc_clock                        signal_clk("Clock", pix_period);
    sc_signal<bool>                 signal_resetn;

    sc_signal<bool>                 signal_vref, signal_href;
    sc_signal<bool>                  vref_out, href_out;
    sc_signal<unsigned char>        signal_pixel;

    sc_signal<unsigned char>        pixel_out;
   
  
    sc_signal<bool>        read_ready;
    sc_signal<unsigned int>        counter;
    sc_signal<unsigned int>        counter_out;
    sc_signal<unsigned int>        z_c;
    
    
    
    
    /********************************************************
     *	Instanciation des composants
     *******************************************************/

    VIDEO_IN video_in("VIDEO_GEN");
    VIDEO_OUT video_out("VIDEO_SORT");
    //    FILTER filter("FILTER");
    ZOOM zoom("ZOOM");
    /*********************************************************
     *	Connexion des composants
     ********************************************************/

    video_in.clk        (signal_clk);
    video_in.reset_n    (signal_resetn);
    video_in.href       (signal_href);
    video_in.vref       (signal_vref);
    video_in.pixel_out  (signal_pixel);

 //    filter.clk        (signal_clk);
 //    filter.reset_n    (signal_resetn);
 //    filter.href_in       (signal_href);
 //    filter.vref_in       (signal_vref);
 //    filter.href_out       (href_out);
 //    filter.vref_out       (vref_out);
 //    filter.pixel_in  (signal_pixel);
 //    filter.pixel_out  (pixel_out);
 //    filter.buffer0(buffer0);
 //    filter.buffer1(buffer1);
 //    filter.buffer2(buffer2);
 //    filter.read_ready(read_ready);
 //    filter.counter0(counter);
 //    filter.counter_out0(counter_out);
    zoom.clk        (signal_clk);
    zoom.reset_n    (signal_resetn);
    zoom.href_in       (signal_href);
    zoom.vref_in       (signal_vref);
    zoom.href_out       (href_out);
    zoom.vref_out       (vref_out);
    zoom.pixel_in  (signal_pixel);
    zoom.pixel_out  (pixel_out);
   
    zoom.read_ready(read_ready);
    zoom.counter0(counter);
    zoom.counter_out0(counter_out);
    zoom.z_c0(z_c);

    video_out.clk        (signal_clk);
    video_out.reset_n    (signal_resetn);
    video_out.href       (href_out);
    video_out.vref       (vref_out);
    video_out.pixel_in  (pixel_out);
    // video_out.count  (count);
    /*********************************************************
     *	Traces
     ********************************************************/

    /* open trace file */
    sc_trace_file * my_trace_file;
    my_trace_file = sc_create_vcd_trace_file ("simulation_trace");
    my_trace_file->set_time_unit(1,SC_NS);

    /* chronogrammes signaux CLK et NRESET */
    sc_trace(my_trace_file, signal_clk,         "clk");
    sc_trace(my_trace_file, signal_resetn,      "reset_n");

    /* chronogrammes video */
    sc_trace(my_trace_file, signal_href,        "href");
    sc_trace(my_trace_file, signal_vref,        "vref");
    sc_trace(my_trace_file, href_out,        "href_out");
    sc_trace(my_trace_file, vref_out,        "vref_out");
    sc_trace(my_trace_file, signal_pixel,       "pixel");
    sc_trace(my_trace_file, pixel_out,       "pixel_out");
       sc_trace(my_trace_file, read_ready,       "ready");
    sc_trace(my_trace_file, counter,       "counter");
    sc_trace(my_trace_file, counter_out,       "counter_out");
    sc_trace(my_trace_file, z_c,       "z_c");

    /*********************************************************
     *	Simulation
     ********************************************************/

    /* Initialisation de la simulation */
    signal_resetn = true;
    sc_start(signal_clk.period());
    sc_start(signal_clk.period());

    /* Generation d'un reset */
    signal_resetn = false;
    sc_start(signal_clk.period());
    sc_start(signal_clk.period());
    sc_start(signal_clk.period());
    signal_resetn = true;

    /* Lancement de la simulation */
    int i=0;
    while(1) {

        /* Si on a atteint le nombre de cycles demandés, on s'arrête */
        if(ncycles > 0)
            if(i >= ncycles)
                break;
        sc_start(signal_clk.period());
        i++;

    } // end simulation loop

    cout << "End of simulation @ " << sc_time_stamp() << endl;

    /* Close trace file */
    sc_close_vcd_trace_file (my_trace_file);


    return EXIT_SUCCESS;

}

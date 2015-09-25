/*

 * beolvassa a parameterkent kapott DIMX * DIMX mereru bitmap file-t, amelyen 
 * elkiemelest vegez, es kiirja a pic.out file-ba az eredmenyt.
 * DIMX, DIMY, valamint az eredmeny file neve be van drotozva a programba!
 *
 * TODO: pgm formatum ertelmezses/eloallitasa
 * 	pgm header: "P5\nW\nH\n255\n"
 *      ahol W - szelesseg
 * 	     H - magassag
 * 
 */

#include <iostream>
#include "image.h"

char M[]= {	0, 1, 0,  1,-4, 1,  0, 1 ,0  };



int main(int argc, char *argv[]) {
	Image img(argv[1]);
  img.convolution(M);
  img.save("proba.pgm");
}


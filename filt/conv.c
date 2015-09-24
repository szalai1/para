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

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define OUTP	"pic.out"
#define DIMX	128			// Felbontas X iranyban
#define DIMY	128			// felbontas Y iranyban

char M[3][3]= {				// konvolucios matrix
	{ 0, 1, 0, },
	{ 1,-4, 1, },
	{ 0, 1 ,0  } };


char inp[DIMX][DIMY],oup[DIMX][DIMY];	// be es kimeneti tombok

struct Image {
  int dimx, dimy;
  char *img;
}

Image make_image(char *file_name) {
  int f = open(file_name, "rb");
  char first_line[4];
  char W[10];
  char H[10];
  readline(, null, );

}

int main(int argc, char *argv[]) {
	int inf,of;
	int i,j,k,l;

	if (argc < 2) {
	  fprintf (stderr,"Usage: %s input_file\n", argv[0]);
	  return 1;
	}

	inf=open(argv[1], O_RDONLY);
	if (inf < 0) {
	  perror(argv[1]);
	  return 1;
	}
	of=open(OUTP, O_WRONLY|O_CREAT, 0666);
	if (of < 0) {
	  perror(OUTP);
	  return 1;
	}
	
	if (read(inf, inp, DIMX*DIMY) < DIMX*DIMY) {
	  fprintf(stderr, "Input error\n");
	  return 1;
	}
	for (i=1; i<DIMX-1; i++) {		// itt vegezzuk el a konvoluciot
	  for (j=1; j<DIMY-1; j++) {		// a szeleket beken hagyjuk
  	    oup[i][j]=0;
	    for (k=0; k<3; k++)
	      for (l=0; l<3; l++)
		 oup[i][j] += M[k][l]*inp[i-k+1][j-l+1];
  	  }
  	}
	if (write(of, oup, DIMX*DIMY) < DIMX*DIMY) {	// kiirjuk az eredmenyt
	  perror(OUTP);
	  return 1;
	}
	close(inf);
	close(of);
	return 0;
}


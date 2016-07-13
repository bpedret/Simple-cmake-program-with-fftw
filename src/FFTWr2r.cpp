# include <stdlib.h>
# include <stdio.h>
# include <time.h>
#ifdef CUFFTW
	#include "cufftw.h"
	#include "thrust/detail/mpl/math.h"
//	#include "math_functions.hpp"
#else
	#include "fftw3.h"
	#include "math.h"
#endif 

int main ( )
{
  int i;
  double *in;
  double *in2;
  int n = 8388608;
  int nc;
  fftw_complex *out;
  fftw_plan plan_backward;
  fftw_plan plan_forward;
  unsigned int seed = 123456789;

	for (int j= 0; j < 32; j++)
{
//  Set up an array to hold the data, and assign the data.

  in = ( double* ) fftw_malloc ( sizeof ( double ) * n );
  srand ( seed );
  for ( i = 0; i < n; i++ ) in[i] = 3*sin(i*0.1);

  //printf ( "  Input Data:\n" );

 // for ( i = 0; i < n; i++ ) printf ("  %4d  %12f\n",i, in[i] );

/*
  Set up an array to hold the transformed data,
  get a "plan", and execute the plan to transform the IN data to
  the OUT FFT coefficients.
*/
  nc = ( n / 2 ) + 1;

  out = ( fftw_complex* ) fftw_malloc ( sizeof ( fftw_complex ) * nc );

  plan_forward = fftw_plan_dft_r2c_1d ( n, in, out, FFTW_ESTIMATE );

  fftw_execute ( plan_forward );

  //printf ( "  Output FFT Coefficients:\n" );


 // for ( i = 0; i < nc; i++ ) printf ( "  %4d  %12f  %12f\n", i, out[i][0], out[i][1] );

/*
  Set up an arrray to hold the backtransformed data IN2,
  get a "plan", and execute the plan to backtransform the OUT
  FFT coefficients to IN2.
*/
  in2 = ( double* ) fftw_malloc ( sizeof ( double ) * n );

  plan_backward = fftw_plan_dft_c2r_1d ( n, out, in2, FFTW_ESTIMATE );

  fftw_execute ( plan_backward );

 // printf ( "  Recovered input data divided by N:\n" );

 // for ( i = 0; i < n; i++ ) printf ( "  %4d  %12f\n", i, in2[i] / ( double ) ( n ) );

  fftw_destroy_plan ( plan_forward );
  fftw_destroy_plan ( plan_backward );

  fftw_free ( in );
  fftw_free ( in2 );
  fftw_free ( out );
}
  return 0;
}

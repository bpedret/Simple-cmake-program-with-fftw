//Example program with fftw given by NVIDIA in its CUDA cast tutorial number #8
//link --> https://www.youtube.com/watch?v=My8YJ3v8XFE

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cufftw.h>

#define C_2POW32_INV_DOUBLE (2.383064365386963E-10)

#define CUDA_ZNEW (cuda_z=36969*(cuda_z&65535)+(cuda_z>>16))
#define CUDA_WNEW (cuda_w=18000*(cuda_w&65535)+(cuda_w>>16))
#define CUDA_MWC ((CUDA_ZNEW<<16)+CUDA_WNEW)
#define CUDA_SHR3 ((cuda_jsr=cuda_jsr^(cuda_jsr<<13), \
		    cuda_jsr=cuda_jsr^(cuda_jsr>>17), \
		    cuda_jsr=cuda_jsr^(cuda_jsr<<5))
#define CUDA_CONG (cuda_jcong=69069*cuda_jcong+13579)
#define KISS ((CUDA_MWC^CUDA_CONG)+CUDA_SHR3)

static unsigned int cuda_z=362436069, cuda_w=521288629;
static unsigned int cuda_jsr=362436069, cuda_jcong=123456789;

#define NX 256	    
#define NY 256
#define NZ 256

int main (int argc, char *argv[])
{
	timeval tStart, tEnd;
	float elapsedTimeMsec = 0.0;
	fftwf_plan wplan = NULL;

	fftwf_complex * input = NULL;
	fftwf_complex * output = NULL;

	int i;
	input = (fftwf_complex *)calloc(NX*NY*NZ, sizeof (fftwf_complex));
	output = (fftwf_complex *)calloc(NX*NY*NZ, sizeof (fftwf_complex));

	for (i=0; i<NX*NY*NZ; i++) {
		input[i][0] = ((KISS * C_2POW32_INV_DOUBLE) + (C_2POW32_INV_DOUBLE/2.0) - 0.5));
		input[i][1] = ((KISS * C_2POW32_INV_DOUBLE) + (C_2POW32_INV_DOUBLE/2.0) - 0.5));
	}
	fftwf_set_timelimit (300.0);
	
	for (int j=0; j<50; j++) {
	wplan = fftwf_plan_dft_3d (NX, NY, NZ, input, output, FFTW_FORWARD, FFTW_ESTIMATE);

	if (wplan==NULL) {
		printf("FFTW plan failed \n");
		return 0*01;
	}

	gettimeofday (&tStart, NULL);


	fftwf_execute (wplan);

	gettimeofday (&tEnd, NULL);

	elapsedTimeMsec = ((tEnd.tv_usec + 1000000*tEnd.tv_sec) - (tStart.tv_usec + 1000000*tStart.tv_sec));
	
	fftwf_destroy_plan (wplan);
}
	return 0;
}

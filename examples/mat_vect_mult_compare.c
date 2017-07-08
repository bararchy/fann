#include <stdio.h>
#include <stdlib.h>
#include <cblas.h>
#include <sys/time.h>
#include <string.h>
#include <xmmintrin.h>

#define NUM_IT 10000
#define NUM_R 100
#define DIM 100

struct timeval tv_start;
struct timeval tv_now;

#define ADDER(m0,m1)\
	xmm ## m0=_mm_load_ps(A_row); A_row+=4; \
	xmm ## m1=_mm_load_ps(Bp); Bp+=4; \
	xmm ## m1 = _mm_mul_ps(xmm ## m1, xmm ## m0);\
	xmm7 = _mm_add_ps(xmm7, xmm ## m1);

static __inline float sum_v4sp(__m128 v4sp)
{
	float *v = (float* )&v4sp;
	return v[0]+v[1]+v[2]+v[3];
}


static __inline void MatVectMult(float *A, float *B, float *S, int R, int C, int Cp)
{
	/* SSE RowMajor */
	__m128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
	float *Bp, *A_row;
	int  x, y;
	A_row=A;

	for (y=0; y< R; y++)
	{
		Bp=B;
		xmm7=_mm_set_ps1(0);
		for (x=0; x<=C-28;x+=28)
		{
			ADDER(0,1);
			ADDER(2,3);
			ADDER(4,5);
			ADDER(6,0);
			ADDER(1,2);
			ADDER(3,4);
			ADDER(5,6);
		}

		for (; x<=C-4;x+=4)
		{
			ADDER(0,1);
		}
		
		for (; x<C;x++)
		{
			S[y]+=A_row[0]*Bp[0]; A_row++; Bp++;
		}

		A_row +=  Cp-C;

		S[y]+=sum_v4sp(xmm7);
	}
}

float* pad_matrix(float* Mo, int *Ro, int *Co)
{
	int x,y,R,C;
	float *M;
	R = *Ro;
	C = (*Co + 3 )>>2<<2;

	M=(float*)_mm_malloc(sizeof(float)*R*C,16);
	memset(M,0,sizeof(float)*R*C);

	for (y=0; y<*Ro; y++)
		for (x=0; x<*Co; x++)
			M[x+C*y]=Mo[x+ *Co*y];
	*Co=C;
	*Ro=R;

	return M;

}


void report(const char* title, float * S)
{
	int i;
	printf("%s: \n \t - result: ", title);

	for (i=0; i<NUM_R; i++)
	{
		if (i && ! (i%10))
			printf("\n\t           ");
		printf("%6.2f ",S[i]);
	}
	printf("\n");

	printf("\t - Elapsed: %f sec\n", ( 
				( tv_now.tv_sec   * 1000000.0  + tv_now.tv_usec   ) - 
				( tv_start.tv_sec * 1000000.0  + tv_start.tv_usec )
				)/1000000.0
	);
}

float* transpose_matrix (float *A, int R, int C)
{
	int i,j;
	float *AT;	
#ifdef SSE
	AT = ( float* ) _mm_malloc(DIM*NUM_R*sizeof(float),16);
#else
	AT = ( float* ) malloc(DIM*NUM_R*sizeof(float));
#endif
	for (i=0; i<R; i++)
		for (j=0; j<C; j++)
			AT[i*C+j] = A[j*R+i];
	return AT;

}

void print_matrix (float *A, int R, int C)
{
	int i,j;
	printf("\n\n(%d,%d)\n    --",R,C);
	for (j=0; j<C; j++)
	{
		printf ("%4d ", j);

	}
	for (i=0; i<R; i++)
	{
		printf("\n %2d ->", i);
		for (j=0; j<C; j++)
		{
			printf ("%3.2f ", A[i*C+j]);

		}
	}

	printf("\n");
}

void print_matrix_slice (float *A, int C, int rb, int cb, int re, int ce)
{
	int i,j;
	printf("\n\n  --  %d",C);
	for (j=cb; j<ce; j++)
	{
		printf ("%-4d ", j);

	}
	for (i=rb; i<re; i++)
	{
		printf("\n %2d ->", i);
		for (j=cb; j<ce; j++)
		{
			printf ("%3.2f ", A[i*C+j]);

		}
	}

	printf("\n");
}

float* optimize_matrix(float* Mo, int *Ro, int *Co)
{
	int x,y,t,R,C;
	float *M;
	int A1, A2;
	t = 4 - *Ro % 4;
	R = ( t == 4) ? *Ro :(*Ro + t);
	t = 4 - *Co % 4;
	C = ( t == 4) ? *Co :(*Co + t);

	M=(float*)_mm_malloc(sizeof(float)*R*C,16);
	memset(M,0,sizeof(float)*R*C);


	//printf ("Ro:%d - Co:%d\n", *Co, *Ro);
	for (y=0; y<*Ro; y++)
	{
		A2= (y* * Co);
		A1= ((y>>2<<2)* *Co) + y%4;
		//printf ("%d-> A1:%d A2:%d \n", y, A1, A2);

		for (x=0; x<*Co; x++)
		{
			t= x*4 + A1;
			//if (x==0 && y==4)
			//	printf("(x=0,y=4) t=%d\n",t);
			M[t]=Mo[x+ A2];
		}
	}
	*Co=C;
	*Ro=R;

	return M;

}

int compare_matrix (float *A1, float *A2, int R, int C)
{
	int i,j,res;
	res=1;
	for (i=0; i<R; i++)
		for (j=0; j<C; j++)
			if (A1[i*C+j]!=A2[i*C+j])
				return 0;
	return 1;
}

int main()
{
	unsigned int i, j, x, y;

	float * A;
	float * AT;
	float * B;
	float * S;

	float * A_row;

#ifdef SSE
	A  = ( float* ) _mm_malloc(DIM*NUM_R*sizeof(float),16);
	AT = ( float* ) _mm_malloc(DIM*NUM_R*sizeof(float),16);
	B  = ( float* ) _mm_malloc(DIM*sizeof(float),16);
	S  = ( float* ) _mm_malloc(NUM_R*sizeof(float),16);
#else
	A  = ( float* ) malloc(DIM*NUM_R*sizeof(float));
	AT = ( float* ) malloc(DIM*NUM_R*sizeof(float));
	B  = ( float* ) malloc(DIM*sizeof(float));
	S  = ( float* ) malloc(NUM_R*sizeof(float));
#endif

	for (i=0; i<DIM; i++)
	{
		B[i]=(float)(rand())/RAND_MAX;
	}
	
	for (i=0; i<NUM_R; i++)
	{
		for (j=0; j<DIM; j++)
		{
			A[i*DIM+j]=(float)rand()/RAND_MAX;
		}
	}

	AT=transpose_matrix(A, NUM_R, DIM);

	//transpose_matrix(AT, NUM_R, DIM);
	//printf( "Transpose works: %d\n", compare_matrix(A, AT, NUM_R, DIM));
	//transpose_matrix(AT, NUM_R, DIM);

	/*Init: put things into cache*/
	
	/*memset(S,0,DIM*sizeof(float));
	cblas_sgemv( CblasRowMajor, CblasNoTrans,
			NUM_R, DIM, 1.0f, A, DIM,
			B, 1, 1.0f, 
			S, 1 );
	*/
	int oC=DIM, oR=NUM_R;
	pad_matrix(A, &oR, &oC);
	
	//print_matrix(A,NUM_R, DIM);	
	//print_matrix(oA,oR, oC);	
	//print_matrix(B,1, DIM);	

#if SSE
	MatVectMult(oA, B, S, NUM_R, DIM, oC);
	{
		gettimeofday(&tv_start,NULL);

		for (i=0; i<NUM_IT;i++)
		{
			memset(S,0,DIM*sizeof(float));
			MatVectMult(oA, B, S, NUM_R, DIM, oC);
		}
		gettimeofday(&tv_now,NULL);
		report("INLINE SSE FUNCTION", S);	
		fflush(stdout);
	}
#endif
#if SSE
	MatVectMult(oA, B, S, NUM_R, DIM, oC);
	{
		gettimeofday(&tv_start,NULL);

		for (i=0; i<NUM_IT;i++)
		{
			memset(S,0,DIM*sizeof(float));
			MatVectMult(oA, B, S, NUM_R, DIM, oC);
		}
		gettimeofday(&tv_now,NULL);
		report("INLINE SSE FUNCTION", S);	
		fflush(stdout);
	}
#endif
	/* BLAS */
	cblas_sgemv( CblasRowMajor, CblasNoTrans,
			NUM_R, DIM, 1.0f, A, DIM,
			B, 1, 1.0f, 
			S, 1 );
	gettimeofday(&tv_start,NULL);
	for (i=0; i<NUM_IT;i++)
	{
		memset(S,0,DIM*sizeof(float));
		cblas_sgemv( CblasRowMajor, CblasNoTrans,
				NUM_R, DIM, 1.0f, A, DIM,
				B, 1, 1.0f, 
				S, 1 );
	}
	gettimeofday(&tv_now,NULL);
	report("BLAS",S);	
	fflush(stdout);

#if SSE
	MatVectMult(oA, B, S, NUM_R, DIM, oC);
	{
		gettimeofday(&tv_start,NULL);

		for (i=0; i<NUM_IT;i++)
		{
			memset(S,0,DIM*sizeof(float));
			MatVectMult(oA, B, S, NUM_R, DIM, oC);
		}
		gettimeofday(&tv_now,NULL);
		report("INLINE SSE FUNCTION", S);	
		fflush(stdout);
	}
#endif

	

return 0; //////////////////////////////// STOP

	/* SCALAR */
	gettimeofday(&tv_start,NULL);
	for (i=0; i<NUM_IT;i++)
	{
		memset(S,0,DIM*sizeof(float));
		A_row=A;
		for (y=0; y< NUM_R; y++)
		{
			for (x=0; x<DIM;x++)
				S[y]+=A_row[x]*B[x];
			A_row+=DIM;
		}
	}
	gettimeofday(&tv_now,NULL);
	report("SCALAR",S);	

	/* SCALAR 4*/
	gettimeofday(&tv_start,NULL);
	for (i=0; i<NUM_IT;i++)
	{
		memset(S,0,DIM*sizeof(float));
		A_row=A;
		for (y=0; y< NUM_R; y++)
		{
			for (x=0; x<DIM;x+=4)
			{
#if 1
				S[y]+=A_row[x]*B[x];
				S[y]+=A_row[x+1]*B[x+1];
				S[y]+=A_row[x+2]*B[x+2];
				S[y]+=A_row[x+3]*B[x+3];
#else
				S[y]+=A_row[x]*B[x]
					+A_row[x+1]*B[x+1]
					+A_row[x+2]*B[x+2]
					+A_row[x+3]*B[x+3];
#endif
			}
			A_row+=DIM;
		}
	}
	gettimeofday(&tv_now,NULL);
	report("SCALAR 4", S);	
	
#ifdef SSE
	/* SSE RowMajor */
#define ITER \
	xmm0 = _mm_load_ps(&A_row[0]);\
	xmm1 = _mm_load_ps(&A_row[4]);\
	xmm2 = _mm_load_ps(&A_row[8]);\
	\
	xmm3=_mm_load_ps(&Bp[0]);\
	xmm4=_mm_load_ps(&Bp[4]);\
	xmm5=_mm_load_ps(&Bp[8]);\
	xmm6=_mm_load_ps(&Bp[12]);\
	Bp+=16;\
	\
	xmm3 = _mm_mul_ps(xmm3, xmm0);\
	xmm0 = _mm_load_ps(&A_row[12]);\
	A_row+=16;\
	xmm4 = _mm_mul_ps(xmm4, xmm1);\
	xmm5 = _mm_mul_ps(xmm5, xmm2);\
	xmm6 = _mm_mul_ps(xmm6, xmm0);\
	\
	xmm3 = _mm_add_ps(xmm3, xmm4);\
	xmm5 = _mm_add_ps(xmm5, xmm6);\
	xmm7 = _mm_add_ps(xmm7, xmm3);\
	xmm7 = _mm_add_ps(xmm7, xmm5);\

#define ITERS 4

#define SKIP 16*ITERS


	/* SSE RowMajor */
	{
		__m128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
		float *Bp;
		int N=0;
		gettimeofday(&tv_start,NULL);
		for (i=0; i<NUM_IT;i++)
		{
			memset(S,0,DIM*sizeof(float));
			A_row=A;
			/*_mm_prefetch(B, _MM_HINT_T1);
			_mm_prefetch(B+16, _MM_HINT_T1);
			_mm_prefetch(B+32, _MM_HINT_T1);
			_mm_prefetch(B+48, _MM_HINT_T1);
			_mm_prefetch(B+96, _MM_HINT_T1);
			_mm_prefetch(S, _MM_HINT_T1);
			_mm_prefetch(S+16, _MM_HINT_T1);
			_mm_prefetch(S+32, _MM_HINT_T1);
			_mm_prefetch(S+48, _MM_HINT_T1);
			_mm_prefetch(S+96, _MM_HINT_T1);*/
			for (y=0; y< NUM_R; y++)
			{
				Bp=B;
				xmm7=_mm_set1_ps(0);
#if 0
				_mm_prefetch(A_row, _MM_HINT_NTA);
				_mm_prefetch(A_row+16, _MM_HINT_NTA);
				_mm_prefetch(A_row+32, _MM_HINT_NTA);
				_mm_prefetch(A_row+48, _MM_HINT_NTA);
				_mm_prefetch(A_row+96, _MM_HINT_NTA);
				for (x=0; x<=DIM-SKIP;x+=SKIP)
				{

					ITER
					ITER
					ITER
					ITER
					/*
					xmm0 = _mm_load_ps(&A_row[0]);
					xmm1 = _mm_load_ps(&A_row[4]);
					xmm2 = _mm_load_ps(&A_row[8]);

					xmm3=_mm_load_ps(&Bp[0]);
					xmm4=_mm_load_ps(&Bp[4]);
					xmm5=_mm_load_ps(&Bp[8]);
					xmm6=_mm_load_ps(&Bp[12]);
					Bp+=16;
					
					xmm3 = _mm_mul_ps(xmm3, xmm0);
					xmm0 = _mm_load_ps(&A_row[12]);
					A_row+=16;
					xmm4 = _mm_mul_ps(xmm4, xmm1);
					xmm5 = _mm_mul_ps(xmm5, xmm2);
					xmm6 = _mm_mul_ps(xmm6, xmm0);
					
					xmm3 = _mm_add_ps(xmm3, xmm4);
					xmm5 = _mm_add_ps(xmm5, xmm6);
					xmm7 = _mm_add_ps(xmm7, xmm3);
					xmm7 = _mm_add_ps(xmm7, xmm5);
					*/
				}
#else
				for (x=0; x<=DIM-28;x+=28)
				{
					ADDER(0,1)
					ADDER(2,3)
					ADDER(4,5)
					ADDER(6,0)
					ADDER(1,2)
					ADDER(3,4)
					ADDER(5,6)
				}
				
#endif
				for (; x<=DIM-4;x+=4)
				{
					ADDER(0,1)
				}
				
				for (; x<DIM;x++)
					S[y]+=A_row[x]*B[x];
				S[y]+=sum_v4sp(xmm7);
			}
		}
		gettimeofday(&tv_now,NULL);
	}
	report("SSE RowMajor", S);
	
	/* SSE RowMajor 3*/
	{
		__m128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
		unsigned int DIMx2=DIM*2, DIMx3=DIM*3, DIMx4=DIM*4, DIMx5=DIM*5;
		gettimeofday(&tv_start,NULL);
		for (i=0; i<NUM_IT;i++)
		{
			memset(S,0,DIM*sizeof(float));
			A_row=A;
			for (y=0; y< NUM_R; y+=3)
			{
				xmm4=_mm_set1_ps(0);
				xmm5=_mm_set1_ps(0);
				xmm6=_mm_set1_ps(0);
				xmm7=_mm_set1_ps(0);
				for (x=0; x<DIM;x+=4)
				{
					xmm0=_mm_load_ps(B+x);
					//_mm_prefetch(B+x+4, _MM_HINT_T1);

					xmm1=_mm_load_ps(A_row+x);
					//_mm_prefetch(A_row+x+DIMx3, _MM_HINT_NTA);
					xmm2=_mm_load_ps(A_row+DIM+x);
					//_mm_prefetch(A_row+x+DIMx4, _MM_HINT_NTA);
					xmm3=_mm_load_ps(A_row+DIMx2+x);
					//_mm_prefetch(A_row+x+DIMx5, _MM_HINT_NTA);

					xmm1 = _mm_mul_ps(xmm1, xmm0);
					xmm2 = _mm_mul_ps(xmm2, xmm0);
					xmm3 = _mm_mul_ps(xmm3, xmm0);

					xmm6 = _mm_add_ps(xmm6, xmm1);
					xmm5 = _mm_add_ps(xmm5, xmm2);
					xmm4 = _mm_add_ps(xmm4, xmm3);

				}
				S[y]+=sum_v4sp(xmm6);
				S[y+1]+=sum_v4sp(xmm5);
				S[y+2]+=sum_v4sp(xmm4);
				A_row+=DIMx3;
			}
		}
		gettimeofday(&tv_now,NULL);
	}
	report("SSE RowMajor 3", S);	
#if 1	
	/* SSE ColMajor*/
	{
		__m128 xmm0, xmm1, xmm2, xmm3, xmm4, xmm5, xmm6, xmm7;
		float * A_col;
		unsigned int DIMx2=DIM*2,DIMx3=DIM*3,DIMx4=DIM*4;
		unsigned int NUM_Rx2=NUM_R*2,NUM_Rx3=NUM_R*3,NUM_Rx4=NUM_R*4;
		int oR=NUM_R, oC=DIM;
		
/*		printf("A\n");
		print_matrix_slice(A, DIM, 0,0 , 4,4);
		
		printf("\noA\n");
		print_matrix_slice(oA, oC, 0,0 , 4,4);

		printf("\nB\n");
		print_matrix_slice(B, DIM, 0,0 , 1,4);
*/
		float *oA=optimize_matrix(A, &oR, &oC);
		gettimeofday(&tv_start,NULL);
		for (i=0; i<NUM_IT;i++) //NUM_IT
		{
			memset(S,0,DIM*sizeof(float));
			A_col=oA;
			for (y=0; y<NUM_R; y+=4) //ROWS
			{
#if 1
				xmm7=_mm_set1_ps(0);
#if 1
				for (x=0; x<DIM; x+=4) //COLS
				{

					xmm0 = _mm_load_ps(B+x);
					//_mm_prefetch(B+x+4, _MM_HINT_NTA);
					
					xmm3 = _mm_load_ps(A_col); A_col+=4;
					xmm4 = _mm_load_ps(A_col); A_col+=4;
					xmm5 = _mm_load_ps(A_col); A_col+=4;
					xmm6 = _mm_load_ps(A_col); A_col+=4;
					//_mm_prefetch(A_col, _MM_HINT_NTA);
					
					xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(0,0,0,0));
					xmm2 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1,1,1,1));
					xmm1 = _mm_mul_ps(xmm1, xmm3);
					xmm2 = _mm_mul_ps(xmm2, xmm4);
					xmm3 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(2,2,2,2));
					xmm4 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3,3,3,3));
					xmm3 = _mm_mul_ps(xmm3, xmm5);
					xmm4 = _mm_mul_ps(xmm4, xmm6);

					xmm0 = _mm_add_ps(xmm1, xmm2);
					xmm2 = _mm_add_ps(xmm3, xmm4);
					xmm0 = _mm_add_ps(xmm0, xmm2);
					xmm7 = _mm_add_ps(xmm7, xmm0);
				}
#else
				for (x=0; x<DIM; x+=1) //COLS
				{
					//xmm0 = _mm_load_ps(B+x);
					//_mm_prefetch(B+x+4, _MM_HINT_NTA);
					
					xmm3 = _mm_load_ps(A_col); A_col+=4;
					//xmm4 = _mm_load_ps(A_col); A_col+=4;
					//xmm5 = _mm_load_ps(A_col); A_col+=4;
					//xmm6 = _mm_load_ps(A_col); A_col+=4;
					//_mm_prefetch(A_col, _MM_HINT_NTA);
					
					//xmm1 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(0,0,0,0));
					//xmm2 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(1,1,1,1));
					xmm1 = _mm_load_ps1(&B[x]); //REMOVE THIS FOR FULL ALGO
					xmm1 = _mm_mul_ps(xmm1, xmm3);
					//xmm2 = _mm_mul_ps(xmm2, xmm4);
					//xmm3 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(2,2,2,2));
					//xmm4 = _mm_shuffle_ps(xmm0, xmm0, _MM_SHUFFLE(3,3,3,3));
					//xmm3 = _mm_mul_ps(xmm3, xmm5);
					//xmm4 = _mm_mul_ps(xmm4, xmm6);

					//xmm0 = _mm_add_ps(xmm1, xmm2);
					//xmm2 = _mm_add_ps(xmm3, xmm4);
					//xmm0 = _mm_add_ps(xmm0, xmm2);
					//xmm7 = _mm_add_ps(xmm7, xmm0);
					xmm7 = _mm_add_ps(xmm7, xmm1); //REMOVE THIS FOR FULL ALGO
				}
#endif
				_mm_store_ps(S+y, xmm7 );
#else
				int k=0;
				for (x=0; x<DIM; x+=4)
				{
				
					k=0;
#if 0	
					S[y+k]   += B[ x ]*A_col[ k ] + B[x+1]*A_col[4+k ] +
					            B[x+2]*A_col[8+k] + B[x+3]*A_col[12+k];
					k++;
					S[y+k]   += B[ x ]*A_col[ k ] + B[x+1]*A_col[4+k ] +
					            B[x+2]*A_col[8+k] + B[x+3]*A_col[12+k];
					k++;
					S[y+k]   += B[ x ]*A_col[ k ] + B[x+1]*A_col[4+k ] +
					            B[x+2]*A_col[8+k] + B[x+3]*A_col[12+k];
					k++;
					S[y+k]   += B[ x ]*A_col[ k ] + B[x+1]*A_col[4+k ] +
					            B[x+2]*A_col[8+k] + B[x+3]*A_col[12+k];
#else
					S[y]   += B[ x  ]*A_col[ 0 ] ;
					S[y+1] += B[ x  ]*A_col[ 1 ] ;
					S[y+2] += B[ x  ]*A_col[ 2 ] ;
					S[y+3] += B[ x  ]*A_col[ 3 ] ;
					S[y]   += B[x+1 ]*A_col[ 4 ] ;
					S[y+1] += B[x+1 ]*A_col[ 5 ] ;
					S[y+2] += B[x+1 ]*A_col[ 6 ] ;
					S[y+3] += B[x+1 ]*A_col[ 7 ] ;
					S[y]   += B[x+2 ]*A_col[ 8 ] ;
					S[y+1] += B[x+2 ]*A_col[ 9 ] ;
					S[y+2] += B[x+2 ]*A_col[10 ] ;
					S[y+3] += B[x+2 ]*A_col[11 ] ;
					S[y]   += B[x+3 ]*A_col[12 ] ;
					S[y+1] += B[x+3 ]*A_col[13 ] ;
					S[y+2] += B[x+3 ]*A_col[14 ] ;
					S[y+3] += B[x+3 ]*A_col[15 ] ;

#endif
					A_col+=16;
				}

#endif 
			}
		}
		gettimeofday(&tv_now,NULL);
	}
	report("SSE ColMajor", S);	
#endif /*0*/
#endif

	return 0;
}


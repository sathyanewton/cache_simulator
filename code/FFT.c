#include<stdio.h>
//#include<conio.h>
#include<math.h>
#include<time.h>


#define pi 3.14

typedef struct mat{		///Complex matrix

	float real;
	float imag;
}mat;

mat WN,WNk;

int main(){

	int i,j;	//for loop indices
	
	int N=4;		//Data points

	float theta1,theta2;


//	printf("Enter number of data points : ");

//	scanf("%d",&N);

	int n[4]={1,2,3,4};
	int k[N];
	int f1[N/2];
	int f2[N/2];

	mat F1[N];
	mat F2[N];
	mat F3[N];
	mat X[N];

	//Initialize output variables
	for(i=0;i<N;i++){
		F1[i].real = 0;
		F1[i].imag = 0;
		F2[i].real = 0;
		F2[i].imag = 0;
	}

//	/*Accept the input sequence from user */
//	for(i=0;i<N;i++){

//		printf("Enter x(%d) : ",i);
//		scanf("%d",&n[i]);
//	}

		
	//------------------------------------------------
	//start = clock();

	//Radix 2 FFT algorithm

	for(i=0;i<(N/2);i++){		//Split input sequence into odd and even

		f1[i] = n[2*i];			//odd  f1(m)
		f2[i] = n[2*i + 1];		//even f2(m)
		printf("%d",f1[i]);
		printf("%d\n",f2[i]);
	}

	for(i=0;i<N;i++){		// Get F1(k) and F2(k)

		for(j=0;j<(N/2 - 1);j++){
			
			theta1 = ((2*pi)/(N/2)) * (j*i);
			theta2 = ((2*pi)/N) * (i);
			
			F1[i].real = F1[i].real + f1[j]* cos(theta1);
			F1[i].imag = F1[i].imag + f1[j]* -sin(theta1);

			F2[i].real = F2[i].real + f1[j]* cos(theta1);
			F2[i].imag = F2[i].imag + f1[j]* -sin(theta1);



		}
		printf("\n%f + i%f",F1[i].real,F1[i].imag);
		printf("\n%f + i%f",F2[i].real,F2[i].imag);
	}

	printf("\n");

	WNk.real = cos(theta2);		//WnK
	WNk.imag = -sin(theta2);


	//Calculate WnK * F2(k)

	for(i=0;i<N;i++){

		F3[i].real = (WNk.real*F2[i].real) - (WNk.imag * F2[i].imag);
		F3[i].imag = (WNk.imag * F2[i].real) + (WNk.real * F2[i].imag);

		X[i].real = F1[i].real + F3[i].real;
		X[i].imag = F1[i].real + F3[i].imag;


		printf("\n%f + i%f",X[i].real,X[i].imag);
	}

	return 0;
	
}
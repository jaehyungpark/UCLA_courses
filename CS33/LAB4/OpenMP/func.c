// Jaehyung Park
// UID: 504212821

#include "func.h"
#include "util.h"

void func0(double *weights, double *arrayX, double *arrayY, int xr, int yr, int n)
{
	int i;
    double dw, dx, dy;
    dw = 1/((double)(n));
    dx = xr;
    dy = yr;

#pragma omp parallel for num_threads(28) firstprivate(n,dw,weights,arrayX,arrayY,dx,dy) private(i)
	for(i = 0; i < n; i++){
		weights[i] = dw;
        arrayX[i] = dx;
		arrayY[i] = dy;
	}
}

void func1(int *seed, int *array, double *arrayX, double *arrayY,
			double *probability, double *objxy, int *index,
			int Ones, int iter, int X, int Y, int Z, int n)
{
	int i, j, iOnes, arrayIndexiOnes;
   	int index_X, index_Y;
	int max_size = X*Y*Z;
    double dox;
    dox = (double) Ones;

#pragma omp parallel for num_threads(28) firstprivate(n,arrayX,arrayY,seed) private(i)
   	for(i = 0; i < n; i++){
   		arrayX[i] += 1 + 5*rand2(seed, i);
   		arrayY[i] += -2 + 2*rand2(seed, i);
   	}

#pragma omp parallel for num_threads(28) firstprivate(Ones,n,iter,max_size,Z,Y,arrayX,arrayY,objxy,array,dox,index) private(index_X,index_Y,i,j,iOnes,arrayIndexiOnes)
   	for(i = 0; i<n; i++){
        iOnes = i*Ones;
   		for(j = 0; j < Ones; j++){
   			index_X = round(arrayX[i]) + objxy[j*2 + 1];
   			index_Y = round(arrayY[i]) + objxy[j*2];
   			index[iOnes + j] = fabs(index_X*Y*Z + index_Y*Z + iter);
   			if(index[iOnes + j] >= max_size)
   				index[iOnes + j] = 0;
   		}
   		probability[i] = 0;

   		for(j = 0; j < Ones; j++) {
            arrayIndexiOnes = array[index[iOnes + j]];
   			probability[i] += (pow((arrayIndexiOnes-100),2) -
   							  pow((arrayIndexiOnes-228),2))/50.0;
   		}
   		probability[i] = probability[i]/dox;
   	}
}

void func2(double *weights, double *probability, int n)
{
	int i;
	double sumWeights=0;
    double dw;

#pragma omp parallel for num_threads(28) firstprivate(n,weights,probability) reduction(+:sumWeights) private(i,dw)
	for(i = 0; i < n; i++){
   		dw = weights[i] * exp(probability[i]);
        weights[i] = dw;
        sumWeights += dw;
    }

#pragma omp parallel for num_threads(28) firstprivate(n,weights,sumWeights) private(i)
	for(i = 0; i < n; i++)
   		weights[i] = weights[i]/sumWeights;
}

void func3(double *arrayX, double *arrayY, double *weights, double *x_e, double *y_e, int n)
{
	double estimate_x=0.0;
	double estimate_y=0.0;
    int i;
    double dw;

#pragma omp parallel for num_threads(28) firstprivate(n,arrayX,arrayY,weights) reduction(+:estimate_x,estimate_y) private(i, dw)
	for(i = 0; i < n; i++){
        dw = weights[i];
   		estimate_x += arrayX[i] * dw;
   		estimate_y += arrayY[i] * dw;
   	}

	*x_e = estimate_x;
	*y_e = estimate_y;

}

void func4(double *u, double u1, int n)
{
	int i;
    double l;
    l = (double)(n);

#pragma omp parallel for num_threads(28) firstprivate(n,u,u1,l) private(i)
	for(i = 0; i < n; i++){
   		u[i] = u1 + i/l;
   	}
}

void func5(double *x_j, double *y_j, double *arrayX, double *arrayY, double *weights, double *cfd, double *u, int n)
{
	int i, j;
    double dw;
    dw = 1/((double)(n));

#pragma omp parallel for num_threads(28) firstprivate(n,u,cfd,arrayX,arrayY,x_j,y_j) private(i,j)
	for(j = 0; j < n; j++){
   		//i = findIndex(cfd, n, u[j]);
   		i = findIndexBin(cfd, 0, n, u[j]);
   		if(i == -1)
   			i = n-1;
   		x_j[j] = arrayX[i];
   		y_j[j] = arrayY[i];

   	}

#pragma omp parallel for num_threads(28) firstprivate(n,arrayX,arrayY,weights,dw,x_j,y_j) private(i)
	for(i = 0; i < n; i++){
		arrayX[i] = x_j[i];
        arrayY[i] = y_j[i];
		weights[i] = dw;
	}
}

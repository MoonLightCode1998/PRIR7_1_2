#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mpi.h"
#include <unistd.h>
MPI_Status statusMPI;
int Mandel[9000000];
int wyznaczx(int rank,int height, int punkty){
return (rank*punkty)/height;
}
int wyznaczy(int rank,int height,int punkty){
return (rank*punkty)%height;
}
void mandluj(int punkty, int width, int height,double dy,double dx,int x,int y,
double x0,double y0, double x1, double y1,int iter){

        double Xre,Xim,Yre,Yim,Tre,Tim;
//printf("%d",x);  
     int k;
int lm = x +750;
printf("%d\n",lm);
	for(int xx=x;xx<lm;xx++){
	//printf("%d\n",x);


                Xre=xx*dx+x0;
                for(int yy=y;yy<3000;yy++){
                
                k=0;
                Xim=yy*dy+y0;
                Yre=0;
                Yim=0;
                do{
                        Tre=Yre*Yre-Yim*Yim;
                        Tim=2*Yre*Yim;
                        Yre=Tre;
                        Yim=Tim;
                        Yre=Yre+Xre;
                        Yim=Yim+Xim;
                        k++;
                }while(k<iter&&(Yre*Yre+Yim*Yim)<4);
                Mandel[xx*3000+yy]=k;
                }
        }
}




int main(int argc, char** argv){
int rank,size;

	MPI_Init(&argc,&argv);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

double x0 =-0.82;
double y0 = 0.1;
double x1=-0.7;
double y1=0.22;
int iter =256;
int width=3000;
int height=3000;
double dx;
double dy;
double  points;
FILE *file;
MPI_Status status;
	if(rank==0){
	file=fopen("/home/oem/mendel.txt","w+"); 
	dy=(y1-y0)/height;
	  dx =(x1-x0)/width;
	 points = (width*height)/size;
	printf("TO POWINNO BYC DY %f\n", dy);}
        MPI_Bcast(&dx,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&dy,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&points,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&x0,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&y0,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&x1,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&y1,1,MPI_DOUBLE,0,MPI_COMM_WORLD);
        MPI_Bcast(&width,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&height,1,MPI_INT,0,MPI_COMM_WORLD);
        MPI_Bcast(&iter,1,MPI_INT,0,MPI_COMM_WORLD);	


//                printf("%f %f %f %f %f %f %f %d %d %d\n",dx,dy,points,
  //                      x0,y0,x1,y1,width,height,iter);
	mandluj(points,width,height,dy,dx,wyznaczx(rank,height,points),
		wyznaczy(rank,height,points),x0,y0,x1,y1,iter);
//printf("%f",points);
//printf("%d %d \n",wyznaczx(rank,height,points),wyznaczy(rank,height,points));
MPI_Barrier(MPI_COMM_WORLD);	
if(rank==0){
for(int x=0;x<9000000;x++){
fprintf(file,"%d\n",Mandel[x]);
}
fclose(file);
}
	MPI_Finalize();	

return 0;
}

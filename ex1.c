#include <stdio.h>  

int main()
{ 
	printf("teste");
	
	int nx=100;
	int nt=200;
	float omega=0.8;
	
	float temperaturas[nx];
	float f1[nx];
	float f2[nx];
	float feq;
	
	/*Inicializando as temperaturas*/
	for(int x=1;x<nx;x++){
		temperaturas[x]=0;}
	temperaturas[0]=1;
	/*Inicializando f1 e f2*/
	for(int x=0;x<nx;x++)
	{
		f1[x]=0.5*temperaturas[x];
		f2[x]=0.5*temperaturas[x];
	}
	/*Loop temporal*/
	for(int t=1;t<nt;t++)
	{
		/*Colisao*/
		for(int x=0;x<nx;x++)
		{
			feq=0.5*temperaturas[x];
			f1[x]=f1[x]*(1-omega)+omega*feq;
			f2[x]=f2[x]*(1-omega)+omega*feq;
		}
		/*Streaming*/
		for(int x=1;x<nx;x++)
		{
			f1[nx-x]=f1[nx-x-1];
			f2[x-1]=f2[x];
		}
		/*Condiçoes de contorno*/
		f2[nx]=f2[nx-1]; //gradiente de temperatura zero
		f1[nx]=f1[nx-1];   //gradiente de temperatura zero
		f1[0]=1-f2[0];   //nao conseguimos obter f1 do processo de streaming, apenas f2

			
		/*Atualizando temperaturas*/
		for(int x=0;x<nx;x++)
		{
			temperaturas[x]=f1[x]+f2[x];
		}
	}
	


//Saving data
    FILE *dataFile = fopen("/home/joao/Documents/PraticaLBM/caso1teste.txt", "w");
    if (dataFile == NULL) {
            perror("Erro ao abrir o arquivo");
    }
    for(int j = 0; j < nt; j++){
        for(int x = 0; x < nx; x++){
            fprintf(dataFile, "%d %f \n", x, temperaturas[x]);
        }
    }
    fclose(dataFile);
    
    printf("teste");

   return 1;
}

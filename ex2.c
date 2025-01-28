#include <stdio.h>
#include <math.h>

int main(){

int nx=50;
int ny=50;
float csqd=0.5; //para d2q4, cs ao quadrado vale 1/2
float alpha=0.25; 
float omega=pow(alpha/csqd+0.5,-1);
int tmax=200; //vamos usar 200 intervalos de tempo

float f1[nx][ny];
float f2[nx][ny];
float f3[nx][ny];
float f4[nx][ny];
float feq;    //como todas tem o mesmo peso, so definimos 1 feq
float w=0.25; //como todas tem o mesmo peso, so definimos 1 variavel peso

/*Inicializando as temperaturas, 1 nas paredes e 0 no restante*/

float temperaturas[nx][ny];
for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
		if(x==0 ||y==0){
			temperaturas[x][y]=1;
		}else{
			temperaturas[x][y]=0;}
	}
}

/*Inicializando f1,f2,f3 e f4*/

for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
		f1[x][y]=w*temperaturas[x][y];
		f2[x][y]=w*temperaturas[x][y];
		f3[x][y]=w*temperaturas[x][y];
		f4[x][y]=w*temperaturas[x][y];
	}
}

/*Loop temporal*/

for (int t=0;t<tmax;t++)
{
	/*Colisao*/
	for (int x=0;x<nx;x++){
		for (int y=0;y<ny;y++){
			feq=w*temperaturas[x][y]; //deveriamos ter uma feq para cada direcao, mas nesse caso sao iguais
			f1[x][y]=f1[x][y]*(1-omega)+omega*feq;
			f2[x][y]=f2[x][y]*(1-omega)+omega*feq;
			f3[x][y]=f3[x][y]*(1-omega)+omega*feq;
			f4[x][y]=f4[x][y]*(1-omega)+omega*feq;
		}
	}
	/*Transmissao*/
	for(int x=1;x<nx;x++){
		for(int y=1;y<ny;y++){
			f1[nx-x][y]=f1[nx-x-1][y]; //f1 vai p direita
			f2[x-1][y]=f2[x][y]; //f2 vai p esquerda
		}
	}
	for(int y=1;y<ny;y++){
		for(int x=1;x<nx;x++){
			f3[x][ny-y]=f3[x][ny-y-1]; //f3 vai p cima
			f4[x][y-1]=f4[x][y]; //f4 vai p baixo
		}
	}
	
	/*Condiçoes de contorno*/
	/*f2 e f4 sao obtidas pela etapa de transmissao, de forma que nos resta determinar f1 e f3. da conservacao de fluxo: f1 = 0.5C1 − f2, o mesmo para f3 e f4*/
	for(int y=0;y<ny;y++){
		f1[0][y]=0.5*1-f2[0][y];   //nao conseguimos obter f1 do processo de streaming, apenas f2
	}
	for(int x=0;x<nx;x++){
		f3[x][0]=0.5*1-f4[x][0];   //nao conseguimos obter f3 do processo de streaming, apenas f4
	}
			
	
	/*Atualizando temperaturas*/
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			temperaturas[x][y]=f1[x][y]+f2[x][y]+f3[x][y]+f4[x][y];
		}
	}
} 

FILE *file;

 // Abre o arquivo para escrita
    file = fopen("/home/joao/Documents/PraticaLBM/temperaturas.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Escreve a matriz `temperaturas` no arquivo
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            fprintf(file, "%f ", temperaturas[i][j]);
        }
        fprintf(file, "\n");  // Nova linha após cada linha da matriz
    }

    // Fecha o arquivo
    fclose(file);



/*
//Saving data
    FILE *dataFile = fopen("/home/joao/Documents/PraticaLBM/caso2teste.txt", "w");
    if (dataFile == NULL) {
            perror("Erro ao abrir o arquivo");
    }
    for(int j = 0; j < nt; j++){
        for(int x = 0; x < nx; x++){
				for(int y = 0; y < ny; y++){
					fprintf(dataFile, "%d %f - ", x, temperaturas[x][y]);     
				}
				fprintf(dataFile, "\n ");
		}
	}
    fclose(dataFile);
*/

return 0;
	
}
	
	
	
	


// codigo D2Q5 para adveccao
#include <stdio.h>
#include <math.h>

int main(){

int nx=100;
int ny=100;
float csqd=0.33333333333; //para d2q4, cs ao quadrado vale 1/3
float alpha=1; 
float omega=pow(alpha/csqd+0.5,-1);
int tmax=400; //vamos usar 400 intervalos de tempo

float f0[nx][ny];
float f1[nx][ny];
float f2[nx][ny];
float f3[nx][ny];
float f4[nx][ny];
float feq0;        //feq da particula central
float feq1;        //feq das particulas 
float feq2;        //feq das particulas 
float feq3;        //feq das particulas 
float feq4;        //feq das particulas 
float wcentral=0.33333333333; //peso central
float wlat=0.166666666666;     //pesos laterais
float u=0.1;
float v=0.4;
float ckucs[5]={0,3*u,-3*u,3*v,-3*v};   // produto escalar que determina o termo de adveccao

/*Inicializando as temperaturas, 1 nas paredes e 0 no restante*/


float temperaturas[nx][ny];
for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
		if(x==0){
			temperaturas[x][y]=1;
		}else{
			temperaturas[x][y]=0;}
	}
}

/*Inicializando f0,f1,f2,f3 e f4*/

for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
		f0[x][y]=wcentral*temperaturas[x][y];
		f1[x][y]=wlat*temperaturas[x][y];
		f2[x][y]=wlat*temperaturas[x][y];
		f3[x][y]=wlat*temperaturas[x][y];
		f4[x][y]=wlat*temperaturas[x][y];
	}
}

/*Loop temporal*/

for (int t=0;t<tmax;t++){
	/*Colisao*/
	for (int x=0;x<nx;x++){
		for (int y=0;y<ny;y++){
			feq0=wcentral*temperaturas[x][y]*(1+ckucs[0]); //feq para a do centro
			feq1=wlat*temperaturas[x][y]*(1+ckucs[1]);     //feq para as laterais
			feq2=wlat*temperaturas[x][y]*(1+ckucs[2]);     //feq para as laterais
			feq3=wlat*temperaturas[x][y]*(1+ckucs[3]);     //feq para as laterais
			feq4=wlat*temperaturas[x][y]*(1+ckucs[4]);     //feq para as laterais
			f0[x][y]=f0[x][y]*(1-omega)+omega*feq0;
			f1[x][y]=f1[x][y]*(1-omega)+omega*feq1;
			f2[x][y]=f2[x][y]*(1-omega)+omega*feq2;
			f3[x][y]=f3[x][y]*(1-omega)+omega*feq3;
			f4[x][y]=f4[x][y]*(1-omega)+omega*feq4;
		}
	}
	/*Transmissao (nao temos transmissao para a particula do centro)*/
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
	for(int y=0;y<ny;y++){
		f1[0][y]=1-f2[0][y]-f0[0][y]-f3[0][y]-f4[0][y];   //temperatura na parede x=0 vai ser 1
		f2[nx-1][y]=f1[nx-1][y];
	}
	for(int x=0;x<nx;x++){
		f3[x][0]=f4[x][0];   //nao conseguimos obter f3 do processo de streaming, apenas f4
		f4[x][ny-1]=-(f0[x][ny-1]+f1[x][ny-1]+f2[x][ny-1]+f3[x][ny-1]); //temperatura na borda final vai ser 0
	}
			
	
	/*Atualizando temperaturas*/
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			temperaturas[x][y]=f0[x][y]+f1[x][y]+f2[x][y]+f3[x][y]+f4[x][y];
		}
	}
} 


FILE *file;

 // Abre o arquivo para escrita
    file = fopen("/home/joao/Documents/PraticaLBM/temperaturasd2q5adv.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 1;
    }

    // Escreve a matriz `temperaturas` no arquivo
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(file, "%f ", temperaturas[i][j]);
        }
        fprintf(file, "\n");  // Nova linha após cada linha da matriz
    }

    // Fecha o arquivo
    fclose(file);


return 0;
	
}


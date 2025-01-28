#include <stdio.h>
#include <math.h>

int main(){

int nx=50;
int ny=50;
float csqd=0.33333333333; //para d2q5, cs ao quadrado vale 1/3
float alpha=0.25; 
float omega=pow(alpha/csqd+0.5,-1);
int tmax=200; //vamos usar 200 intervalos de tempo

float f0[nx][ny];
float f1[nx][ny];
float f2[nx][ny];
float f3[nx][ny];
float f4[nx][ny];
float feqlat;        //feq das particulas laterais
float feqcentral;    //feq da particula central
float wcentral=0.33333333333; //peso central
float wlat=0.166666666666;     //pesos laterais

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
			feqcentral=wcentral*temperaturas[x][y]; //feq para a do centro
			feqlat=wlat*temperaturas[x][y]; //feq para as laterais
			f0[x][y]=f0[x][y]*(1-omega)+omega*feqcentral;
			f1[x][y]=f1[x][y]*(1-omega)+omega*feqlat;
			f2[x][y]=f2[x][y]*(1-omega)+omega*feqlat;
			f3[x][y]=f3[x][y]*(1-omega)+omega*feqlat;
			f4[x][y]=f4[x][y]*(1-omega)+omega*feqlat;
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
			temperaturas[x][y]=f0[x][y]+f1[x][y]+f2[x][y]+f3[x][y]+f4[x][y];
		}
	}
} 

FILE *file;

 // Abre o arquivo para escrita
    file = fopen("/home/joao/Documents/PraticaLBM/temperaturas5.txt", "w");
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


return 0;
	
}

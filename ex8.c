#include <stdio.h>
#include <math.h>

int main(){

int nx=100;
int ny=100;
float f[9][nx][ny];   // f
float g[9][nx][ny];   // f temperatura
float feq[9][nx][ny];        //feq do fluido
float geq[9][nx][ny];        //feq da temperatura
float w[9]={0.4444444444,0.1111111111,0.1111111111,0.1111111111,0.1111111111,0.0277777777,0.0277777777,0.0277777777,0.0277777777}; //pesos
float cx[9]={0,1,0,-1,0,1,-1,-1,1};   //componentes da vel em x
float cy[9]={0,0,1,0,-1,1,1,-1,-1};	  //componentes da vel em y
float u0=0.1;
float alpha=0.01;  // viscosidade
float pr=0.71;
float alphag=alpha/pr;   // coef alpha
//float re=u0*nx/alpha=400;
float omega=1/(3*alpha+0.5);   //omega para escoamento
float omegag=1/(3*alphag+0.5);	//omega para parte termica
float count=0; 
float tol=0.00001;
float erro=10.0;
float ers;
float erso=0.0;
float v[nx][ny];
float u[nx][ny];
float t1;
float t2;
float rhon;


// inicializando densidade
float densidade[nx][ny];
float temperatura[nx][ny];
for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
			densidade[x][y]=1;
			temperatura[x][y]=0;
}}
//inicializando f e g
for(int i=0;i<9;i++){
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			f[i][x][y]=0;
			g[i][x][y]=0;
			}}}
			
//inicializando u e v		
for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
	u[x][y]=0;
	v[x][y]=0; }}
			
//inicializando vel e T da superficie superior
for(int x=0;x<nx;x++){
	u[x][ny-1]=u0;
	temperatura[x][ny-1]=1;
	}


/*Loop temporal*/

while(erro>tol){
	/*Colisao fluido*/
	for (int x=0;x<nx;x++){
		for (int y=0;y<ny;y++){
			t1=pow(u[x][y],2)+pow(v[x][y],2);
			for(int i=0;i<9;i++){
				t2=u[x][y]*cx[i]+v[x][y]*cy[i];
				feq[i][x][y]=w[i]*densidade[x][y]*(1+3*t2+4.5*pow(t2,2)-1.5*t1);
				f[i][x][y]=f[i][x][y]*(1-omega)+omega*feq[i][x][y];
			}
		}
	}
	/*Transmissao fluido (nao temos transmissao para a particula do centro)*/
	for(int x=1;x<nx;x++){
		for(int y=1;y<ny;y++){ //trocamos para ny-1
			f[1][nx-x][y]=f[1][nx-x-1][y]; //f1 vai p direita
			f[2][x][ny-y]=f[2][x][ny-y-1]; //f2 vai p cima
			f[3][x-1][y]=f[3][x][y]; //f3 vai p esquerda
			f[4][x][y-1]=f[4][x][y]; //f4 vai p baixo
			f[5][nx-x][ny-y]=f[5][nx-x-1][ny-y-1]; //f5 vai p direita e p cima
			f[6][x-1][ny-y]=f[6][x][ny-y-1]; //f6 vai p esquerda e p cima
			f[7][x-1][y-1]=f[7][x][y]; //f7 vai p esquerda e p baixo
			f[8][nx-x][y-1]=f[8][nx-x-1][y]; //f8 vai p direita e p baixo
		}
	}

	/*Condiçoes de contorno fluido*/
	//a) bounce-back na fronteira esquerda
	for(int y=0;y<ny;y++){
		f[1][0][y]=f[3][0][y];  //f1 recebe f3 na parede esquerda para todo y
		f[5][0][y]=f[7][0][y];  //f5 recebe f7 na parede esquerda para todo y
		f[8][0][y]=f[6][0][y];  //f8 recebe f6 na parede esquerda para todo y
		}
	//b) bounce-back na fronteira direita
	for(int y=0;y<ny;y++){
		f[3][nx-1][y]=f[1][nx-1][y];  //f3 recebe f1 na parede direita para todo y
		f[7][nx-1][y]=f[5][nx-1][y];  //f7 recebe f5 na parede direita para todo y
		f[6][nx-1][y]=f[8][nx-1][y];  //f6 recebe f8 na parede direita para todo y
		}
	//c) bounce-back na fronteira inferior
	for(int x=0;x<nx;x++){
		f[2][x][0]=f[4][x][0];  //f2 recebe f4 na parede inferior para todo x
		f[5][x][0]=f[7][x][0];  //f5 recebe f7 na parede inferior para todo x
		f[6][x][0]=f[8][x][0];  //f6 recebe f8 na parede inferior para todo x
		}
	//d) movimento da fronteira superior
	for(int x=1;x<nx;x++){
		rhon=f[0][x][ny-1]+f[1][x][ny-1]+f[3][x][ny-1]+2*(f[2][x][ny-1]+f[6][x][ny-1]+f[5][x][ny-1]);
		f[4][x][ny-1]=f[2][x][ny-1];  //f4 recebe f2 na parede superior para todo x
		f[7][x][ny-1]=f[5][x][ny-1]-rhon*u0/6;  //f5 recebe f7 na parede inferior para todo x
		//f[7][x][ny-1]=f[5][x][ny-1]-0.5*(f[3][x][ny-1]-f[1][x][ny-1])-rhon*u0*0.5;  //codigo livro
		f[8][x][ny-1]=f[6][x][ny-1]+rhon*u0/6;  //f6 recebe f8 na parede inferior para todo x
		//f[8][x][ny-1]=f[6][x][ny-1]+0.5*(f[3][x][ny-1]-f[1][x][ny-1])+rhon*u0*0.5; //codigo livro
		}
	// seguindo o codigo do apendice, mas diferente do livro
		
	/*Atualizando rho*/
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			densidade[x][y]=f[0][x][y]+f[1][x][y]+f[2][x][y]+f[3][x][y]+f[4][x][y]+f[5][x][y]+f[6][x][y]+f[7][x][y]+f[8][x][y];
		}
	}
	for(int x=0;x<nx;x++){
		densidade[x][ny-1]=f[1][x][ny-1]+f[3][x][ny-1]+f[0][x][ny-1]+2*(f[2][x][ny-1]+f[5][x][ny-1]+f[6][x][ny-1]);
		}
	
	/*Atualizando u e v*/
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			u[x][y]=(f[1][x][y]+f[5][x][y]+f[8][x][y]-(f[3][x][y]+f[6][x][y]+f[7][x][y]))/densidade[x][y];
			v[x][y]=(f[2][x][y]+f[5][x][y]+f[6][x][y]-(f[4][x][y]+f[7][x][y]+f[8][x][y]))/densidade[x][y];
			}}
			
//Repetimos agora o procedimento para a parte termica

	/*Colisao termica*/
		for (int x=0;x<nx;x++){
			for (int y=0;y<ny;y++){
				t1=pow(u[x][y],2)+pow(v[x][y],2);
				for(int i=0;i<9;i++){
					t2=u[x][y]*cx[i]+v[x][y]*cy[i];
					geq[i][x][y]=w[i]*temperatura[x][y]*(1+3*t2+4.5*pow(t2,2)-1.5*t1);
					g[i][x][y]=g[i][x][y]*(1-omegag)+omegag*geq[i][x][y];
				}
			}
		}
	/*Transmissao termica (nao temos transmissao para a particula do centro)*/
	for(int x=1;x<nx;x++){
		for(int y=1;y<ny;y++){ //trocamos para ny-1
			g[1][nx-x][y]=g[1][nx-x-1][y]; //f1 vai p direita
			g[2][x][ny-y]=g[2][x][ny-y-1]; //f2 vai p cima
			g[3][x-1][y]=g[3][x][y]; //f3 vai p esquerda
			g[4][x][y-1]=g[4][x][y]; //f4 vai p baixo
			g[5][nx-x][ny-y]=g[5][nx-x-1][ny-y-1]; //f5 vai p direita e p cima
			g[6][x-1][ny-y]=g[6][x][ny-y-1]; //f6 vai p esquerda e p cima
			g[7][x-1][y-1]=g[7][x][y]; //f7 vai p esquerda e p baixo
			g[8][nx-x][y-1]=g[8][nx-x-1][y]; //f8 vai p direita e p baixo
		}
	}
	/*Condiçoes de contorno para a temperatura*/
	//a) bounce-back na parede esquerda
	for(int y=0;y<ny;y++){
		g[1][0][y]=-g[3][0][y];
		g[5][0][y]=-g[7][0][y];
		g[8][0][y]=-g[6][0][y];
		}
	//b) bounce-back na parede direita
	for(int y=0;y<ny;y++){
		g[3][nx-1][y]=-g[1][nx-1][y];
		g[7][nx-1][y]=-g[5][nx-1][y];
		g[6][nx-1][y]=-g[8][nx-1][y];
		}
	//c) bounce-back na parede inferior
	for(int x=0;x<nx;x++){
		g[2][x][0]=g[4][x][0];
		g[5][x][0]=g[7][x][0];
		g[6][x][0]=g[8][x][0];
		}
	//d) tampa se movendo com velocidade u0 e temperatura 1
	for(int x=0;x<nx;x++){
		g[4][x][ny-1]=(w[4]+w[2])*1-g[2][x][ny-1];
		g[7][x][ny-1]=(w[8]+w[6])*1-g[6][x][ny-1];
		g[8][x][ny-1]=(w[7]+w[5])*1-g[5][x][ny-1];
		}
	/*Atualizando temperatura*/
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			temperatura[x][y]=g[0][x][y]+g[1][x][y]+g[2][x][y]+g[3][x][y]+g[4][x][y]+g[5][x][y]+g[6][x][y]+g[7][x][y]+g[8][x][y];
		}
	}



	count=count+1;
	ers=0;
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			ers=ers+pow(u[x][y],2)+pow(v[x][y],2);			
		}
	}
	erro=fabs(ers-erso);
	erso=ers;
	
	printf("ok");			
} 


//impressao
// Escreve a matriz 'u' no arquivo
	FILE *fileu = fopen("/home/joao/Documents/PraticaLBM/utermico.txt", "w");
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(fileu, "%.4f ", u[i][j]);  // Salva cada valor com duas casas decimais
        }
        fprintf(fileu, "\n");
    }
    fclose(fileu);

    // Escreve a matriz 'v' no arquivo
    FILE *filev = fopen("/home/joao/Documents/PraticaLBM/vtermico.txt", "w");
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(filev, "%.4f ", v[i][j]);
        }
        fprintf(filev, "\n");
    }
    fclose(filev);
    
	// Escreve a matriz 'temperaturas' no arquivo
	FILE *filet = fopen("/home/joao/Documents/PraticaLBM/temperaturasescoamento.txt", "w");
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(filet, "%.4f ", temperatura[i][j]);  // Salva cada valor com duas casas decimais
        }
        fprintf(filet, "\n");
    }
    fclose(filet);
	
	
	return 0;
	}

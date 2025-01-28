#include <stdio.h>
#include <math.h>

int main(){

int nx=200;
float f[9][nx][ny];   // f
float feq[9][nx][ny];        //feq das particulas
float w[9]={0.4444444444,0.1111111111,0.1111111111,0.1111111111,0.1111111111,0.0277777777,0.0277777777,0.0277777777,0.0277777777}; //pesos
float cx[9]={0,1,0,-1,0,1,-1,-1,1};   //componentes da vel em x
float cy[9]={0,0,1,0,-1,1,1,-1,-1};	  //componentes da vel em y
float u0=0.1;
float alpha=0.01;
//float re=u0*nx/alpha=1000;
float omega=1/(3*alpha+0.5);
float count=0; 
float tol=0.0001;
float erro=10.0;
float ers;
float erso=0.0;
float v[nx][ny];
float u[nx][ny];
float rhon;
float t1;
float t2;

// inicializando densidade
float densidade[nx][ny];
for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
			densidade[x][y]=1;
}}
//inicializando f e feq
for(int i=0;i<9;i++){
	for(int x=0;x<nx;x++){
		for(int y=0;y<ny;y++){
			feq[i][x][y]=0;
			f[i][x][y]=0;
			}}}
//inicializando u e v		
for (int x=0;x<nx;x++){
	for (int y=0;y<ny;y++){
	u[x][y]=0;
	v[x][y]=0; }}
			
//inicializando vel da superficie superior
for(int x=0;x<nx;x++){
	u[x][ny-1]=u0;}

/*% Collition
function [f]=collision(nx,ny,u,v,cx,cy,omega,f,rho,w)
for j=1:ny
for i=1:nx
t1=u(i,j)*u(i,j)+v(i,j)*v(i,j);
for k=1:9
t2=u(i,j)*cx(k)+v(i,j)*cy(k);
feq(i,j,k)=rho(i,j)*w(k)*(1.0+3.0*t2+4.5*t2*t2-1.5*t1);
f(i,j,k)=(1.-omega)*f(i,j,k)+omega*feq(i,j,k);
end
end
end
end*/

/*Loop temporal*/

while(erro>tol){
	/*Colisao*/
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
	/*Transmissao (nao temos transmissao para a particula do centro)*/
	for(int x=1;x<nx;x++){
		for(int y=1;y<ny;y++){
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
/*
 function [f]=boundary(nx,ny,f,uo)
%Boundary condition:
%left boundary, bounc back
f(1,:,1)=f(1,:,3);
f(1,:,5)=f(1,:,7);
f(1,:,8)=f(1,:,6);
%right hand boundary
f(nx,:,3)=f(nx,:,1);
f(nx,:,7)=f(nx,:,5);
f(nx,:,6)=f(nx,:,8);
%bottom boundary, bounce back
f(:,1,2)=f(:,1,4);
f(:,1,5)=f(:,1,7);
f(:,1,6)=f(:,1,8);
%Top boundary,moving lid with uo
for i=2:nx-1
rhon=f(i,ny,9)+f(i,ny,1)+f(i,ny,3)+2.*(f(i,ny,2)+f(i,ny,6)+f(i,ny,5));
f(i,ny,4)=f(i,ny,2);
f(i,ny,8)=f(i,ny,6)+rhon*uo/6.0;
f(i,ny,7)=f(i,ny,5)-rhon*uo/6.0;
end
 */	
	/*Condiçoes de contorno*/
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
		f[8][x][ny-1]=f[6][x][ny-1]+rhon*u0/6;  //f6 recebe f8 na parede inferior para todo x
		}
	// seguindo o codigo do apendice, mas diferente do livro
/*
 function[rho,u,v]=ruv(nx,ny,f)
rho=sum (f,3);
for i=1:nx
rho(i,ny)=f(i,ny,9)+f(i,ny,1)+f(i,ny,3)+2.*(f(i,ny,2)+f(i,ny,6)+f(i,ny,5));
end
%calculate velocity compnents
u = ( sum(f(:,:,[1 5 8]),3) - sum(f(:,:,[3 6 7]),3) )./rho;
v = ( sum(f(:,:,[2 5 6]),3) - sum(f(:,:,[4 7 8]),3) )./rho;
end
*/
		
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

/*
 count=count+1;
ers=0.;
for i =1:nx
for j=1:ny
ers=ers+u(i,j)*u(i,j)+v(i,j)*v(i,j);
end
end
error=abs(ers-erso);
erso=ers;
end
*/	
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
	FILE *fileu = fopen("/home/joao/Documents/PraticaLBM/u.txt", "w");
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(fileu, "%.2f ", u[i][j]);  // Salva cada valor com duas casas decimais
        }
        fprintf(fileu, "\n");
    }
    fclose(fileu);

    // Escreve a matriz 'v' no arquivo
    FILE *filev = fopen("/home/joao/Documents/PraticaLBM/v.txt", "w");
    for (int i = 0; i < nx; i++) {
        for (int j = 0; j < ny; j++) {
            fprintf(filev, "%.2f ", v[i][j]);
        }
        fprintf(filev, "\n");
    }

    // Fecha o arquivo
    fclose(filev);
    printf("Matrizes armazenadas no arquivo 'velocidades.txt' com sucesso!\n");
    
	
	
	
//conferir u e v, multiplicacoes em t1,t2 e t3
	
	return 0;
	}

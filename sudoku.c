#include "stdio.h"
#include "string.h"
 
#define ROW 3
#define COLUMN 3
#define RANK (ROW * COLUMN)
#define MAXR (RANK * RANK * RANK)
#define MAXC (4 * RANK * RANK) 
#define MAXN (4 * MAXR + MAXC + 1)   //4*MAXR+MAXC+1
 
int L[MAXN],R[MAXN],U[MAXN],D[MAXN],C[MAXN],G[MAXN],S[MAXC+1],O[RANK * RANK],H[MAXR+1];
int A[MAXR][MAXC],ans[RANK * RANK];
char sudoku[RANK * RANK];
 
void fill(int r,int c,int k)              //(r,c) of sudoku is filled with number k
{
	int grid=RANK*r+c, row=RANK*(RANK*r+c)+k;
	A[row][grid]=1;
	A[row][RANK*RANK+RANK*r+k]=1;
	A[row][2*RANK*RANK+RANK*c+k]=1;
	A[row][3*RANK*RANK+RANK*(ROW*(r/ROW)+c/COLUMN)+k]=1;
}
 
void construct()                          //construct matrix A
{
	int i,j,r,c;
	memset(A,0,sizeof(A));
	for(i=0;i<RANK*RANK;i++)
	{
		r=i/RANK;  c=i%RANK;
		if(sudoku[i]=='.')
		{
			for(j=0;j<RANK;j++)
				fill(r,c,j);
		}
		else fill(r,c,sudoku[i]-'1');
	}
}
 
void build_cross_list()                   //build the cross list of matrix A
{
	int i,j,count;
	for(i=1;i<=MAXC;i++)            
	{
		L[i]=i-1;   R[i]=i+1;
		U[i]=i;     D[i]=i;
		C[i]=i;    
	}
	L[0]=MAXC;   R[0]=1;
	R[MAXC]=0;
	
	memset(H,-1,sizeof(H));
	memset(S,0,sizeof(S));
	count=MAXC+1;
	
	for(i=1;i<=MAXR;i++)  
        for(j=1;j<=MAXC;j++)  
        {  
            if(!A[i-1][j-1])  continue;  
			
            if(H[i]==-1)                       
                H[i]=L[count]=R[count]=count;  
            else      
            {  
                L[count]=L[H[i]];    R[count]=H[i];  
                R[L[H[i]]]=count;    L[H[i]]=count;  
            }  
			
            U[count]=U[j];   D[count]=j;             
            D[U[j]]=count;   U[j]=count;  
            C[count]=j;      G[count]=i;      //record the row of the node                         
            S[j]++;                       
            count++;  
        }  
}
 
void re_move(int c)
{
	int i,j;
	L[R[c]]=L[c];                      
	R[L[c]]=R[c];
	for(i=D[c];i!=c;i=D[i])           
		for(j=R[i];j!=i;j=R[j])
		{
			U[D[j]]=U[j];
			D[U[j]]=D[j];
			S[C[j]]--;                
		}
}
 
void resume(int c)
{
	int i,j;
	for(i=U[c];i!=c;i=U[i])
		for(j=L[i];j!=i;j=L[j])
		{
			S[C[j]]++;
			U[D[j]]=j;
			D[U[j]]=j;
		}
		L[R[c]]=c;
		R[L[c]]=c;
}
 
int dfs(int depth)
{
	int i,j,c,min=10000;
	if(R[0]==0)                                    //print the answer
	{
		for(i=0;i<depth;i++)
			ans[(G[O[i]]-1)/RANK]=(G[O[i]]-1)%RANK+1;
		for(i=0;i<RANK*RANK;i++)
			printf("%d",ans[i]);
		printf("\n");
        for(i=0;i<RANK;i++) {
            for (j=0;j<RANK;j++) {
                printf("%d", ans[i*RANK+j]);
            }
            printf("\n");
        }
		return 1;   
	}
	
	for(i=R[0];i!=0;i=R[i])           
		if(S[i]<min)
		{
			min=S[i];
			c=i;
		}	
	re_move(c);
		
	for(i=D[c];i!=c;i=D[i])
	{
		O[depth]=i;                   
		for(j=R[i];j!=i;j=R[j])
			re_move(C[j]);
			
		if(dfs(depth+1))   return 1;
			
		for(j=L[i];j!=i;j=L[j])        
			resume(C[j]);
	}
		
	resume(c);
	return 0;
}
 
int main()
{
	while(scanf("%s",sudoku)&&sudoku[0]!='e')
	{
		construct();
		build_cross_list();
		if (dfs(0)) {
			printf("Find a solution\n");
		} else {
			printf("Unable to find a solution\n");
		}
	}
	return 0;
}


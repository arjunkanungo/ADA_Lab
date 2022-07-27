/*C program to find minimum cost spanning tree of a given undirected graph using Prim's algorithm*/
#include<stdio.h>
#include<stdlib.h>
void main()
{
    int min,no_of_edges=0,x,y,v,i,j,s=0;
    printf("\nEnter the number of vertices: ");
    scanf("%d",&v);
    int selected[v],G[v][v];
    for(i=0;i<v;i++)
        selected[i]=0;
    selected[0]=1;
    printf("\nEnter the cost adjacency matrix: ");
    for(i=0;i<v;i++)
        for(j=0;j<v;j++)
            scanf("%d",&G[i][j]);
    while (no_of_edges<v-1)
    {
        min=10000;
        x=0,y=0;
        for(i=0;i<v;i++)
            if(selected[i]==1)
            {
                for(j=0;j<v;j++)
                    if(selected[j]==0 && G[i][j]!=0)
                        if(min>G[i][j])
                        {
                            min=G[i][j];
                            x=i;
                            y=j;
                        }
            }
            printf("%d - %d : %d \n",x,y,G[x][y]);
            s=s+G[x][y];
            selected[y]=1;
            no_of_edges++;
    }
    printf("\nThe minimum cost is %d",s);
}
    

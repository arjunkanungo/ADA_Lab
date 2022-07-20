/*C progarm to find the Transitive closure of Directed graph using Warshall Algorithm*/
#include<stdio.h>
#include<stdlib.h>
void main()
{
    int v;
    printf("\nEnter the number of verties: ");
    scanf("%d",&v);
    int c[v][v],j,k,i;
    printf("\nEnter the adjacency matrix: ");
    for(i=0;i<v;i++)
        for(j=0;j<v;j++)
            scanf("%d",&c[i][j]);
    for(k=0;k<v;k++)
        for(i=0;i<v;i++)
            for(j=0;j<v;j++)
                if(c[i][j]||c[i][k]&&c[k][j])
                    c[i][j]=1;
    printf("\nTransitive Closure Matrix is\n");
    for(i=0;i<v;i++)
    {
        for(j=0;j<v;j++)
            printf("%d ",c[i][j]);
        printf("\n");
    }
}
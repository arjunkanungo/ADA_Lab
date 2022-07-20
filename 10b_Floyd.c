/*C program to find shortest path between two variables using Floyd's algorithm*/
#include<stdio.h>
#include<stdlib.h>
int Min(int a,int b)
{
    if(a<b)
        return a;
    return b;
}
void main()
{
    int v;
    printf("Enter the number of Vertices: ");
    scanf("%d",&v);
    int c[v][v],d[v][v],i,j,k;
    printf("\nEnter the Cost Matrix:\n ");
    for(i=0;i<v;i++)
    {
        for(j=0;j<v;j++)
        {
            scanf("%d",&c[i][j]);
            d[i][j]=c[i][j];
        }
    }
    for(k=0;k<v;k++)
    {
        for(i=0;i<v;i++)
        {
            for(j=0;j<v;j++)
            {
                d[i][j]=Min(d[i][j],d[i][k]+d[k][j]);
            }
        }
    }
    printf("\nThe Shortest distance Matrix:\n");
    for(i=0;i<v;i++)
    {
        for(j=0;j<v;j++)
            printf("%d\t",d[i][j]);
        printf("\n");
    }
}
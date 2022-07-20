#include<stdio.h>
#include<stdlib.h>
int max( int a,int b)
{
    if(a>b)
        return a;
    else
     return b;
}
int main()
{
    int n,m,i,j,w;
    printf("\nEnter the number of items: ");
    scanf("%d",&n);
    printf("\nEnter the maximum size of the bag: ");
    scanf("%d",&m);
    int p[n+1],wt[n+1],k[n+1][m+1];
    p[0]=0;wt[0]=0;
    for(i=1;i<=n;i++)
    {
        printf("Enter the weights and their corresponding Value of item %d: ",i);
        scanf("%d %d",&wt[i],&p[i]);
    }
    for(i=0;i<=n;i++)
         for(w=0;w<=m;w++)
        {
             if(i==0 || w==0)
                k[i][w]=0;
            else if(wt[i]<=w)
                k[i][w]=max(k[i-1][w],k[i-1][w-wt[i]]+p[i]);
            else
                 k[i][w]=k[i-1][w];
        }
    for(i=0;i<=n;i++)
    {
        for(j=0;j<=m;j++)
            printf("%d\t",k[i][j]);
        printf("\n");
    }
    printf("\nThe items that need to be picked are: ");
    i=n;j=m;
    while(i>0 && j>0)
    {
        if(k[i][j]==k[i-1][j])
        i--;
        else
        {
            printf("%d",i);
            j=j-p[i];
            i--;
        }
    }
    return 0;
}
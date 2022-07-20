/*C program to implement recursive binary search algorithm*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
int count=0;
void bin(int *a,int s,int e,int key)
{
    if(s<=e){
        count++;
        int mid=(s+e)/2;
        if(a[mid]==key)
            return;
        else if(a[mid]>key)
            bin(a,mid+1,e,key);
        else
            bin(a,s,mid-1,key);
    }
}
void main()
{
    FILE *f=NULL;
    srand(time(NULL));
    int i,j,mid,s,e,k,key;
    for(k=1;k<=3;k++)
    {
        switch(k)
        {
            case 1:f=fopen("binbest.txt","w");break;
            case 2:f=fopen("binworst.txt","w");break;
            case 3:f=fopen("binavg.txt","w");break;
        }
        for(j=0;j<100;j++)
        {
            int a[j];
            for(i=0;i<j;i++)
                a[j]=i;
            e=j-1;
            s=0;
            mid=(s+e)/2;
            switch (k)
            {
                case 1:key=a[mid];break;
                case 2:key=1000;break;
                case 3:key=a[mid/2];
            }
            bin(a,0,j-1,key);
            fprintf(f,"%d\t%d\n",j,count);
            count=0;
        }
        fclose(f);
    }
}
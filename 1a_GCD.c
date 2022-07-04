/*C Program to find GCD of two numbers using Euclid's algorthm, Modified Euclid's algorithm and Consecutive Integer Checking method technique*/

#include<stdio.h>
#include<stdlib.h>
#include<time.h>
void main()
{
    int a,b,c,m,n,rem,gcd,min,temp1,temp2,count=0;
    srand(time(0));
    a=rand()%100;
    b=rand()%100;
    //To find GCD of two numbers using Euclid's algorithm
    m=a;
    n=b;
    while (n!=0)
    {
        rem=m%n;
        m=n;
        n=rem;
        count++;
    }
    gcd=m;
    printf("The GCD of %d and %d found using Euclid's algorithm is %d",a,b,gcd);
    printf("\nCount: %d",count);
    //To Find GCD of two numbers using Consecutive Integer checking method
    count=0;
    if(a==0 || b==0)
    {
        printf("\nThe consecutive integer checking method does not work for integers with 0");
    }
    else
    {
        m=a;
        n=b;
        if(m>n)
        {
            min=n;
        }
        else
        {
            min=m;
        }
        while (min!=1)
        {
            count++;
            temp1=m%min;
            if(temp1==0)
            {
                temp2=n%min;
                if(temp2==0)
                {
                    gcd=min;
                    break;
                }
            }
            min--;
            gcd=min;
        }
        printf("\nThe GCD of  %d and %d using Consecutive integer checking method is %d",a,b,gcd); 
        printf("\nCount:\t%d",count);
    }
    //To find GCD of using Modified Euclid's algorithm 
    count=0;
    m=a;
    n=b;
    if (m==0 || n==0)
    {
        printf("\nWe cannot find the GCD of two numbers when either of the given two integers is 0");
    }
    else
    {
        while (m!=n)
        {
            count++;
            if(m>n)
                m=m-n;
            else
                n=n-m;
        }
        printf("\nThe GCD of %d and %d using Modified Euclid's Algorithm is %d",a,b,gcd);
        printf("\nCount: %d",count);
    }
}
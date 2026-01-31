#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#define BUFFSIZE 10

float sum(float a, float b, float c) 
{
    return a + b + c;
}
int fibboanci(int x)
{

    if(x == 0)
        return 0;
    if(x == 1)
        return 1;
    return fibboanci(x - 2) + fibboanci(x - 1);
}
bool isPrime(int n)
{
    if (n == 1 || n == 0)
        return false;

    for (int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

void problema1()
{
    char s1[BUFFSIZE];
    char s2[BUFFSIZE];
    char s3[BUFFSIZE];

    printf("Introduce 3 numere reale : ");
    scanf("%s %s %s",s1,s2,s3);

    float f1 = atof(s1);
    float f2 = atof(s2);
    float f3 = atof(s3);

    float result = sum(f1,f2,f3);
    printf("\nSuma numerelor : %.1f\n", result);
}
void problema2()
{
    int x;
    printf("Introduce numarul pentru a afla primele n valori fibboanci : ");
    scanf("%d", &x);

    int t1 = 0;
    int t2 = 1;
    int next = t1 + t2;

    int i = 0;
    while(i < x)
    {
        printf("%d ", next);
        t1 = t2;
        t2 = next;
        next = t1 + t2;
        i++;
    }
}
void problema3()
{
    int x;
    printf("Introduce numarul pentru a afisa primele x valori din fibboanci : ");
    scanf("%d", &x);

    for(int i = 0; i < x; i++)
    {
        printf("%d ", fibboanci(i));
    }
    printf("\n");
}
void problema5()
{
    int x;
    printf("Introduce un numar pentru a afla primele n numere prime : ");
    scanf("%d", &x);

    for(int i = 0; i < x; i++)
    {
        if(isPrime(i))
            printf("%d ", i);
    }
    printf("\n");
}
int main(void)
{
    problema5();
    return 0;
}

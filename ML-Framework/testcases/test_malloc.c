#include<stdlib.h>
#include<stdint.h>
#include<stdio.h>
int main()
{
char *ptr;
uint64_t s;
int mb=2048;
s=1024*1024*mb;
while(1)
{
ptr=(char *)malloc(s);
if(ptr)
{
printf("Allocated : %d MB\n",mb);
free(ptr);
mb++;
s=1024*1024*mb;
}
else
{
printf("Unable to allocated : %d MB\n",mb);
break;
}
}
return 0;
}

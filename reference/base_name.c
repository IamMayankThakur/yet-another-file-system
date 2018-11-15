#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/time.h>
#include <libgen.h>

int main(int argc, char const *argv[])
{
    char i[100]="abc";
    strcpy(i,"/usr/lib/apache2");
    char * a = basename(i);
    char * b = dirname(i);
    // strcpy(i,)
    char * p ;
    // printf("%s\n%s\n",a,b);
    printf("%s\n%p",p,&p);
    printf("%s\n%p",i,&i);
    // printf("$$$%d",sizeof(p));
    // printf("!!!!%d",sizeof(*p));
    return 0;
}

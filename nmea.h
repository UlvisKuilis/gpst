#include <stdio.h>
#include <string.h>
 
int checksum(char *s) {
    int c = 0;
 
    while(*s)
        c ^= *s++;
 
    return c;
}
 
int main()
{
    char mystring[] = "GPRMC,092751.000,A,5321.6802,N,00630.3371,W,0.06,31.66,280511,,,A";
 
    printf("String: %s\nChecksum: 0x%02X\n", mystring, checksum(mystring));
 
    return 0;
}

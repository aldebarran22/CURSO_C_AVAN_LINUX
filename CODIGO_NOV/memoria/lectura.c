#include <stdio.h>


int main(){
    char s[10];

    puts("Teclear una cadena: ");
    //gets(s);
    fgets(s, 10, stdin);
    puts(s);

    return 0;
}
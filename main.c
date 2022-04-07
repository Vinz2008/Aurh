#include <stdio.h>
#include <stdlib.h>
#include "libs/color.h"


int InstallAurPackage(){
    system("ls");
    return 0;
}
int main(int argc, char* argv[]){
    printf("%c", argc);
    if (argc == 0){
        printf(BRED "ERROR: No Extra Command Line Argument Passed Other than the Program Name\n" reset);
    }
    InstallAurPackage();
    return 0;
}

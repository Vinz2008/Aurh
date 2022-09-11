#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <curl/curl.h>
#include "libs/color.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

CURL *curl;
CURLcode res;
struct MemoryStruct chunk;

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(!ptr) {
    /* out of memory! */
    printf("not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}

void updatePacman(){
    system("sudo pacman -Syu");
} 


void cleanCacheFolder(){
    system("rm -rf ~/.cache/aurh");
}



void installAurPackage(char repoName[20]){
    char command[100];
    char answer;
    char temp[20];
    printf(BBLU "--> " reset);
    printf(BWHT"Do you want to see the PKGBUILD ? [Y/N] : " reset);
    scanf("%c", &answer);
    switch (answer)
    {
    case 'Y':
        chunk.memory = malloc(1);
        chunk.size = 0;
        curl = curl_easy_init();
        if (curl){
            char* url = malloc(120);
            char* header;
            strcpy(url, "https://aur.archlinux.org/cgit/aur.git/plain/PKGBUILD?h=");
            strcat(url, repoName);
            printf("url : %s\n", url);
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
            curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
            printf(BRED "curl request failed: %s\n",
            curl_easy_strerror(res));
            exit(0);
            }
            printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
            printf("%s\n ", chunk.memory);
            printf(" ");
            printf("Do you want to coninue ? [Y/N] : ");
            scanf("%s", &temp);
            if (strcmp(temp,"Y") != 0){
                exit(0);
            }
            curl_easy_cleanup(curl);
            free(chunk.memory);
            free(url);

        }
        else {
            printf(BRED "ERROR: curl initialization error\n" reset);
            exit(0);
        }
        curl_global_cleanup();
        break;
    case 'N':
        break;
    default:
        exit(0);
        break;
    }
    system("if [ ! -d ~/.cache/aurh ]; then mkdir -p ~/.cache/aurh; fi");
    strcpy(command, "git clone https://aur.archlinux.org/");
    strcat(command, repoName);
    strcat(command, " ~/.cache/aurh/");
    strcat(command,repoName);
    printf("command : %s\n" , command);
    system(command); //CLONE REPO
    strcpy(command, "cd ~/.cache/aurh/");
    strcat(command, repoName);
    strcat(command, "/ && ");
    strcat(command, "makepkg -si");
    printf("command : %s\n" , command);
    system(command); //CD REPO and BUILD PACKAGE
}

void installPacmanPackage(char reponame[20]){
    char* command = malloc(100 * sizeof(char));
    strcpy(command, "sudo pacman -S ");
    strcat(command, reponame);
    system(command);
    free(command);
}

int testPacmanPackageExist(char repoName[20]){
    int returnedNumber = 0;
    chunk.memory = malloc(1);
    chunk.size = 0;
    curl = curl_easy_init();
    if (curl){
        char url[120];
        char* header;
        strcpy(url, "https://archlinux.org/packages/community/x86_64/");
        strcat(url, repoName);
        strcat(url, "/");
        printf("url : %s\n", url);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
        printf(BRED "curl request failed: %s\n",
        curl_easy_strerror(res));
        exit(0);
        }
        long http_code;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        printf("http_code : %ld\n", http_code);
        if (http_code == 200){
            returnedNumber = 1;
            printf("Is a pacman package\n");
        } else {
            printf("is not a pacman package\n");
        }

        curl_easy_cleanup(curl);
        free(chunk.memory);
    }
    return returnedNumber;
}

void installAurOrPacmanPackage(char repoName[20]){
    updatePacman();
    if (testPacmanPackageExist(repoName) == 1){
        installPacmanPackage(repoName);
    } else {
        installAurPackage(repoName);
    }
}

void removePackage(char* program){
    char command[40];
    snprintf(command, 40, "sudo pacman -R %s", program);
    printf("command : %s\n", command);
    system(command);
}

int main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_ALL);
    int i;
    char repoName[20];
    char program[20];
    int installmode = 0;
    int removemode = 0;
    int cleanmode = 0;
    if(argc==1){
        printf(BRED "ERROR: No Extra Command Line Argument Passed Other Than Program Name\n" reset);
        //printf(GRN "Do --help to know how to use this cli application\n" reset);
        exit(0);
    }
    for (i = 1; i < argc; i++ ){
        if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0){
        printf(BLU "USAGE : work in progress, it will be done later\n" reset);
        //usage();
        exit(0);
        }
        else if (strcmp(argv[i], "-S") ==  0 || strcmp(argv[i], "install") ==  0){
            strcpy(repoName, argv[i + 1]); 
            installmode = 1;
        }
        else if (strcmp(argv[i], "-R") == 0 || strcmp(argv[i], "remove") == 0){
	    strcpy(program, argv[i + 1]);
	    removemode = 1;
	}
        else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "clean") == 0){
        cleanmode = 1;
        }
        
    }
    if (installmode == 1){
        installAurOrPacmanPackage(repoName);
    } else if (removemode == 1){
        removePackage(program);
    } else if (cleanmode == 1) {
        cleanCacheFolder();
    }
    return 0;
}

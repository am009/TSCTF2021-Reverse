
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "myaesni.h"

// make.sh
// #!/bin/bash
// gcc -std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX -maes -O1 -flto -Ilua-5.4.3/src   -c -o mylua.o main.c 
// gcc -std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX -maes -O1 -flto -Ilua-5.4.3/src   -c -o myaesni.o myaesni.c -Wno-missing-braces
// gcc -std=gnu99 -flto -o lua.elf   mylua.o myaesni.o liblua.a -lm -ldl

// link time optimization `-flto`

char* arr[50];
int arr_ind = 0;

char* process_code(char* code_path) {
    char* buf = malloc(0x1000);
    buf[0] = '\x00';
    FILE* fp = fopen(code_path, "r");
    if (fp == NULL) {
        printf("open %s failed.\n", code_path);
        exit(-1);
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    for(int i=1;i<=50 && (read = getline(&line, &len, fp)) != -1;i++) {
        if (i == 17) {
            free(line);
            line = NULL;
            len = 0;
            continue;
        }
        if (i % 2 == 1) {
            if (line != NULL) {
                strcat(buf, line);
                free(line);
                line = NULL;
                len = 0;
            }
        } else {
            arr[arr_ind] = line;
            arr_ind ++;
            line = NULL;
            len = 0;
        }
    }
    for(int i=0;i<arr_ind;i++) {
        if (arr[i] != NULL) {
            strcat(buf, arr[i]);
            free(arr[i]);
            arr[i] = NULL;
        }
    }
    return buf;

}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s lua.elf <path to checker.lua>\n", argv[0]);
        exit(-1);
    }
    char* code_path = argv[1];
    char* code = process_code(code_path);
    // printf("%s", code);
    // exit(0);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    // TODO setup funcs
    lua_pushcfunction(L, lua_enc);
    lua_setglobal(L, "enc");

    int error = luaL_loadstring(L, code) || lua_pcall(L, 0, 0, 0);
    if (error) {
        const char * error_str = lua_tostring(L, -1);
        puts("script error:");
        puts(error_str);
        lua_pop(L, 1);  /* pop error message from the stack */
        exit(-1);
    }
    lua_close(L);
    return 0;
}
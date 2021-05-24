#!/bin/bash
gcc -std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX -maes -O1 -Ilua-5.4.3/src   -c -o mylua.o main.c 
gcc -std=gnu99 -O2 -Wall -Wextra -DLUA_COMPAT_5_3 -DLUA_USE_LINUX -maes -O1 -Ilua-5.4.3/src   -c -o myaesni.o myaesni.c -Wno-missing-braces
gcc -std=gnu99 -o lua.elf   mylua.o myaesni.o liblua.a -lm -ldl
strip ./lua.elf -N main -N aes_enc -N lua_enc -N process_code -N plain -N computed -N arr -N arr_ind -N subkeys 
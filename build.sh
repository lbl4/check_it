#!/bin/bash


clang -fsanitize=memory -fno-omit-frame-pointer -g -O2 main.c -o check_it 


#clang -O2 main.c elf_check.c -o check_it 

if [ $1 == "stack" ];
then
	clang -z execstack test.c -o test
elif [ $1 == "pie" ];
then
	clang -no-pie test.c -o test
elif [ $1 == "nor" ];
then
	clang -Wall -Werror -Wl,-z,norelro test.c -o test
elif [ $1 == "fullr" ];
then
	clang -Wl,-z,relro,-z,now test.c -o test
elif [ $1 == "all" ];
then
	clang -fstack-protector-all -Wl,-z,relro,-z,now test.c -o test 
else
	clang test.c -o test
fi

./check_it test

pwn checksec test



#__stack_chk_fail

# clang-format
# _guard_check_icall 
#clang -fstack-protector-all -Wl,-z,relro,-z,now  -fvisibility=hidden -flto -fsanitize=cfi  test.c -o test


#include "elf_check.h"


// TODO:

    // Stack canaries
    // RPATH
    // RUNPATH
    // SYMBOLS
    // FORTIFY

int main(int argc, char **argv) {

    if (argc < 2 || (elf = fopen(argv[1], "rb")) == NULL) {
        fputs("./check_it <elf_files>\n", stderr); 
        exit(-1);
    }

    fread(&header, sizeof(header), 1, elf);

    if(memcmp(header.e_ident, ELFMAG, SELFMAG) != 0) {
        fputs("file isn't an elf\n",stderr);
        fclose(elf);
        exit(-1);
    }


    done();

    fclose(elf);
}
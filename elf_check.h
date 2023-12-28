#ifndef ELF_CHECK_H
#define ELF_CHECK_H

#include <elf.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define bool _Bool


Elf64_Ehdr header;
Elf64_Phdr phdr;
FILE *elf;
Elf64_Dyn dyn;


bool full_relro(Elf64_Phdr phdr, FILE *elf) {

    if (phdr.p_type == PT_DYNAMIC) {
        size_t dyn_entry_size = sizeof(Elf64_Dyn);
        int n = phdr.p_memsz / dyn_entry_size;

        fseek(elf, phdr.p_offset, SEEK_SET);

        for (int i = 0; i < n; i++) {

            fread(&dyn, dyn_entry_size, 1, elf);

            if (dyn.d_tag ==  DT_FLAGS && (dyn.d_un.d_val & DT_BIND_NOW) ) {
                return 1;
            }
        }
    }

    return 0;
}


bool relro(Elf64_Phdr phdr) {

    return (phdr.p_type == PT_GNU_RELRO);
}

bool pie(Elf64_Ehdr header) {
    return (header.e_type == ET_DYN);
}

bool nx(Elf64_Phdr phdr) {
    return (phdr.p_type == PT_GNU_STACK && (phdr.p_flags == (PF_W | PF_R)));
}

void done() {

    char *nx_ = "Disabled", *pie_ = "Disabled", *canary_ = "Disabled";

    int relro_ = 0;
    int full_relro_ = 0;

    for (int i = 0; i < header.e_phnum; i++) {

        fseek(elf, header.e_phoff + i * header.e_phentsize, SEEK_SET);
        fread(&phdr, sizeof(phdr), 1, elf);

        if(nx(phdr)) {
            nx_ = "Enabled";
        } if(pie(header)) {
            pie_ = "Enabled";
        } 
        
        if(relro(phdr)) {relro_ = 1;}
        if(full_relro(phdr,elf)) {full_relro_ = 2;}
    }

    char *relrox = (full_relro_ == 2 ? "Full" : relro_ == 1 ? "Partial" : "No Relro");
    printf("NX: %s\t PIE: %s\t RELRO: %s\t CANARY: %s\n", nx_, pie_, relrox, canary_);

}

#endif

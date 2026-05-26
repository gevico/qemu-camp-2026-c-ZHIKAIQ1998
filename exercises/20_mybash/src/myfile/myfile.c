#include "myfile.h"
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <string.h>

void print_elf_type(uint16_t e_type) {
    const char *type_str;
    switch (e_type) {
    case ET_NONE:
      type_str = "Unknown (ET_NONE)";
      break;
    case ET_REL:
      type_str = "Relocatable (ET_REL)";
      break;
    case ET_EXEC:
      type_str = "Executable (ET_EXEC)";
      break;
    case ET_DYN:
      type_str = "Shared Object/PIE (ET_DYN)";
      break;
    case ET_CORE:
      type_str = "Core Dump (ET_CORE)";
      break;
    default:
      if (e_type >= ET_LOOS && e_type <= ET_HIOS)
        type_str = "OS-Specific";
      else if (e_type >= ET_LOPROC && e_type <= ET_HIPROC)
        type_str = "Processor-Specific";
      else
        type_str = "Invalid";
    }
    printf("ELF Type: %s (0x%x)\n", type_str, e_type);
}

int __cmd_myfile(const char* filename) {
    char filepath[256];
    int fd;
    Elf64_Ehdr ehdr;

    strcpy(filepath, filename);
    fflush(stdout);
    printf("filepath: %s\n", filepath);

    // 打开文件
    fd = open(filepath, O_RDONLY);
    if (fd < 0) {
        perror("无法打开文件");
        return -1;
    }

    // 读取 ELF 文件头
    ssize_t bytes_read = read(fd, &ehdr, sizeof(Elf64_Ehdr));
    if (bytes_read != sizeof(Elf64_Ehdr)) {
        fprintf(stderr, "读取 ELF 文件头失败\n");
        close(fd);
        return -1;
    }

    // 检查 ELF 魔数
    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3) {
        fprintf(stderr, "不是有效的 ELF 文件\n");
        close(fd);
        return -1;
    }

    print_elf_type(ehdr.e_type);
    close(fd);
    return 0;
}
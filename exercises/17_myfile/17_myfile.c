#include <elf.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

void analyze_elf(const char *filepath) {
  int fd;
  Elf64_Ehdr ehdr;
  Elf64_Phdr *phdr = NULL;

  // 打开文件
  fd = open(filepath, O_RDONLY);
  if (fd < 0) {
    perror("open");
    return;
  }

  // 读取 ELF 文件头
  ssize_t bytes_read = read(fd, &ehdr, sizeof(ehdr));
  if (bytes_read != sizeof(ehdr)) {
    fprintf(stderr, "读取 ELF 头失败\n");
    close(fd);
    return;
  }

  // 验证 ELF 魔数
  if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
      ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
      ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
      ehdr.e_ident[EI_MAG3] != ELFMAG3) {
    fprintf(stderr, "%s: 不是有效的 ELF 文件\n", filepath);
    close(fd);
    return;
  }

  printf("\nFile: %s\n", filepath);
  
  // 输出 ELF 类型
  print_elf_type(ehdr.e_type);
  
  // 输出入口地址
  printf("Entry: 0x%lx\n", (unsigned long)ehdr.e_entry);
  
  // 读取程序头表
  if (ehdr.e_phnum > 0 && ehdr.e_phentsize == sizeof(Elf64_Phdr)) {
    phdr = (Elf64_Phdr *)malloc(ehdr.e_phnum * sizeof(Elf64_Phdr));
    if (!phdr) {
      fprintf(stderr, "内存分配失败\n");
      close(fd);
      return;
    }

    // 定位到程序头表位置并读取
    if (lseek(fd, ehdr.e_phoff, SEEK_SET) == -1) {
      perror("lseek");
      free(phdr);
      close(fd);
      return;
    }

    bytes_read = read(fd, phdr, ehdr.e_phnum * sizeof(Elf64_Phdr));
    if (bytes_read != (ssize_t)(ehdr.e_phnum * sizeof(Elf64_Phdr))) {
      fprintf(stderr, "读取程序头表失败\n");
      free(phdr);
      close(fd);
      return;
    }

    // 输出加载段（PT_LOAD）的内存范围
    printf("Load segments: ");
    int first = 1;
    for (int i = 0; i < ehdr.e_phnum; i++) {
      if (phdr[i].p_type == PT_LOAD) {
        if (!first) {
          printf(", ");
        }
        printf("0x%lx-0x%lx", 
               (unsigned long)phdr[i].p_vaddr,
               (unsigned long)(phdr[i].p_vaddr + phdr[i].p_memsz));
        first = 0;
      }
    }
    printf("\n");

    free(phdr);
  }

  close(fd);
}

int main(int argc, char *argv[]) {
  char filepath[2][256] = {
    "./17_myfile.o",
    "./17_myfile",
  };

  for (int i = 0; i < 2; i++) {
    analyze_elf(filepath[i]);
  }
  
  return 0;
}

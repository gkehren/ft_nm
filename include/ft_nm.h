#ifndef FT_NM_H
# define FT_NM_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <elf.h>
# include <stdio.h>

# include "../libft/libft.h"

typedef struct s_symbol
{
	Elf64_Sym	*sym;
	char		type;
}	t_symbol;

// parse.c
int	parse_elf_file(void *filedata);
char	*ulong_to_hex_str(unsigned long num, int addr_size);

int	read_symbols_elf64(Elf64_Ehdr *header, void *filedata);
int	read_symbols_elf32(Elf32_Ehdr *header, void *filedata);

#endif

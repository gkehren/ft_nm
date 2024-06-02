/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:01:28 by gkehren           #+#    #+#             */
/*   Updated: 2024/06/02 15:42:35 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_nm.h"

char	get_symbol_type(Elf64_Sym sym, Elf64_Shdr *shdr) {
	char type = '?';

	if (ELF64_ST_BIND(sym.st_info) == STB_GNU_UNIQUE) return 'u';
	if (ELF64_ST_BIND(sym.st_info) == STB_WEAK) {
		if (sym.st_shndx == SHN_UNDEF) return 'w';
		return 'W';
	}
	if (ELF64_ST_BIND(sym.st_info) == STB_WEAK && ELF64_ST_TYPE(sym.st_info) == STT_OBJECT) {
		if (sym.st_shndx == SHN_UNDEF) return 'v';
		return 'V';
	}
	if (sym.st_shndx == SHN_UNDEF) return 'U';
	if (sym.st_shndx == SHN_ABS) return 'A';
	if (sym.st_shndx == SHN_COMMON) return 'C';
	if (shdr[sym.st_shndx].sh_type == SHT_NOBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) type = 'b';
	if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == SHF_ALLOC) type = 'r';
	if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_WRITE)) type = 'd';
	if (shdr[sym.st_shndx].sh_type == SHT_PROGBITS && shdr[sym.st_shndx].sh_flags == (SHF_ALLOC | SHF_EXECINSTR)) type = 't';
	if (shdr[sym.st_shndx].sh_type == SHT_DYNAMIC) type = 'd';
	if (shdr[sym.st_shndx].sh_type == SHT_FINI_ARRAY) type = 'd';
	if (shdr[sym.st_shndx].sh_type == SHT_INIT_ARRAY) type = 'd';

	// Convert to uppercase if the symbol is global
	if (ELF64_ST_BIND(sym.st_info) == STB_GLOBAL) {
		type = (char)ft_toupper(type);
	}

	return type;
}

void	read_symbols_elf64(Elf64_Ehdr *header, void *filedata)
{
	Elf64_Shdr	*sections = (Elf64_Shdr *)((char *)filedata + header->e_shoff);
	const char	*strtab = NULL;
	Elf64_Sym	*symtab = NULL;
	int			symcount = 0;

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			symtab = (Elf64_Sym *)((char *)filedata + sections[i].sh_offset);
			symcount = sections[i].sh_size / sizeof(Elf64_Sym);
		}
		else if (sections[i].sh_type == SHT_STRTAB && ft_strcmp((char *)filedata + sections[header->e_shstrndx].sh_offset + sections[i].sh_name, ".strtab") == 0)
		{
			strtab = (const char *)filedata + sections[i].sh_offset;
		}
	}

	if (!symtab || !strtab)
	{
		printf("ft_nm: No symbols\n");
		exit(EXIT_FAILURE);
	}

	// TODO: Sort symbols by address

	for (int i = 0; i < symcount; i++) {
		if (symtab[i].st_name == 0) continue;
		char type = get_symbol_type(symtab[i], sections);
		if (type == 'U' || type == 'w') {
			printf("                 %c %s\n", type, strtab + symtab[i].st_name);
		} else {
			printf("%016lx %c %s\n", symtab[i].st_value, type, strtab + symtab[i].st_name);
		}
	}
}

void	read_symbols_elf32(Elf32_Ehdr *header, void *filedata)
{
	Elf32_Shdr	*sections = (Elf32_Shdr *)((char *)filedata + header->e_shoff);
	const char	*strtab = NULL;
	Elf32_Sym	*symtab = NULL;
	int			symcount = 0;

	for (int i = 0; i < header->e_shnum; i++)
	{
		if (sections[i].sh_type == SHT_SYMTAB)
		{
			symtab = (Elf32_Sym *)((char *)filedata + sections[i].sh_offset);
			symcount = sections[i].sh_size / sizeof(Elf32_Sym);
		}
		else if (sections[i].sh_type == SHT_STRTAB && ft_strcmp((char *)filedata + sections[header->e_shstrndx].sh_offset + sections[i].sh_name, ".strtab") == 0)
		{
			strtab = (const char *)filedata + sections[i].sh_offset;
		}
	}

	if (!symtab || !strtab)
	{
		printf("ft_nm: No symbols\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < symcount; i++)
	{
		printf("%08x %c %s\n", symtab[i].st_value, (ELF32_ST_BIND(symtab[i].st_info) == STB_GLOBAL) ? 'T' : 't', strtab + symtab[i].st_name);
	}
}

void	parse_elf_file(void *filedata)
{
	Elf64_Ehdr	*header;

	header = (Elf64_Ehdr *)filedata;
	// Check if the file is an ELF file
	if (header->e_ident[EI_MAG0] != ELFMAG0
		|| header->e_ident[EI_MAG1] != ELFMAG1
		|| header->e_ident[EI_MAG2] != ELFMAG2
		|| header->e_ident[EI_MAG3] != ELFMAG3)
	{
		printf("Not an ELF file\n");
		exit(EXIT_FAILURE);
	}
	// Check if the file is a 64 or 32-bit ELF file
	if (header->e_ident[EI_CLASS] == ELFCLASS32)
	{
		read_symbols_elf32((Elf32_Ehdr *)header, filedata);
	}
	else if (header->e_ident[EI_CLASS] == ELFCLASS64)
	{
		read_symbols_elf64((Elf64_Ehdr *)header, filedata);
	}
	else
	{
		printf("Invalid ELF class\n");
		exit(EXIT_FAILURE);
	}
}

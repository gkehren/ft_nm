/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   elf64.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/03 00:21:26 by gkehren           #+#    #+#             */
/*   Updated: 2024/06/03 00:28:51 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_nm.h"

static char	get_symbol_type(Elf64_Sym sym, Elf64_Shdr *shdr) {
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
	if (ELF64_ST_BIND(sym.st_info) == STB_GLOBAL)
		type = (char)ft_toupper(type);

	return type;
}

static void	sort_symbols(Elf64_Sym **symtab, int symcount, const char *strtab)
{
	for (int i = 1; i < symcount; i++)
	{
		Elf64_Sym *key = symtab[i];
		const char *key_name = strtab + key->st_name;
		int j = i - 1;

		while (j >= 0 && ft_strcmp(strtab + symtab[j]->st_name, key_name) > 0)
		{
			symtab[j + 1] = symtab[j];
			j--;
		}
		symtab[j + 1] = key;
	}
}

int	read_symbols_elf64(Elf64_Ehdr *header, void *filedata)
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
			strtab = (const char *)filedata + sections[i].sh_offset;
	}

	if (!symtab || !strtab)
	{
		printf("ft_nm: No symbols\n");
		return (EXIT_FAILURE);
	}

	Elf64_Sym **symbols_to_display = (Elf64_Sym **)malloc(sizeof(Elf64_Sym *) * symcount);
	int display_count = 0;

	for (int i = 0; i < symcount; i++)
	{
		if (symtab[i].st_name == 0) continue;
		char type = get_symbol_type(symtab[i], sections);
		if (type != 'A')
			symbols_to_display[display_count++] = &symtab[i];
	}

	sort_symbols(symbols_to_display, display_count, strtab);

	for (int i = 0; i < display_count; i++)
	{
		char type = get_symbol_type(*symbols_to_display[i], sections);
		if (type == 'U' || type == 'w')
			printf("                 %c %s\n", type, strtab + symbols_to_display[i]->st_name);
		else
			printf("%016lx %c %s\n", symbols_to_display[i]->st_value, type, strtab + symbols_to_display[i]->st_name);
	}

	free(symbols_to_display);
	return (EXIT_SUCCESS);
}

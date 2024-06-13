/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:01:28 by gkehren           #+#    #+#             */
/*   Updated: 2024/06/03 00:23:10 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_nm.h"

int	parse_elf_file(void *filedata)
{
	Elf64_Ehdr	*header;

	header = (Elf64_Ehdr *)filedata;
	// Check if the file is an ELF file
	if (header->e_ident[EI_MAG0] != ELFMAG0
		|| header->e_ident[EI_MAG1] != ELFMAG1
		|| header->e_ident[EI_MAG2] != ELFMAG2
		|| header->e_ident[EI_MAG3] != ELFMAG3)
	{
		ft_printf("Not an ELF file\n");
		return (EXIT_FAILURE);
	}
	// Check if the file is a 64 or 32-bit ELF file
	if (header->e_ident[EI_CLASS] == ELFCLASS32)
	{
		if (read_symbols_elf32((Elf32_Ehdr *)header, filedata) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else if (header->e_ident[EI_CLASS] == ELFCLASS64)
	{
		if (read_symbols_elf64((Elf64_Ehdr *)header, filedata) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	else
	{
		ft_printf("Invalid ELF class\n");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

char	*ulong_to_hex_str(unsigned long num, int addr_size)
{
	static char	hex_str[17];
	const char*	hex_digis = "0123456789abcdef";
	hex_str[16] = '\0';
	for (int i = (addr_size == 32) ? 7 : 15; i >= 0; i--)
	{
		hex_str[i] = hex_digis[num & 0xF];
		num >>= 4;
	}
	return (hex_str);
}

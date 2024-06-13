/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:00:44 by gkehren           #+#    #+#             */
/*   Updated: 2024/06/03 00:22:39 by gkehren          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_NM_H
# define FT_NM_H

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <elf.h>

// TODO: remove
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

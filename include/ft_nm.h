/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_nm.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkehren <gkehren@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 14:00:44 by gkehren           #+#    #+#             */
/*   Updated: 2024/06/02 15:52:24 by gkehren          ###   ########.fr       */
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

#endif

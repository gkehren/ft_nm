#include "../include/ft_nm.h"

void	handle_error(const char *msg, int fd)
{
	perror(msg);
	if (fd != -1)
		close(fd);
	exit(EXIT_FAILURE);
}

void	*open_file(const char *filename, size_t *filesize)
{
	int			fd;
	struct stat	sb;
	void		*filedata;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		handle_error("open", -1);
	if (fstat(fd, &sb) == -1)
		handle_error("fstat", fd);
	*filesize = sb.st_size;
	filedata = mmap(NULL, *filesize, PROT_READ, MAP_PRIVATE, fd, 0);
	if (filedata == MAP_FAILED)
		handle_error("mmap", fd);
	close(fd);
	return (filedata);
}

int	main(int argc, char **argv)
{
	void	*filedata;
	size_t	filesize;

	if (argc != 2)
	{
		ft_printf("Usage: %s <filename>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	filedata = open_file(argv[1], &filesize);
	if (parse_elf_file(filedata) == EXIT_FAILURE)
	{
		if (munmap(filedata, filesize) == -1)
		{
			perror("munmap");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_FAILURE);
	}

	if (munmap(filedata, filesize) == -1)
	{
		perror("munmap");
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

#include "../inc/minishell.h"

int	ft_redir_heredoc(t_cmd *cmd)
{
	int		pipefd[2];
	char	*buffer;
	if (pipe(pipefd) == -1)
	{
		perror("Error pipe\n");
		return (1);
	}
	while (1)
	{
		buffer = readline("> ");
		if (!buffer || !ft_strcmp(buffer, cmd->infile))
		{
			free(buffer);
			break;
		}
		write(pipefd[1], buffer, ft_strlen(buffer));
		write(pipefd[1], "\n", 1);
		free(buffer);
	}
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("Error heredoc\n");
		return (1);
	}
	close(pipefd[0]);
	return (0);
}

int ft_redir_infile(char *infile)
{
    int fd;
    fd = open(infile, O_RDONLY);
    if (fd == -1)
    {
        perror("Error reading infile\n");
        return (1);
    }
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("Error duplicating input file\n");
        close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int ft_redir_outfile(char *outfile, int append)
{
	int	fd;
	if (append == 1)
		fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("Error reading outfile\n");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("Error duplicating output file\n");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	ft_redirections(t_cmd *cmd)
{
	if (cmd->hd == 1 && ft_redir_heredoc(cmd))
		return (1);
    if (cmd->hd == 0 && cmd->infile && ft_redir_infile(cmd->infile))
        return (1);
    if (cmd->outfile && ft_redir_outfile(cmd->outfile, cmd->append))
        return (1);
    return (0);
}

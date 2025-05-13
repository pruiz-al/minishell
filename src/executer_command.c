#include "../inc/minishell.h"

void	free_paths(char **paths, int i)
{
	while (paths[i])
		free(paths[i++]);
	free(paths);
}

char	*ft_path(char *path, char **comm)
{
	char	**paths;
	char	*route;
	char	*tmp;
	int		i;

	if (ft_strchr(comm[0], '/')) //if we get the route instead of the command
	{
		if (access(comm[0], F_OK | X_OK) == 0)
			return (ft_strdup(comm[0]));
		return (0);
	}
	if (!path)
		return (NULL);
	paths = ft_split(path + 5, ':');
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		route = ft_strjoin(tmp, *comm);
		free(tmp);
		if(access(route, F_OK | X_OK) == 0)
		{
			free_paths(paths, i);
			return (route);
		}
		free(route);
		i++;
	}
	i = 0;
	free_paths(paths, i);
	return (0);
}

void	execute_command(t_cmd *cmd, char **env)
{
	int		i;
	char	*path;

	i = 0;
	if (!cmd->args || !cmd->args[0])
	{
		ft_putstr_fd("Error: empty command\n", 2);
		exit (127);
	}
	while (env[i] && ft_strnstr(env[i], "PATH", 4) == 0)
		i++;
	if (!(env[i]))
	{
		ft_putstr_fd("No PATH found\n", 2);
		exit (127);
	}
	path = ft_path((env[i]), cmd->args);
	i = 0;
	if (!path)
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("Error executing.\n");
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
	free(path);
	exit(126);
}

#include "../minishell.h"

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	free_exit(t_shell *ms)
{
	ft_free_shell(ms);
	free(ms);
}

void	exec_exit(t_shell *ms, t_cmd *cmd)
{
	int	status;

	status = 0;
	if (cmd->args[1])
	{
		if (!ft_isnum(cmd->args[1]))
		{
			printf("minishel: exit: %s: numeric argument required\n", cmd->args[1]);
			free_exit(ms);
			exit(2);
		}
		status = (ft_atoi(cmd->args[1]) % 256);
		if (cmd->args[2])
		{
			printf("minishell: exit: too many arguments\n");
			ms->last_exit_st = 1;
			return ;
		}
	}
	printf("exit\n");
	free_exit(ms);
	exit((unsigned char)status);
}
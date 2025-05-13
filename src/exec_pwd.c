#include "../minishell.h"

int exec_pwd(void)
{
	char	*pwd;

	pwd = getcwd(NULL,0);
	if (!pwd)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", pwd);
	free(pwd);
	return (0);
}

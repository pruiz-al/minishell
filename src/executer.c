#include "../inc/minishell.h"

void    ft_check_exitstat(int status, t_cmd *cmd)
{
    if (WIFEXITED(status))
        cmd->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        cmd->exit_status = 128 + WTERMSIG(status);
}

void	execute_builtin(t_shell *ms, t_cmd *cmd)
{
	if (!cmd->args[0])
        cmd->args[0] = ft_strdup("");
    if (!ft_strncmp(cmd->args[0], "echo", 5))
        ms->last_exit_st = 1;//exec_echo(ms, cmd);
    else if (!ft_strncmp(cmd->args[0], "cd", 3))
        ms->last_exit_st = 1;//exec_cd(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "pwd", 4))
        ms->last_exit_st = exec_pwd();
	else if (!ft_strncmp(cmd->args[0], "export", 7))
        ms->last_exit_st = 1;//exec_export(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "unset", 6))
        ms->last_exit_st = 1;//exec_unset(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "env", 4))
        ms->last_exit_st = 1;//exec_env(ms, cmd);
	else if (!ft_strncmp(cmd->args[0], "exit", 5))
        exec_exit(ms, cmd);
	else
		return ;
}

void    child_process(t_cmd *cmd, int prevfd, int pipefd[2], t_shell *ms, char **envp[])
{
    if (prevfd != -1)
        dup2(prevfd, STDIN_FILENO);
    if (cmd->next)
        dup2(pipefd[1], STDOUT_FILENO);
    if (prevfd != -1)
        close(prevfd);
    if (cmd->next)
    {
        close(pipefd[0]);
        close(pipefd[1]);
    }
    if (ft_redirections(cmd) == 1)
        exit (1);
    if (cmd->is_btn)
    {
        execute_builtin(ms, cmd);
        exit(ms->last_exit_st);
    }
    else
        execute_command(cmd, *envp);
    perror("Error executing\n");
    exit (126);
}

void    parent_process(pid_t pid, t_cmd *cmd, int *prevfd, int pipefd[2])
{
    int status;

    if (*prevfd != -1)
        close(*prevfd);
    if (cmd->next)
    {
        close(pipefd[1]);
        *prevfd = pipefd[0];
    }
    else
        *prevfd = -1;
    waitpid(pid, &status, 0);
    ft_check_exitstat(status, cmd);
}

void    ft_exec_commands(t_shell *ms, char **envp[])
{
    pid_t   pid;
    int     pipefd[2];
    int     prevfd;
    t_cmd   *cmd;

    cmd = ms->cmd_lst;
    prevfd = -1;
    while (cmd)
    {
        if (cmd->next && pipe(pipefd) == 1)
        {
            perror("Error creating pipe\n");
            exit(1);
        }
        pid = fork();
        if (pid == -1)
        {
            return (perror("Error creating child process.\n"));
            exit(1);
        }
        if (pid == 0)
            child_process(cmd, prevfd, pipefd, ms, envp);
        else
        {
            parent_process(pid, cmd, &prevfd, pipefd);
			if (cmd->is_btn && !ft_strncmp(cmd->args[0], "exit", 5))
        		exit(ms->last_exit_st);
            cmd = cmd->next;
        }
    }
}

#include "pipex.h"

/*EXECUTION

		INITIALISER UN TABLEAU DE PID = pid[2]
		CREER UNE PREMIERE FORK

			FORMATER CHILD EN FONCTION DE SA POSITION (1ER OU 2EME)

			GERER LES REDIRECTIONS

				INITALISER IN_FD ET OUT_FD

				AFFECTER A  CMD.IN_FD -> INFILE_FD SI 1ER CHILD
				AFFECTER A  CMD.OUT_FD -> PIPE[0] SI 1ER CHILD

			SI ERREUR (IN_FD < 0 OU OUT_FD < 0)
				ERREUR (FREE ET EXIT) /!\ ON EST TOUJOUR DANS UN CHILD
			DUP2(INFILEFD, STDIN)
			DUP2(OUTFILEFD, STDOUT)
			FERMER TOUT LE RESTE

			CREER L'EXECUTIION

				FAIRE PASSER  DANS EXECVE
				GERER LES CAS D'ERREURS OU EXECVE ECHOUE

		CREER UNE DEUXIEME FORK

			FORMATER CHILD EN FONCTION DE SA POSITION (1ER OU 2EME)

				GERER LES REDIRECTIONS

				INITALISER IN_FD ET OUT_FD

				AFFECTER A  IN_FD -> PIPE[1] SI 2EME CHILD
				AFFECTER A  OUT_FD -> OUTFILE_FD SI 2EME CHILD

			SI ERREUR (IN_FD < 0 OU OUT_FD < 0)
				ERREUR (FREE ET EXIT) /!\ ON EST TOUJOUR DANS UN CHILD
			DUP2(INFILEFD, STDIN)
			DUP2(OUTFILEFD, STDOUT)
			FERMER TOUT LE RESTE

			CREER L'EXECUTIION

				FAIRE PASSER  DANS EXECVE
				GERER LES CAS D'ERREURS OU EXECVE ECHOUE

	PENDANT CE TEMPS LA DANS LE PARENT :

		ON COMMENCE PAR TOUT FERMER
		ON ATT POUR LES 2 PROCESSUS DE SE TERMINER
		ON AFFECTE A STATUS LA VALEUR DE RETOUR DE LA DERNIERE COMMANDE RECU PAR WAITPID
		ON RETURN
*/

// int		child2(t_arg *pipex, int tab_pid[2])
int		child2(t_arg *pipex)
{
	pipex->cmd2.fdinput = pipex->pipe[0];
	// TODO : ???
	// if (pipex->cmd1.fdinput < 0 || pipex->cmd1.fdoutput < 0)
	// {
	// 	freePipex(pipex);
		// close(pipex);
	// 	exit(1); // exit code pour ce cas d'erreur???
	// }
	dup2(pipex->cmd2.fdinput, STDIN_FILENO);
	dup2(pipex->cmd2.fdoutput, STDOUT_FILENO);
	closePipex(pipex);
	execve(pipex->cmd2.path, pipex->cmd2.args, pipex->env);
	freePipex(pipex);
	closeChild2(pipex);
	exit(errno); // exit code pour ce cas dérreur???
	// est ce que je dois return une valeur jsp comment je fait pour recup le status?
}

// int		child1(t_arg *pipex, int tab_pid[2])
int		child1(t_arg *pipex)
{
	t_cmd *child;

	child = &pipex->cmd1;
	child->fdoutput = pipex->pipe[1];
	// TODO : ???
	// if (child->fdinput < 0 || child->fdoutput < 0)
	// {
	// 	freePipex(pipex);
		// close(pipex);
	// 	exit(1); // exit code pour ce cas d'erreur???
	// }
	dup2(child->fdinput, STDIN_FILENO);
	close(child->fdinput);
	dup2(child->fdoutput, STDOUT_FILENO);
	close(child->fdoutput);
	execve(child->path, child->args, pipex->env);
	freePipex(pipex);
	closeChild1(pipex);
	exit(errno); // exit code pour ce cas dérreur???
	// est ce que je dois return une valeur jsp comment je fait pour recup le status?
}

void	exec(t_arg *pipex)
{
	int		j = 0;
	int		tab_pid[2];
	int 	status;

	// tab_pid[0] = -1;
	// tab_pid[1] = -1;

	if (pipe(pipex->pipe) == -1)
	{
		ft_printf("Pipex error, pipe creation : %s\n", strerror(errno));
		freePipex(pipex);
		closeFiles(pipex);
		exit(1);
	}
	tab_pid[0] = fork();
	if (tab_pid[0] == 0)
		// child1(pipex, tab_pid);
		child1(pipex);
	tab_pid[1] = fork();
	if (tab_pid[1] == 0)
		// child2(pipex, tab_pid);
		child2(pipex);
	closePipex(pipex);
	while (j <= 1)
	{
		waitpid(tab_pid[j], &status, 0);
		j++;
	}
	freePipex(pipex);
	exit(status);
}

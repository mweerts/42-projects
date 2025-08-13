
void	ft_exe()


int	main(int argc, char **argv, char **env)
{
	if (argc != 4)
	{
		std::cout << "add parameters, name program, congif, ip serv" << "\n\n";
	}
	else
	{
		if (execve(argv[1], argv[2], env) == 1)
		{
		}
	}
}

#include "Zombie.hpp"

int main(int argc, char **argv)
{
	Zombie *horde;
	int	N = 5;

	if (argc != 2)
	{
		std::cout << RED << "error: Wrong number of arguments." << END << std::endl;
		return (0);
	}
	if (atoi(argv[1]) <= 0)
	{
		std::cout << RED << "error: Please launch with a positive number of zombies." << END << std::endl;
		return (0);
	}
	horde = zombieHorde(N, "Bob");
	for (int i = 0; i < N; i++)
		horde[i].announce();
	delete[] horde;
	return (0);
}
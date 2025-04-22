#include "Zombie.hpp"

int main(void)
{
	Zombie zomb1("Normal Zombie");
	Zombie *zomb2 = newZombie("Allocated Zombie");
	randomChump("RandomChump Zombie");

	zomb1.announce();
	zomb2->announce();
	
	delete (zomb2);
	return (0);
}
#include "GAUtils.h"

/*
* Input: Vector of Animals
* Output: Sorted vector of Animals; Ascending order
* Funcation: Sorts the vector of Animals in ascending order
* Notes: None
*/
void GAUtils::rankPop(std::vector<std::shared_ptr<Animal>> &pop){
	std::sort(pop.begin(), pop.end(), compareAnimals);
}

/*
* Input: Gene vector
* Output: Fitness value
* Funcation: Determines the fitness value based on the genes supplied
* Notes: None
*/
float GAUtils::fitnessSingle(std::vector<float> &genes){
	float total = 0.0;
	int i = 0;
	for (auto & gene : genes){
		total += gene;
		i++;
	}
	return total/(float)i;
}

/*
* Input: Number of genes to generate
* Output: Vector of floats 
* Funcation: Generates a random vector of floats. Length is based on input
* Notes: None
*/
std::vector<float> GAUtils::generateGenes(int &genesNum){
	std::vector<float> genes;
	for (int i = 0; i < genesNum; i++){
		float r = randFloatGen();
		genes.push_back(r);
	}
	return genes;
}

/*
* Input: None
* Output: Chararacter M or F
* Funcation: Returns either M or F based on random number 
* Notes: None
*/
char GAUtils::randSex(){
	char c;
	if (randIntGen(10) > 5)
		c = 'M';
	else
		c = 'F';
	return c;
}


/*
* Input: None
* Output: Random float 
* Funcation: Returns a random float with a precision of 4
* Notes: Non-determinsitic number generation taken from https://msdn.microsoft.com/en-us/library/bb982398.aspx
*/
float GAUtils::randFloatGen(){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(1, 999);

	return (float)(dist(gen) / 1000.0);
}

/*
* Input: Upperbound on the range of random interger (e.g. upperBound = 7, random number from 0 to 7)
* Output: Random number from 0 to specified upperbound
* Funcation Summary: Returns a random integer from 0 to the supplied parameter
* Notes: Non-determinsitic number generation taken from https://msdn.microsoft.com/en-us/library/bb982398.aspx
*/
int GAUtils::randIntGen(int upperBound){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(0, upperBound);

	return dist(gen);
}

int GAUtils::randIntGen(int lowerBound, int upperBound){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dist(lowerBound, upperBound);

	return dist(gen);
}

/*
* Input: Two animals objects
* Output: None
* Function Summary: Compares animal 1 to animal 2 based on fitness
* Notes: None
*/
bool GAUtils::compareAnimals(std::shared_ptr<Animal> a1, std::shared_ptr<Animal> a2) {
	return a1->getFitness() > a2->getFitness();
}

/*
* Input: wolfPopulation (vector of Animal struct objects)
* Output: none
* Function Summary: Outputs the entire wolfPopulation
* Notes: None
*/
void GAUtils::popOut(std::vector<std::shared_ptr<Animal>> &pop){
	for (auto & animal : pop){
		std::cout << animal->getTag() + "  " + std::to_string(animal->getFitness()) << std::endl;
	}
}

/*
* Input: wolfPopulation (vector of Animal struct objects) and number of animals to output (starting from position 1)
* Output: none 
* Outputs wolfPopulation to supplied paramter
* Notes: None
*/
void GAUtils::popOut(std::vector<std::shared_ptr<Animal>> &pop, int x){
	for (int i = 0; i < x; i++){
		std::cout << std::to_string((i+1))+": " +pop[i]->getTag() + "  " + std::to_string(pop[i]->getFitness()) << std::endl;
	}
}

/*
* Input: wolfPopulation (vector of Animal struct objects), x number of years
* Output: none
* Advances each member of wolfPopulation x number of years
* Notes: None
*/
void GAUtils::agePopulation(std::vector<std::shared_ptr<Animal>> &pop, int x) {
	for (int i = 0; i < pop.size(); i++) {
		pop[i]->setAge(pop[i]->getAge() + x);
	}
}

/*
* Input: wolfPopulation (vector of Animal struct objects)
* Output: none
* Kills every individual older than 23 (for now)
* Notes: None
*/
void GAUtils::killSeniorCitizens(std::vector<std::shared_ptr<Animal>> &pop) {
	for (int i = 0; i < pop.size(); i++) {
		if (pop[i]->getAge() > 23) {
			//Animal a = pop[i];
			pop.erase(pop.begin() + i);
			i--;
		}
	}
}
#include <stdio.h>
#include <iostream>
#include <fstream>

#include "CUnit.h"
#include "CCircuit.h"
#include "Genetic_Algorithm.h"

#include <float.h>
// export OMP_STACKSIZE=16M

// Uncomment to enable writing optimal result to file
#define WRITE_VECTOR

int main(int argc, char * argv[])
{
	int num_offspring = 100; // number of population
	double cross_prob = 0.9; // crossing probability
	double mutation_rate = 0.01; // mutation rate
	int num_unit = 10; // number of units in one circuit
	int eva_iteration = 2000; // number of max iterations for evaluating
	int num_run = num_unit / 3 + 5; // number of runs for avoiding local optimum
	double max_count_optimum = 200; // max iterations stay on local optimum
	double tolerance_eval = 1.0e-5; // tolerance of evaluating
	int max_iteration_ga = 4000; // max iteration for evolving

	Genetic_Algorithm ga = Genetic_Algorithm(num_offspring, cross_prob, mutation_rate, num_unit, eva_iteration, num_run, max_count_optimum, tolerance_eval, max_iteration_ga);
	
	int res[2 * num_unit +1];

	double performance = ga.G_A(res);

	std::cout << "\nfinal score: " << performance;

	std::cout << "\nfinal circuit: ";

	for (int i = 0; i < num_unit * 2 + 1; i++)
	{
		std::cout << res[i] << ",";
	}

	std::cout << "\n";
	

	// std::cout << ga.Evaluate_Circuit(res, 1e-6, 2000, num_unit) << std::endl;
	
#ifdef WRITE_VECTOR
    std::ofstream txt_vector;
    txt_vector.open("./visualization/circuit_vector.txt");
    for (int i = 0; i < 2 * num_unit + 1; i++)
        txt_vector << res[i] << ' ';
    txt_vector.close();
#endif
	return 0;
}

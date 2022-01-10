#include <stdio.h>
#include <cmath>
#include <vector>
#include <iostream>
#include "CUnit.h"
#include "CCircuit.h"
#include "Genetic_Algorithm.h"
#include <ctime>
#include <string.h>
#include <algorithm>
#include <omp.h>
#include <float.h>
#include <random>

using namespace std;

// This function is the entrance
double Genetic_Algorithm::G_A(int *res) {

    // initialize population array
    int **parent_circuit = new int* [num_offspring];
    int **child_circuit = new int* [num_offspring];

    for (int i = 0; i < num_offspring; i++)
    {
        parent_circuit[i] = new int [vector_size];
        child_circuit[i] = new int [vector_size];
        
    }
    
    double max_performance = 0.0;

    // run several times to avoid local optimum
    for (int k = 0; k < num_run; k++)
    {

        generate_initial_population(parent_circuit);
        generate_initial_population(child_circuit);

        double performance = do_iterations(parent_circuit, child_circuit);

        if (performance > max_performance) {
            max_performance = performance;
            memcpy(res, parent_circuit[0], sizeof(int) * vector_size);
        }


        cout << "candidate " << k << ": ";

        for (int j = 0; j < vector_size; j++)
        {
            cout << parent_circuit[0][j] << ",";
        }

        cout << " score: " << performance << endl;

    }

    for (int i = 0; i < num_offspring; i++)
    {
        delete[] parent_circuit[i];
        delete[] child_circuit[i];
    }
        
    delete[] parent_circuit;
    delete[] child_circuit;


    return max_performance;
    
}

double Genetic_Algorithm::Evaluate_Circuit_parallel(int* circuit_vector, double tolerance, int max_iterations, int num_cells) {
	/*
	This function models the overall circuit simulation, calculating the mass flow rate of each component
	in each stream.
	Successive substitution algorithm has been implemented in parallel (using OpenMP) as convergence for
	this problem is guaranteed with this method.

	Input:
	int* circuit_vector : array input
	double tolerance	: pre-defined tolerance for convergence
	int max_iterations	: maximum number of iterations before divergence is concluded
	int num_cells		: number of units/cells present in the input circuit_vector

	Output:
	double Performance	: Performance value (pounds per second) is returned

	If divergence has occurred: -50000 is returned

	*/

	// allocating circuit specification inputs - 
	// setting the recovery rates for gormanium & waste, income and cost of concentrate streams  
	double val_rec = 0.2;
	double wast_rec = 0.05;
	int gormanium_price = 100;
	int waste_cost = -500;

	//use change of feed rate as a threshold
	double rate_change = 1;

	//initialize performance score
	double Performance = 0.0;

	// allocating num_cells+2 number of nodes, treating concentrate and waste streams as nodes inclusively
	// feed_val and feed_waste are used to calculate performance
	std::vector<CUnit> units(num_cells + 2);

	//initialize num_cell of units 
	//the last two(concentrate and waste)nodes don't need to be initialized
	//because of their duties are just containing the stream going to them 
	for (int i = 0; i < num_cells; i++)
	{
		units[i].conc_num = circuit_vector[2 * i + 1];
		units[i].tails_num = circuit_vector[2 * i + 2];
		units[i].feed_val = 0.0;
		units[i].feed_waste = 0.0;
		units[i].temp_val = 0.0;
		units[i].temp_waste = 0.0;
	}

	// give out some default value
	units[num_cells].temp_val = 0.0;
	units[num_cells].temp_waste = 0.0;

	// ensuring the feed and waste values are given to the appropriate first unit in the vector
	units[circuit_vector[0]].feed_val = 10;
	units[circuit_vector[0]].feed_waste = 100;

	// Iteration loop uses pre-defined max_iterations
	// If the number of iterations meets this value, performance has not converged
	for (int iter = 0; iter < max_iterations; iter++)
	{
#pragma omp parallel for num_threads(4)
		// In each iteration, we loop over all the cells, using feed_val and feed_waste to 
		// calculate the concentrate_val, concentrate waste, tailing value and tailing waste.
		for (int i = 0; i < num_cells; i++)
		{
			// calculate the flowrate of each component in both the concentrate and tailings streams
			units[i].conc_val = units[i].feed_val * val_rec;
			units[i].conc_waste = units[i].feed_waste * wast_rec;
			units[i].tail_val = units[i].feed_val * (1 - val_rec);
			units[i].tail_waste = units[i].feed_waste * (1 - wast_rec);

			// set the feed to the cell receiving the circuit feed equal to the flowrates of the circuit feed
			// accumulating input-streams as more than one unit may point at a single cell
			// use of atomic is to ensure only one process at one time to does the accumulation calculation.
#pragma omp atomic
			units[units[i].conc_num].temp_val += units[i].conc_val;
#pragma omp atomic
			units[units[i].conc_num].temp_waste += units[i].conc_waste;
#pragma omp atomic
			units[units[i].tails_num].temp_val += units[i].tail_val;
#pragma omp atomic
			units[units[i].tails_num].temp_waste += units[i].tail_waste;

			//Check the difference between the newly calculated feed rate and the old feed rate for each cell.
			//if (units[i].feed_val != 0 )
			//{
			//		rate_change = std::abs(units[i].old_rate - units[i].feed_val / (units[i].feed_val + units[i].feed_waste));

			//		if (rate_change <= tolerance)
			//		{
			//			return Performance;
			//		}

			//		units[i].old_rate = units[i].feed_val / (units[i].feed_val + units[i].feed_waste);

			//}

			// resetting feed_val and feed_waste
			units[i].feed_val = 0;
			units[i].feed_waste = 0;

		}

		//update performance
		//after each iteration, we check if the change of performance less than tolerance
		//if yes, then we return the performance as the result
		double thisPerformance = (units[num_cells].temp_val * gormanium_price + (units[num_cells].temp_waste) * waste_cost);
		if (Performance != 0 && std::abs(Performance - thisPerformance) <= tolerance)
		{
			//convergence criteria : the total mass of concentrate and tailing should be equal to or slightly smaller than the initial feed
			if (std::fabs(units[num_cells].temp_val + units[num_cells].temp_waste + units[num_cells + 1].temp_val + units[num_cells + 1].temp_waste - 110) < 110 * 0.001)
			{
				return thisPerformance;
			}
			else
			{
				return -50000;
			}
		}

		//std::cout << iter <<"   "<<Performance << std::endl;
		Performance = thisPerformance;

#pragma omp parallel for num_threads(4)
		//copy sum of feed material in temp to feed_val and feed_wast
		for (int i = 0; i < num_cells; i++)
		{
			//calculate the flowrate of each component in both the concentrate and tailings streams
			units[i].feed_val = units[i].temp_val;
			units[i].feed_waste = units[i].temp_waste;

			//reset temp_val and temp_waste after every iteration
			units[i].temp_val = 0.0;
			units[i].temp_waste = 0.0;

		}

	}
	//if the loop ends because of the limitation of iteration, it means performance will not converge
	//then we return 100*(-500) as a sign to imply the performance diverges when iteration times goes up
	return -50000;
}


double Genetic_Algorithm::Evaluate_Circuit(int* circuit_vector, double tolerance, int max_iterations, int cell_units) {

	/*
	This function models the overall circuit simulation, calculating the mass flow rate of each component
	in each stream.
	Successive substitution algorithm has been implemented in serial as convergence for
	this problem is guaranteed with this method.

	Input:
	int* circuit_vector : array input
	double tolerance	: pre-defined tolerance for convergence
	int max_iterations	: maximum number of iterations before divergence is concluded
	int num_cells		: number of units/cells present in the input circuit_vector

	Output:
	double Performance	: Performance value (pounds per second) is returned

	If divergence has occurred: -50000 is returned

	*/

	int ini_val;
	int ini_waste;
	double val_rec = 0.2;
	double wast_rec = 0.05;
	int gormanium_price = 100;
	int waste_cost = -500;
	bool flat = true;

	//use change of feed rate as a threshold
	double rate_change = 1;

	//get performance score
	double Performance = 0.0;

	std::vector<CUnit> units(cell_units + 2);

	//initialize the units 
	for (int i = 0; i < cell_units; i++)
	{
		units[i].conc_num = circuit_vector[2 * i + 1];
		units[i].tails_num = circuit_vector[2 * i + 2];
		units[i].feed_val = 0.0;
		units[i].feed_waste = 0.0;
	}

	units[circuit_vector[0]].feed_val = 10;
	units[circuit_vector[0]].feed_waste = 100;

	for (int iter = 0; iter < max_iterations; iter++)
	{
		//recursion process
		for (int i = 0; i < cell_units; i++)
		{
			//calculate the flowrate of each component in both the concentrate and tailings streams
			units[i].conc_val = units[i].feed_val * val_rec;
			units[i].conc_waste = units[i].feed_waste * wast_rec;
			units[i].tail_val = units[i].feed_val * (1 - val_rec);
			units[i].tail_waste = units[i].feed_waste * (1 - wast_rec);

			//Set the feed to the cell receiving the circuit feed equal to the flowrates of the circuit feed
			units[units[i].conc_num].feed_val += units[i].conc_val;
			units[units[i].conc_num].feed_waste += units[i].conc_waste;
			units[units[i].tails_num].feed_val += units[i].tail_val;
			units[units[i].tails_num].feed_waste += units[i].tail_waste;

			//reset feed stream
			units[i].feed_val = 0.0;
			units[i].feed_waste = 0.0;

		}

		//update performance
		//after each iteration, we check if the change of performance less than tolerance
		//if yes, then we return the performance as the result
		double thisPerformance = (units[cell_units].feed_val * gormanium_price + (units[cell_units].feed_waste) * waste_cost);
		if (Performance != 0 && std::abs(Performance - thisPerformance) <= tolerance)
		{
			//convergence criteria: the total mass of concentrate and tailing should be equal to or slightly smaller than the initial feed
			if (std::abs(units[cell_units].feed_val + units[cell_units].feed_waste + units[cell_units + 1].feed_val + units[cell_units + 1].feed_waste - 110) < 110 * 0.001)
			{
				return thisPerformance;
			}
			else
			{
				return -50000;
			}
		}

		//std::cout << iter <<"   "<<Performance << std::endl;
		Performance = thisPerformance;
	}
	return -50000;
}


Genetic_Algorithm::Genetic_Algorithm(int num_offspring, double crossing_prob, double mutation_rate, int num_units, int max_iteration_eval, int num_run, int max_count_optimum, double tolerance_eval, int max_iteration_ga) {
    this->num_offspring = num_offspring;
    this->crossing_prob = crossing_prob;
    this->mutation_rate = mutation_rate;
    this->num_units = num_units;
    this->vector_size = num_units * 2 + 1;
    this->max_iteration_eval = max_iteration_eval;
    this->num_run = num_run;
    this->tolerance_eval = tolerance_eval;
    this->max_count_optimum = max_count_optimum;
    this->max_iteration_ga = max_iteration_ga;
    srand(time(0));
    
}

Genetic_Algorithm::~Genetic_Algorithm() {
}

void Genetic_Algorithm::generate_initial_population(int **parent_circuit) {

    // generate circuit population
    
    random_device rd;
    mt19937 gen(rd());

// #pragma omp parallel for
    for (int i = 0; i < num_offspring; i++)
    {
        
        do
        {
            parent_circuit[i][0] = gen() % num_units;

            for (int j = 1; j < vector_size; j+=2)
            {   
                // Concentrate stream unit, ensures no self-recycle
                int con;
                do
                {
                    con = gen() % (num_units + 2); 
                } while (con == (j - 1) / 2);
                parent_circuit[i][j] = con;

                // Waste stream unit, ensures no self-recycle and different destination for 
                // concentrate and waste
                int waste;
                do
                {
                    waste = gen() % (num_units + 2);
                } while (waste == (j - 1) / 2 || waste == con);
                parent_circuit[i][j + 1] = waste;
            }

            // ensure at least one output point to concentrate
            // and one output point to tailing
            parent_circuit[i][gen() % (vector_size - 1) + 1] = num_units;
            parent_circuit[i][gen() % (vector_size - 1) + 1] = num_units + 1;


        } while (!Check_Validity(parent_circuit[i], this->num_units) || Evaluate_Circuit(parent_circuit[i], tolerance_eval, max_iteration_eval, this->num_units) < -1000.0);
        
    }
}

// double Genetic_Algorithm::do_iterations() {
double Genetic_Algorithm::do_iterations(int **parent_circuit, int **child_circuit) {
    double max_fitness = 0.0;
    // Iteration count for stopping criteria
    int it_total = 0;
    double last_max = 0;
    // Record the initial mutation rate and crossing probability
    double this_mutation_rate = mutation_rate;
    double this_crossing_prob = crossing_prob;

    for (int k = 0; k < max_iteration_ga; k++)
    {
        // Calculate fitness values for all parents
        // prefix_fitness is an array such that prefix_fitness[i] = comp_fitness(i th parent) + comp_fitness(i - 1 th parent)
        // Design aimed at facilitating probability of crossover based on fitness level
        double prefix_fitness[num_offspring + 1];
        prefix_fitness[0] = 0.0;
        int max_index = -1;
        max_fitness = 0.0;

        for(int i = 0; i < num_offspring; i++)
        {   
            // calculate the performance of every parent
            double fitness = Evaluate_Circuit(parent_circuit[i], tolerance_eval, max_iteration_eval, num_units);

            // record the best one
            if (fitness > max_fitness)
            {
                max_fitness = fitness;
                max_index = i;
            }

            // if its performance is low, give it a very lower weight
            // but weight cannot be negative, so give it 1
            if (fitness <= 1) {
                fitness = 1;
            }
 
            prefix_fitness[i + 1] = fitness + prefix_fitness[i];

        }

        // take the best circuit to the child list without change
        memcpy(child_circuit[0], parent_circuit[max_index], sizeof(int) * vector_size);

        // Check if theres is any improvement
        if (last_max == max_fitness)
        {
            it_total ++;
            // Dynamically change the hyper-parameter if stuck in local optimum for many iterations
            // if we stuck in local optimum for a long time, we can amend the rates
            if (it_total % (max_count_optimum / 10) == 0)
            {
                this_mutation_rate += 0.01;
                this_crossing_prob -= 0.03;
            }

            // stop if stuck in a local point for too many iterations
            if (it_total == max_count_optimum)
            {
                std::swap(child_circuit, parent_circuit);
                return max_fitness;
            }
        } else if (it_total > 0){
            it_total = 0;
            this_mutation_rate = mutation_rate;
            this_crossing_prob = crossing_prob;
        }

        last_max = max_fitness;
        
        // the total weight of parents
        int total = prefix_fitness[num_offspring];
        
        
        // int index_child = 1;
        // while (index_child < num_offspring)
#pragma omp parallel for
        for (int i = 1; i < num_offspring; i += 2)
        {
            int candidate1[vector_size];
            int candidate2[vector_size];

            do
            {
                // select father
                double father = rand() % total;
                int father_index = 0;
                while (father > prefix_fitness[++father_index]);
                
                // select mother
                double mother = rand() % total;
                int mother_index = 0;
                while (mother > prefix_fitness[++mother_index]);

                // Performing crossover on two selected parents
                Crossover(parent_circuit[father_index - 1], parent_circuit[mother_index - 1], candidate1, candidate2, this_crossing_prob);

                // Performing mutation on both vector
                Mutations(candidate1, this_mutation_rate);
                Mutations(candidate2, this_mutation_rate);

                // reproducing until two children all valid
            } while (!Check_Validity(candidate1, this->num_units) || !Check_Validity(candidate2, this->num_units));

            memcpy(child_circuit[i], candidate1, sizeof(int) * vector_size);
            if (i + 1 < num_offspring)
            {
                memcpy(child_circuit[i + 1], candidate2, sizeof(int) * vector_size);
            }
            
        }
        
        std::swap(child_circuit, parent_circuit);
    }
    return max_fitness;
}


void Genetic_Algorithm::Crossover(int *father_vector, int *mother_vector, int *child1, int *child2, double crossing_prob)
{   
    
    // We put the random value generation inside the function such that if the function runs once
    // and (one of) the children vectors aren't valid, there's a chain that when the function is re-run
    // the algorithm will skip the cross-over process entirely
    double rand_num = rand() / double(RAND_MAX);
    if (rand_num < crossing_prob)
    {
        // random cross point
        int rand_cpt = (rand() % this->vector_size - 1) + 1;

        memcpy(child1, father_vector, sizeof(int) * rand_cpt);
        memcpy(child2, mother_vector, sizeof(int) * rand_cpt);

        child1 += rand_cpt;
        child2 += rand_cpt;
        father_vector += rand_cpt;
        mother_vector += rand_cpt;

        memcpy(child1, mother_vector, sizeof(int) * (vector_size - rand_cpt));
        memcpy(child2, father_vector, sizeof(int) * (vector_size - rand_cpt));

    } else {
        memcpy(child1, mother_vector, sizeof(int) * vector_size);
        memcpy(child2, father_vector, sizeof(int) * vector_size);
    }
}


void Genetic_Algorithm::Mutations(int *circuit_vector, double mutation_rate)
{
    //Mutation for the feed entry

    random_device rd;
    mt19937 gen(rd());
    if (gen() / double(RAND_MAX) < mutation_rate)
    {
        circuit_vector[0] = gen() % num_units;
    }

    // In other cases we need to ensure no self-recycling, and unique destination for each stream
    for (int i = 1; i < vector_size; i++)
    {
        if (gen() / double(RAND_MAX) < mutation_rate)
        {   
            int another_ouput = circuit_vector[i % 2 ? i + 1 : i - 1];
            int rand_mut;
            do {
                rand_mut = gen() % (num_units + 2);
            } while (rand_mut == another_ouput || rand_mut == (i - 1) / 2);
            circuit_vector[i] = rand_mut;
        }
    }
}

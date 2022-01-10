# Source Files Documentation

## `CCircuit.cpp`

This source file is used to test the validity of a circuit. Each separation unit is an instance of the `CUnit` class.

## Validation
#### bool Check_Validity(int* circuit_vector, int n)
The `Genetic_Algorithm` class validates potential circuits through a call to `Check_Validity`. This function determines a circuit's validity based on four criteria:
1. No units can self-recycle, a unit is not allowed to have a tailings stream that connects to itself.
2. The destination of a single unit's concentrate and tailings stream must be different, they cannot both go to the same other unit.
3. Every unit must have a route forward to both the concentrate and tailings outlet streams.
4. Every unit must be accessible from the feed.

Check_validity works by first determining whether or not conditions 1 and 2 are satisfied, then by checking if 3 then 4 are satisfied.

Criteria 1 and 2 are examined first as they can be determined in a performant manner through simple iteration over the 1D `circuit_vector` array. A loop is implemented which checks each unit's satisfaction of both criteria 1 and 2 in a single logical statement. Even for arrays of large circuits (> 60 units), traversing a 1D array and checking a single logical statement (albeit with three embedded or conditions), is a task that can be handled in a timely manner.

In order to check criteria 3 and 4, Check_Validity first copies the values from `circuit_vector` to an stl vector container of type CUnit objects, length `n` and initially marks all units as unseen. It then calls the recursive function `mark_units`, after which an interrogation of two global booleans `conc_outlet` and `tail_outlet` is performed to determine whether or not criterion 3 is satisfied. Finally, an iterative check is performed over the vector of CUnit objects to determine if all units have been seen and thus satisfy criterion 4.

Note, as soon as any of the checks are not satisfied, false is returned by the function and no further progress through the remaining code is made.

#### void mark_units(int unit_num)
This function marks each unit that can be seen from the feed, then modifies the two global booleans `conc_outlet` and `tail_outlet`, one for each outlet stream, if the concentrate and tailings outlet streams are connected to this unit. It works by first checking if the `unit_num` unit has been seen (which in the first call to the function will always be unseen), then marks it as seen and enters an if statement to first determine if the concentrate outlet, then the tailings outlet, are connected to this unit. If this unit is connected, then the global booleans `conc_outlet` and `tail_outlet` are modified to seen. If either the concentrate or tailings stream cannot be seen, a recursive call is made to `mark_units` to continue the process.

Note, for both `Check_Validity` and `mark_units`, seen refers to a boolean value of true, unseen refers to a boolean value of false.
 
#### void is_valid_loop(int unit_num, int* circuit, int curr)
This function is to detect the existence of tailing loop. In light of our experience, the circuit with tailing loop can always get very high score but actually it doesn't converge, so we implement this function. This function uses recursive method, and the time complexity is O(n), where n is the number of units.
 
## `Genetic_Algorithm.cpp`
 
The genetic algorithm has been implemented in a class `Genetic_Algorithm`, whose functions are detailed below.
 
### `Evaluate_Circuit`
###### Modelling the Circuit

This function models the overall circuit simulation, calculating the mass flow rate of each component in each stream.
Successive substitution algorithm has been implemented as convergence for this problem is guaranteed with this method and further parallelised (using OpenMP).

		Input:
			int* circuit_vector : array input
			double tolerance	: pre-defined tolerance for convergence
			int max_iterations	: maximum number of iterations before divergence is concluded
			int num_cells		: number of units/cells present in the input circuit_vector

		Output:
			double Performance	: Performance value (pounds per second) is returned
			
			If divergence has occured: -DBL_MAX is returned

A standard mass-balance simulator, with each unit recovering 20% of the valuable (gormanium material) and 5% of the waste collected within the concentrate stream. 
As a result the circuit model output represents the optimised product/waste purity with reference to maximum economic gain given specified product price and detriment waste cost values.

OpenMP was used to parallelise this iterative solver, accruing benefits in run-time.

An example Evaluate_Circuit test case:


	    //test case 2
	    int main_test2()
	    {
		std::cout << "Test 2: ------------------------------------------" << std::endl;
		const int num_cells2 = 5;
		int test_vector2[2 * num_cells2 + 1] = { 0, 4, 3, 2 , 0 , 5 , 4 ,4 , 6 , 2 ,1 };

		double performance2 = 0;
		double result = 24.82;

		performance2 = Evaluate_Circuit(test_vector2, 1.0e-8, 300, num_cells2);
		std::cout << performance2 << std::endl;

		if (std::fabs(Evaluate_Circuit(test_vector2, 1.0e-8, 300, num_cells2) - result < 1.0e-8))
		    std::cout << "Test 2 passed\n";
		else
		    std::cout << "Test 2 failed\n";

		return 0;
	    }


   Output:


	    Test 2: ------------------------------------------
	    24.8161
	    final iteration count: 30
	    Test 2 passed


An example Evaluate_Circuit test case carried out on the SERIAL VERSION
- This test case must be carried out on only the serial version of the code, as this is where a single iteration is represented accurately and can be measured against manually derived performance outputs.

	    //test case 1
	    int main_test1()
	    {
		std::cout << "Test 1: ------------------------------------------" << std::endl;
		const int num_cells1 = 3;
		int test_vector1[2 * num_cells1 + 1] = { 0, 1, 2, 3, 0, 0, 4 };

		double performance1 = 0;
		double tol = 1e-4;
		// manually derived results
		double results[10] = { -85.0, -84.075, -81.1071, -79.9036, -79.4943, -79.3611, -79.3182, -79.3045, -79.3001, -79.2987 };
		std::vector<double> outputs;
		bool flag = true;

		for (int i = 1; i < 11; i++)
		{
		    performance1 = Evaluate_Circuit(test_vector1, 1.0e-8, i, num_cells1);
		    outputs.push_back(performance1);
		    std::cout << outputs[i - 1] << " ";
		    if (abs(outputs[i - 1] - results[i - 1]) > tol)
		    {
			flag = false;
		    }
		}

		std::cout << std::endl;
		if (flag == false)
		{
		    std::cout << "Test 1 failed\n";
		}
		else {
		    std::cout << "Test 1 passed\n";
		}

		return 0;
	    }
    
    Output:

	    Test 1: ------------------------------------------
	    final iteration count: 1
	    -85 final iteration count: 2
	    -84.075 final iteration count: 3
	    -81.1071 final iteration count: 4
	    -79.9036 final iteration count: 5
	    -79.4943 final iteration count: 6
	    -79.3611 final iteration count: 7
	    -79.3182 final iteration count: 8
	    -79.3045 final iteration count: 9
	    -79.3001 final iteration count: 10
	    -79.2987
	    Test 1 passed
    

 
 ### `generate_initial_population`
 This function generates the population of the parent and child circuits
 
 `void generate_initial_population(int **parent_circuit);`
 
 This function gets a 2D int array and randomly generate a group of circuit (population), and also ensures that every circuit in this group is valid and convergent. 
 
 ### `do_iterations`
 Iterates through the genetic algorithm. 
 
 `double do_iterations(int **parent_circuit, int **child_circuit);`
 
 In this function, a group of population will continue evolving until the optimum circuit appears.
 1) Evaluate every parent circuit, the circuit with higher score will be more likely to mate.
 2) The circuit with best score will directly be the child, that is, enter next iteration.
 3) If the max score doesn't change for tens of iterations, we will increase the mutation rate and decrease the crossing rate. If the max scores doesn't change for hundreds of iterations, which means it reach a local optimum, the iteration will stop.
 4) Generate a number of pairs of parents for crossover, and the child will be possible to mutate. OpenMP is used here to boost the reproducing speed.
 5) Loop from 1 to 4 until stop.
 
 ### `Crossover`
 generates one random crossover point and applies this to the parent and child vectors. <br />
 
 `void Crossover(int *father_vector, int *mother_vector, int *child1, int *child2, double crossing_prob)`
 
 1. Randomly decide if the corssover is applied on the parent circuits based on the crossing_prob.
 2. If it doen not perform corssover on the parent circuit, the selected parent circuits go in to the next step.
 3. Else, the function will randomly choose one crossover point and perform crossover on the two parent.
 
 ### `Mutations`
 Generates a random probability for mutations to occur. <br />
 
 `void Genetic_Algorithm::Mutations(int *circuit_vector, double mutation_rate)`
 
 1. For each of the number in the circuit, the function will randomly decided if mutation will be applied based on the mutation rate.
 2. If the mutation is applied, the function will generate a random number to replace.

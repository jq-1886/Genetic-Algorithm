#include <cmath>
#include <iostream>

#include "CUnit.h"
#include "CCircuit.h"
#include "Genetic_Algorithm.h"

// Testing the mass balance 
void test_evalute_circuit()
{
      Genetic_Algorithm ga = Genetic_Algorithm(100, 0.9, 0.01, 10, 243, 243, 200, 1.0e-8, 10);

	  std::cout << "Test 1: ------------------------------------------" << std::endl;
	  const int num_cells2 = 5;
	  int test_vector2[2 * num_cells2 + 1] = { 0, 4, 3, 2 , 0 , 5 , 4 ,4 , 6 , 2 ,1 };

	  double performance2 = 0;
	  double result2 = 24.8162;

	  performance2 = ga.Evaluate_Circuit(test_vector2, 1.0e-8, 300, num_cells2);
	  //std::cout << performance2 << std::endl;

	  if (std::fabs(performance2 - result2) < 1.0e-3)
		  std::cout << "Test 1 passed\n";
	  else
		  std::cout << "Test 1 failed\n";


	  std::cout << "Test 2: ------------------------------------------" << std::endl;
	  const int num_cells3 = 10;
	  int test_vector3[2 * num_cells3 + 1] = { 0, 1, 2, 3, 4, 4, 5, 6, 7, 7, 8, 8, 9, 10, 11, 10, 11, 10, 11, 10, 11 };

	  double performance3 = 0;
	  //manually derived result
	  double result3 = -2300;

	  performance3 = ga.Evaluate_Circuit(test_vector3, 1.0e-8, 100, num_cells3);
	  std::cout << performance3 << std::endl;

	  if (std::fabs(performance3- result3) < 1.0e-8)
		  std::cout << "Test 2 passed\n";
	  else
		  std::cout << "Test 2 failed\n";

	  std::cout << "Test 3: ------------------------------------------" << std::endl;
	  const int num_cells4 = 15;
	  int test_vector4[2 * num_cells4 + 1] = { 12,7,12,7,0,7,14,10,1,10,3,14,6,13,5,10,4,1,11,1,8,15,7,1,5,7,2,0,16,1,9 };
	  double performance4 = 0;

	  double result4 = 408.55;

	  performance4 = ga.Evaluate_Circuit(test_vector4, 1.0e-8, 10000, num_cells4);

	  if (std::fabs(performance4 - result4) < 1.0e-2)
		  std::cout << "Test 3 passed\n";
	  else
		  std::cout << "Test 3 failed\n";

	  std::cout << "Test 4: ------------------------------------------" << std::endl;
	  const int num_cells5 = 20;
	  int test_vector5[2 * num_cells5 + 1] = { 18,14,2,14,16,14,9,8,19,20,15,8,18,18,12,14,0,4,13,14,10,3,11,19,17,18,21,4,14,8,3,20,8,14,7,5,6,14,1,8,5 };
	  double performance5 = 0;
	  double result5 = 630.77;
	  //parallel result = 630.80;

	  performance5 = ga.Evaluate_Circuit(test_vector5, 1.0e-8, 10000, num_cells5);

	  if (std::fabs(performance5 - result5) < 1.0e-1)
		  std::cout << "Test 4 passed\n";
	  else
		  std::cout << "Test 4 failed\n";
}

// Testing the whole program
void test_all()
{
      Genetic_Algorithm ga2 = Genetic_Algorithm(100, 0.9, 0.01, 5, 243, 1, 200, 1.0e-8, 1000);
      int res[11];

	ga2.G_A(res);
	
      std::cout << "Evaluate_Circuit(ga.result, 1.0e-8, 243) close to 24.8162:\n";
	if (std::fabs(ga2.Evaluate_Circuit(res, 1.0e-6, 243, 5) - 24.8162) < 1.0e-3)
	    std::cout << "pass\n";
    else
	    std::cout << "fail";
}

int main(int argc, char * argv[])
{
      // call function to test evaluate circuit
      test_evalute_circuit();

      // call function to test the whole program
      test_all();
}


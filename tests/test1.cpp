#include <iostream>

#include "CUnit.h"
#include "CCircuit.h"

// Testing the circuit validatin function
int main(int argc, char * argv[]){

    int valid[3] = {0, 1, 2};
    int invalid[3] = {0, 2, 2};

    int valid2[11] = {0,4,3,2,0,5,4,4,6,2,1};

    int valid3[31] = {6,16,7,7,13,11,12,15,5,3,6,0,2,14,12,1,12,14,11,5,16,11,9,4,1,0,8,5,10,2,6};

	std::cout << "Check_Validity({0,1,2}):\n";
        if (Check_Validity(valid, 1))
	    std::cout  << "pass\n";
	else
	    std::cout << "fail\n";

    std::cout << "Check_Validity(valid2):\n";
        if (Check_Validity(valid2, 5))
	    std::cout  << "pass\n";
	else
	    std::cout << "fail\n";
    
    std::cout << "Check_Validity(valid3):\n";
        if (Check_Validity(valid3, 15))
	    std::cout  << "pass\n";
	else
	    std::cout << "fail\n";
    
	std::cout << "Check_Validity({0,2,2}):\n";
        if (!Check_Validity(invalid, 3))
            std::cout << "pass\n";
        else
            std::cout << "fail\n";
    

}

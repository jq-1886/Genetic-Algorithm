#include <vector>
#include <string.h>
#include <stdio.h>
#include <CUnit.h>
#include <CCircuit.h>
#include <iostream>

// initialising a global vector of CUnit objects, to be used later in Check_Validity and mark_unit functions
std::vector<CUnit> units;

// setting up 2 global booleans to determine if all units lead to both outlet streams
bool conc_outlet = false;
bool tail_outlet = false;

// setting a global variable for the number of units in the circuit being assessed by Check_Validity
int n_units;

bool Check_Validity(int *circuit_vector, int n)
{
    // ensuring conc_outlet and tail_outlet are reset to false each time Check_Validity is called
    conc_outlet = false; // false is unseen, true is seen
    tail_outlet = false; // false is unseen, true is seen

    // integer variables used in self-recycling and unique destination checking
    int c_counter = 1;
    int t_counter = 2; 
    
    // assign the number of units to n, passed in by user
    n_units = n;

    //resize the vector of CUnits to size n
    units.resize(n);

    // self-recycling and unique destination checking run first as they simply iterate over circuit_vector
    // and so are performant enough, even for large arrays

    // self-recycling checking and unique concentrate and tailings destination checking
    for(int i = 0; i < n; i++)
    {
        // logic here says if the ith units concentrate or tailings stream leads to i or the concentrate and tailings stream
        // lead to the same destination, return false
        if(circuit_vector[t_counter] == i || circuit_vector[c_counter] == i || circuit_vector[c_counter] == circuit_vector[t_counter])
        {
          return false;
        }
        c_counter += 2; // increment c_counter by 2 to access the ith unit's concentrate stream
        t_counter += 2; // increment t_counter by 2 to access the ith unit's tailings stream
    }

    // a loop to copy the values from answer_vector to units vector of Cunit objects
    c_counter = 1; // resetting c_counter
    t_counter = 2; // resetting t_counter
    for(int i = 0; i < n; i++)
    {
        units[i].conc_num = circuit_vector[c_counter]; // copying across the id to which this units concentrate stream leads
        units[i].tails_num = circuit_vector[t_counter]; // copying across the id to which this units tailings stream leads 
        units[i].mark = false; // setting each unit to unseen, a mark of false is unseen, a mark of true is seen

        c_counter += 2; // increment c_counter by 2 to access the ith unit's concentrate stream
        t_counter += 2; // increment t_counter by 2 to access the ith unit's tailings stream
    }

    // call to mark_units to start marking each unit, starting at the root
    mark_units(circuit_vector[0]);

    // if statement to return false if either conc_outlet or tail_outlet are false
    if(!conc_outlet || !tail_outlet)
    {
        return false; // a false return here indicates that conc_outlet and tail_outlet cannot be seen
    }

    // loop to finally move through all members of the vector of Cunit objects and break if any are unmarked
    for(int i = 0; i < n; i++)
    {
        if(!units[i].mark)
        {
            return false; // a return false here indicates that not all units are able to be seen from root
        }
    }

    
    int *v = new int[2 * n + 1];

    // for (int i = 0; i < 2 * n + 1; i++)
    // {
    //     v[i] = circuit_vector[i];
    // }
    

    memcpy(v, circuit_vector, (2 * n + 1) * sizeof(int));

    for (int i = 0; i < n; i++)
    {
        if (v[2 * i + 2] != n + 1)
        {
            if (!is_valid_loop(n, v, i))
            {
                delete[] v;
                return false;
            }
        }
    }
    delete[] v;

    // for (int i = 0; i < n; i++)
    // {
    //     int ptr1 = circuit_vector[2 * i + 2];
    //     int ptr2 = ptr1;
    //     while (ptr1 != n + 1 && ptr2 != n + 1 && circuit_vector[ptr2 * 2 + 2])
    //     {
    //         ptr1 = circuit_vector[ptr1 * 2 + 2];
    //         ptr2 = circuit_vector[circuit_vector[ptr2 * 2 + 2]];
    //         if(ptr1 == ptr2) return false;
    //     }
    // }

    // finally true is returned if only all other requirements have been satisfied
    return true;
}


// This function used to detect the tailing loop, because tailing loop won't
// converge, even though it will get a high score
bool is_valid_loop(int num_units, int *circuit, int curr) {
    // find the next unit
    int next = circuit[2 * curr + 2];
    if (next == num_units + 1) {
        // if it's the destination, there is no tailing loop
        return true;
    } else if (next == -1) {
        // if we have been the unit, there is a loop
        return false;
    } else if (next == num_units) {
        // if the tailing point to concentrate, the circuit is invalid
        return false;
    } else {
        // mark units as visited
        circuit[2 * curr + 2] = -1;
        if (is_valid_loop(num_units, circuit, next)) {
            // if there is a way to destination, which means there is no loop
            // so we can mark this unit as destination
            circuit[2 * curr + 2] = num_units + 1;
            return true;
        } else {
            return false;
        }
    }
}

void mark_units(int unit_num)
{
    if(units[unit_num].mark) // if this unit has been seen already, return
    {
        return;
    }

    units[unit_num].mark = true; // mark this unit as seen

    // if conc_num does not point at a circuit outlet recursively call the function
    if (units[unit_num].conc_num<n_units)
    {
        mark_units(units[unit_num].conc_num); // mark_units is starts another journey
    }
    // if the id of this unit's concentrate stream equals n_units mark conc_outlet as seen
    else if (units[unit_num].conc_num == n_units) 
    { 
        conc_outlet = true;
    }
    
    //If tails_num does not point at a circuit outlet recursively call the function 
    if (units[unit_num].tails_num<n_units)
    {
        mark_units(units[unit_num].tails_num); // mark_units starts anouther journey 
    }
    // if the id of this unit's tailings stream equals n_units + 1 mark conc_outlet as seen
    else if (units[unit_num].tails_num == n_units + 1)
    {
        tail_outlet = true;
    }
}

#pragma once
// const int num_units = 5;

bool Check_Validity(int *circuit_vector, int n);
bool is_valid_loop(int num_units, int *v, int curr);
void mark_units(int unit_num);

class Genetic_Algorithm
{
private:
    int num_offspring;

    int num_units;

    int vector_size;

    double crossing_prob;

    double mutation_rate;

    int max_iteration_eval;

    int num_run;

    double tolerance_eval;

    int max_count_optimum;

    int max_iteration_ga;

    
public:

    double G_A(int *res);

    // yinze
    Genetic_Algorithm(int num_offspring, double crossing_prob, double mutation_rate, int num_units, int max_iteration_eval, int num_run, int max_count_optimum, double tolerance_eval, int max_iteration_ga);
    // yinze
    void generate_initial_population(int **parent_circuit);
    // siyi, yinze
    // double do_iterations();
    double do_iterations(int **parent_circuit, int **child_circuit);
    
    double Evaluate_Circuit(int *circuit_vector, double tolerance, int max_iterations, int n);
    double Evaluate_Circuit_parallel(int* circuit_vector, double tolerance, int max_iterations, int num_cells);
    
    // Computes fitness of a vector from its performance
    double comp_fitness(double Performance);

    // Yunjie
    void Crossover(int *father_vector, int *mother_vector, int *child1, int *child2, double crossing_prob);
    // void Crossover(int *father_vector, int *mother_vector, int *child1, int *child2);
    // Yunjie
    void Mutations(int *circuit_vector, double mutation_rate);
    // void Mutations(int *circuit_vector);

    ~Genetic_Algorithm();

};




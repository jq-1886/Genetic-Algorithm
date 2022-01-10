# Visualisation

Our genetic algorithm program should output a file named `circuit_vector.txt` file to the `visualization/` folder.
The visualisation script `visualisation.py` will take the list of numbers and output a .PNG file `circuit.png`.

`visualisation.py` requires a working python installation of Graphviz. For conda users a working environment can be: 

```
conda env create -f environment.yml
conda activate acse-4-p3
```

or you can (conda) install the packages yourself
```
conda install graphviz python-graphviz
```

Run the python script using
```
python visualisation.py
```

which should save `circuit.png` in the `visualization/` directory.

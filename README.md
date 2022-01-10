
<div align="center">


	This software package aims to advise on optimal mineral recovery using a genetic algorithm to simulate separation techniques.

</div>


<br>
<br>
<br>

# Contents

- [Instructions](#Instrucitions)
- [Implement](#Implement)
- [Build&compile](#Build&compile)
- [Execute](#Execute)
- [Programming Languages](#Languages)
- [Visualization](#Visualization)

***

# Instructions

- Please see `src/README.md` for source file documentation. Our executeable will output the results of the optimisation process, and it's performance value.

***

# Implement

## Set up environment

1. Ubuntu/Linux

	> `sudo apt install build-essential`

	> `sudo apt install cmake`

	> `pip install graphviz`

2. Windows

	A C++ compiler that supports the latest version(s) of OpenMP is required, ideally g++.

3. MacOS

	For reliable results it is recommended to use a package manager such as homebrew

	> `brew install cmake`

	Further details of cmake installation if required https://cmake.org/install/

	> `brew install graphviz`

	or

	> `pip install graphviz`

	Note if the clang compiler is being used please update to the latest version to make use of OpenMP functionality

## Build&compile

1. `cmake .`

2. `make`

## Execute

> `./main`

***

## Languages

* C++
* python

***

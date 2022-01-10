
<div align="center">

	<h3>
	This software package aims to advise on optimal mineral recovery using a genetic algorithm to simulate separation techniques.
	</h3>
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

## Editors         


&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Devang Patel&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Zifan Ding &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Jack Quested&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Jamesson Ipock&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Siyi Li&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Yinze Li&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
Yunjie Piao&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

***

## Visualization

#### 1. Below shows an example of a 10 cell circuit visualisation with its respective final performance value
<a href="#"><img src="https://github.com/acse-2020/acse-4-gormanium-rush-acanthite/blob/documentation/images/Post_processing.png?raw=true" width="1200"></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

#### 2. Below shows an example of a 25 cell circuit visualisation with its respective final performance value
<a href="#"><img src="https://github.com/acse-2020/acse-4-gormanium-rush-acanthite/blob/documentation/images/25cells.png?raw=true" width="1200"></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

#### 3. Performance changes as the number of units ranging from 5 to 25
<a href="#"><img src="https://github.com/acse-2020/acse-4-gormanium-rush-acanthite/blob/documentation/images/optimum.png?raw=true" width="800"></a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;


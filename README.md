# PropTreeLib
A flexible and feature-rich input file reader designed for use in scientific computing applications.

## Description
This project was inspired by extensive work with a number of scientific computing codes.
A typical structure for one of these codes is to have an input file that contains every runtime parameter.
The solver then reads this file and sets parameters accordingly.

The very nature of scientific computing means that every run counts. If you aren't debugging, the code is likely to run for
days, possibly even weeks, at a time. This costs a lot of money and effort, so it makes sense to make sure that every parameter is correct!

Sometimes, however, deadlines can get to us. Small mistakes are made, such as a typo on one input option that causes it to be ignored and/or
set to a default value. In some cases, this can cause an entire simulation to be garbage.

## Purpose
PropTreeLib is designed to fix the following symptoms of primitive input file readers:

* You run 10 different cases with slightly varying input parameters, but forgot to print the values of the parameters to the slurm log. Now half of the data is meaningless
and needs to be run again!



## License
[MIT](https://choosealicense.com/licenses/mit/)

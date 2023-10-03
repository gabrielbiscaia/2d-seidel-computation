# 2d-seidel-computation
Work of concurrent programming that consists in parallelizing 2d Seidel algorithm

Alunos:<br>
Arthur Maciel   120119<br>
Gabriel Biscaia 118928<br>
Pedro Zafalon   120117<br>

* * * * * * * * * *
<h1>PolyBench/C 3.2</h1>

Copyright (c) 2011-2012 the Ohio State University.
Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>


<h3>Available benchmarks:</h3>

* stencils/seidel-2d_sequential.c
* stencils/seidel-2d_pthreads.c
* stencils/seidel-2d_mpi.c

<br>
<h3>To compile the code:</h3>

$> gcc -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -o atax_base
<br>

<br>


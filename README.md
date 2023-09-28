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
<h3>Sample compilation commands:</h3>

* To compile a benchmark without any monitoring:

$> gcc -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -o atax_base
<br>

* To compile a benchmark with execution time reporting:

$> gcc -O3 -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -DPOLYBENCH_TIME -o atax_time

<br>
<h3>Some available options:</h3>

They are all passed as macro definitions during compilation time (e.g,
-Dname_of_the_option).

- POLYBENCH_TIME: output execution time (gettimeofday) [default: off]

- POLYBENCH_NO_FLUSH_CACHE: don't flush the cache before calling the
  timer [default: flush the cache]

- POLYBENCH_LINUX_FIFO_SCHEDULER: use FIFO real-time scheduler for the
  kernel execution, the program must be run as root, under linux only,
  and compiled with -lc [default: off]

- POLYBENCH_CACHE_SIZE_KB: cache size to flush, in kB [default: 33MB]

- POLYBENCH_STACK_ARRAYS: use stack allocation instead of malloc [default: off]

- POLYBENCH_DUMP_ARRAYS: dump all live-out arrays on stderr [default: off]

- POLYBENCH_CYCLE_ACCURATE_TIMER: Use Time Stamp Counter to monitor
  the execution time of the kernel [default: off]

- POLYBENCH_PAPI: turn on papi timing (see below).

- MINI_DATASET, SMALL_DATASET, STANDARD_DATASET, LARGE_DATASET,
  EXTRALARGE_DATASET: set the dataset size to be used
  [default: STANDARD_DATASET]

- POLYBENCH_USE_C99_PROTO: Use standard C99 prototype for the functions.

- POLYBENCH_USE_SCALAR_LB: Use scalar loop bounds instead of parametric ones.

<br>
<h3>PAPI support:</h3>

* To compile a benchmark with PAPI support:

$> gcc -O3 -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -DPOLYBENCH_PAPI -lpapi -o atax_papi

* To specify which counter(s) to monitor:

Edit utilities/papi_counters.list, and add 1 line per event to
monitor. Each line (including the last one) must finish with a ',' and
both native and standard events are supported.

The whole kernel is run one time per counter (no multiplexing) and
there is no sampling being used for the counter value.



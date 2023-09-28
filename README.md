# 2d-seidel-computation
work of concurrent programming that consists in parallelizing 2d Seidel algorithm

Alunos:
Arthur Maciel   120119
Gabriel Biscaia 118928
Pedro Zafalon   120117

* * * * * * * * * *
* PolyBench/C 3.2 *
* * * * * * * * * *

Copyright (c) 2011-2012 the Ohio State University.
Contact: Louis-Noel Pouchet <pouchet@cse.ohio-state.edu>



-----------------------
* Available benchmarks:
-----------------------

stencils/seidel-2d_sequential.c
stencils/seidel-2d_pthreads.c
stencils/seidel-2d_mpi.c


------------------------------
* Sample compilation commands:
------------------------------


** To compile a benchmark without any monitoring:
-------------------------------------------------

$> gcc -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -o atax_base


** To compile a benchmark with execution time reporting:
--------------------------------------------------------

$> gcc -O3 -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -DPOLYBENCH_TIME -o atax_time


** To generate the reference output of a benchmark:
---------------------------------------------------

$> gcc -O0 -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -DPOLYBENCH_DUMP_ARRAYS -o atax_ref
$> ./atax_ref 2>atax_ref.out



-------------------------
* Some available options:
-------------------------

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



---------------
* PAPI support:
---------------

** To compile a benchmark with PAPI support:
--------------------------------------------

$> gcc -O3 -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -DPOLYBENCH_PAPI -lpapi -o atax_papi


** To specify which counter(s) to monitor:
------------------------------------------

Edit utilities/papi_counters.list, and add 1 line per event to
monitor. Each line (including the last one) must finish with a ',' and
both native and standard events are supported.

The whole kernel is run one time per counter (no multiplexing) and
there is no sampling being used for the counter value.



------------------------------
* Accurate performance timing:
------------------------------

With kernels that have an execution time in the orders of a few tens
of milliseconds, it is critical to validate any performance number by
repeating several times the experiment. A companion script is
available to perform reasonable performance measurement of a PolyBench.

$> gcc -O3 -I utilities -I linear-algebra/kernels/atax utilities/polybench.c linear-algebra/kernels/atax/atax.c -DPOLYBENCH_TIME -o atax_time
$> ./utilities/time_benchmark.sh ./atax_time

This script will run five times the benchmark (that must be a
PolyBench compiled with -DPOLYBENCH_TIME), eliminate the two extremal
times, and check that the deviation of the three remaining does not
exceed a given threshold, set to 5%.

It is also possible to use POLYBENCH_CYCLE_ACCURATE_TIMER to use the
Time Stamp Counter instead of gettimeofday() to monitor the number of
elapsed cycles.




----------------------------------------
* Generating macro-free benchmark suite:
----------------------------------------

(from the root of the archive:)
$> PARGS="-I utilities -DPOLYBENCH_TIME";
$> for i in `cat utilities/benchmark_list`; do create_cpped_version.sh $i "$PARGS"; done

This create for each benchmark file 'xxx.c' a new file
'xxx.preproc.c'. The PARGS variable in the above example can be set to
the desired configuration, for instance to create a full C99 version
(parametric arrays):

$> PARGS="-I utilities -DPOLYBENCH_USE_C99_PROTO";
$> for i in `cat utilities/benchmark_list`; do ./utilities/create_cpped_version.sh "$i" "$PARGS"; done



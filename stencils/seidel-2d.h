#ifndef SEIDEL_2D_H
# define SEIDEL_2D_H

/* Default to STANDARD_DATASET. */
# if !defined(SMALL_DATASET) && !defined(LARGE_DATASET)
#   define STANDARD2_DATASET
# endif

/* Do not define anything if the user manually defines the size. */
# if !defined(TSTEPS) && ! defined(N)
/* Define the possible dataset sizes. */
#  ifdef SMALL_DATASET
#   define TSTEPS 1200
#   define N 6000
#  endif

#  ifdef STANDARD_DATASET
#   define TSTEPS 1700
#   define N 6000
#  endif

#  ifdef LARGE_DATASET /* Default if unspecified. */
#   define TSTEPS 2200
#   define N 6000
#  endif

# endif /* !N */

# define _PB_TSTEPS POLYBENCH_LOOP_BOUND(TSTEPS,tsteps)
# define _PB_N POLYBENCH_LOOP_BOUND(N,n)

# ifndef DATA_TYPE
#  define DATA_TYPE double
#  define DATA_PRINTF_MODIFIER "%0.2lf "
# endif


#endif /* !SEIDEL_2D */

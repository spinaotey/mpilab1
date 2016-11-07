#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


#define sqr(x) ((x)*(x))
#define ndarts     50000
#define rounds     100

/*  Dboard
 * 
 *  Computes an approximation of pi by throwing darts to a board and
 *  looking how many are inside the circle of radius 1.
 *
 *  Variables:
 *      -darts = Number of darts to throw for the approximaton
 */

double dboard(int darts);


/*  Computes an approximation of pi by throwing ndarts in parallel
 *  to different boards, and averages the resulint pies. This is
 *  done for many rounds.
 */

int main(int argc, char *argv[]){
    /* VARIABLES */
    int i;              // Dummy variable
    int err;            // Error handling
    int rank, size;     // MPI variables
    double pi,result;   // Pi approximations and final result
    
    /* INTIATE PARALLELIZATION AND RANDOM */
    err = MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    srandom(((int) time(NULL)) +rank); // A different random seed for each process


    /* MAIN PROGRAM */

    for(i=0; i<rounds; i++){
        pi = dboard(ndarts);
        // Sum over all the pi's obtained and send it to the master
        MPI_Reduce(&pi, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        // Master computes average and prints it out
        if(rank == 0){
            result /= size;
            printf("π = %g\n",result);
        }
    }

    /* FINISH PARALLELIZATION */
    err = MPI_Finalize();

    return 0;
    
}



double dboard(int darts){
    
    /* VARIABLES */
    double x_coord, y_coord, pi, r;
    int score, n;
    
    score = 0;
    /* "throw darts at board" */
    for (n = 1; n <= darts; n++) {
        /* generate random numbers for x and y coordinates */
        r = (double)random()/RAND_MAX;
        x_coord = (2.0 * r) - 1.0;
        r = (double)random()/RAND_MAX;
        y_coord = (2.0 * r) - 1.0;
        /* if dart lands in circle, increment score */
        if ((sqr(x_coord) + sqr(y_coord)) <= 1.0)
            score++;
    }
    /* calculate pi */
    pi = 4.0 * (double)score/(double)darts;
    return(pi);
}

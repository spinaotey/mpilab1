#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


#define sqr(x) ((x)*(x))

/*  Dboard
 * 
 *  Computes an approximation of pi by throwing darts to a board and
 *  looking how many are inside the circle of radius 1.
 *
 *  Variables:
 *      -darts = Number of darts to throw for the approximaton
 */

unsigned dboard(unsigned darts);


/*  Computes an approximation of pi by throwing ndarts in parallel
 *  to different boards, and averages the resuling π's. This is
 *  done for many rounds.
 */

int main(int argc, char *argv[]){
    /* VARIABLES */
    int i;              // Dummy variable
    int err;            // Error handling
    int rank, size;     // MPI variables
    int master = 0;
    unsigned aux, succ_darts;
    int ndarts;
    double pi;   // Pi approximations and final result
    
    if(sscanf(argv[1],"%d",&ndarts)!= 1){
        fprintf(stderr,"Error. Correct way of executing: ./%s ndarts\n",argv[0]);
        return -1;
    }

    /* INTIATE PARALLELIZATION AND RANDOM */
    err = MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    srandom(((int) time(NULL)) +rank); // A different random seed for each process


    /* MAIN PROGRAM */

    succ_darts = dboard(ndarts/size + 1 * (rank < (ndarts % size)));
    // Sum over all the successes obtained and send it to the master
    MPI_Reduce(&succ_darts, &aux, 1, MPI_UNSIGNED, MPI_SUM, master, MPI_COMM_WORLD);
    // Master computes average and prints it out
    if(rank == master){
        pi = 4.*(double) aux /(double)(ndarts);
        printf("π = %g\n",pi);
    }

    /* FINISH PARALLELIZATION */
    err = MPI_Finalize();

    return 0;
    
}



unsigned dboard(unsigned darts){
    
    /* VARIABLES */
    double x_coord, y_coord, pi, r;
    unsigned score, n;
    
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

    return score;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>


#define sqr(x) ((x)*(x))
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
 *  to different boards, and averages the resulting π's. This is
 *  done for many rounds.
 */

int main(int argc, char *argv[]){
    /* VARIABLES */
    int i,j;              // Dummy variable
    int err;            // Error handling
    int rank, size;     // MPI variables
    int master = 0, masterTag = 1<<10;
    int ndarts;
    MPI_Status status;
    double pi,aux;   // Pi approximations and final result
    
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

    for(i=0; i<rounds; i++){
        pi = dboard(ndarts);
        if(rank != master)
            MPI_Send(&pi,1,MPI_DOUBLE,master,masterTag,MPI_COMM_WORLD);
        // Master computes average and prints it out
        if(rank == master){
            for(j=1; j<size; j++){
                MPI_Recv(&aux,1,MPI_DOUBLE,j,masterTag,MPI_COMM_WORLD,&status);
                pi += aux;
            }
            pi /= size;
            printf("π = %g\n",pi);
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

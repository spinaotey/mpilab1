#include <stdio.h>
#include <stdlib.h>

#define sqr(x) ((x)*(x))
#define ndarts     50000
#define rounds     100


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



int main(void){
    int i;
    int rank;
    double pi = 0;
    
    srandom(0);

    for(i=0; i<rounds; i++){
        pi = dboard(ndarts);
        printf("π = %g\n",pi);
    }

    return 0;
    
}

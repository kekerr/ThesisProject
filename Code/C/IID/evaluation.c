#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structures.h"
#include "iid.h"
#include "printing.h"
#include "evaluation.h"

extern int IHEIGHT;
extern int IWIDTH;

//calculates the MSE between two arrays of identical height and width
double calcMSE(int array1[IHEIGHT][IWIDTH], int array2[IHEIGHT][IWIDTH], int height, int width){
    int i,j=0;
    double sum=0;
    double mse;
    double diff;

    for(i=0;i<height;i++){
        for(j=0;j<width;j++){
            diff= array1[i][j]-array2[i][j];
            sum = diff*diff + sum;
        }
    }
    mse=sum/(width*height);
    return mse;

}

//calculates the PSNR value between two arrays of identical height and width
double getPSNR(int array1[IHEIGHT][IWIDTH], int array2[IHEIGHT][IWIDTH], int height, int width){
    double mse, psnr;
    mse=calcMSE(array1, array2, height, width);
    printf("MSE = %2.2f \n", mse);
    if(mse == 0) {
        psnr = 20*log10(255);
    } else {
        psnr = 20*log10(255);
        psnr = psnr - 10*log10(mse);
    }
    printf("PSNR = %f\n", psnr);
    return psnr;

}

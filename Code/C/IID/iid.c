#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structures.h"
#include "iid.h"
#include "printing.h"

extern int BINSIZE;
extern int QUANT;
extern int MAXMERGE;
extern int IHEIGHT;
extern int IWIDTH;

//calculates the empirical distribution using information from the atom
double *calcEmpiricalDist(int x, int y, int height, int width, int pixels[IHEIGHT][IWIDTH], double *empirDist){
	int pix;
	int bin;
	int i,j;
	double delta = 256/QUANT;
	//Quantization Code for grey scale:
	for(i=y; i<(y+height); i++) {
		for (j=x; j<(x+width); j++) {
			pix = pixels[i][j];
			bin = floor(pix/delta);
			empirDist[bin]++;
		}
	}

	for(i=0; i<QUANT; i++) {
		empirDist[i] = empirDist[i] / (height*width);
	}
	return empirDist;
}

//calculates the merging criterion between two empirical distributions
double calcMergingCriterion(double *empirptr, double n, double *nempirptr, double nn){
	double mergingCriterion;
	double jointEmpir[QUANT];
	double dempir;
	double dnempir;
	double sum = n + nn;
	double check;
	int i;

	for(i=0; i<QUANT; i++){
		jointEmpir[i] = (n/sum)*empirptr[i] + (nn/sum)*nempirptr[i];
	}
	dempir = KLDivergence(empirptr, jointEmpir);
	dnempir = KLDivergence(nempirptr, jointEmpir);
	mergingCriterion = n*dempir + nn*dnempir;
	return mergingCriterion;
}

//calculates the Divergence between two distributions
double KLDivergence(double *empirptr, double *jointptr) {
	double kldivergence = 0;
	int i;
	for(i=0; i<QUANT; i++) {
		//can't divide by zero!
		if(jointptr[i] < 0.0001 && empirptr[i] < 0.001){
			//do nothing
		}else {
			if(empirptr[i] == 0) {
				//do nothing
			} else {
				kldivergence = kldivergence + (empirptr[i]*log2(empirptr[i]/jointptr[i]));
			}
		}
	}
	return kldivergence;
}

//finds the next best region to merge within the whole image
int findNextBestRegion(Imageptr image) {
	int numAtomsWidth = image->numAtomsWidth;
	int numAtomsHeight = image->numAtomsHeight;
	int numAtoms = numAtomsHeight*numAtomsWidth;
	int *currentRegions = image->currentRegionID;
	Regionptr r;

	int minMC = MAXMERGE;
	int minMCRegion = 0;
	int tempMC;

	int i;
	for(i=0; i<numAtoms; i++){
		if(currentRegions[i] == 0){ //checking to see if this is a region in the neighbour
			r = image->regions[i];
			tempMC = r->bestMergeCriterion;
			if(tempMC< minMC) {
				minMC = tempMC;
				minMCRegion = i;
			}
		}
	}
	return minMCRegion;
}

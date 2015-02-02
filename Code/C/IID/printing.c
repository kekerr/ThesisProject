#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "printing.h"

//prints a int 2D array
void printI2D(int **A, int width, int height) {
	int i,j;
	for(i = 0; i < height; i++) {
		for(j = 0; j< width; j++) {
			printf("%d   ", A[i][j]);
		}
		printf("\n");
	}
}

//prints a double 1D array
void printD1D(double *A, int size) {
	int i;
	for(i=0; i<size; i++) {
		printf("%f \t", A[i]);
	}
	printf("\n");
}

//prints a int 1D array
void printI1D(int *A, int size) {
	int i;
	for(i=0; i<size; i++) {
		printf("%d   ", A[i]);
	}
	printf("\n");
}

//prints neighbours from a region
void printNeighbours(Regionptr r1) {
	printf("Printing Neighbour's for Region %d \n", r1->regionID);
	Neighbourptr n = r1->headNeighbour;
	while(n!=NULL) {
		printf("\t %d (%d)", n->regionID, n->mergingCriterion);
		n = n->nextNeighbour;
	}
	printf("\n");
}

//prints all remain regions
void printRegions(int *remainingRegions, int numAtoms, Imageptr image){
	int i;
	Regionptr r;
	int n;
	double *empir;
	printf("Printing Regions & Empirical Distributions\n");
	for(i=0; i<numAtoms; i++) {
		if(remainingRegions[i] == 0) {
			printf("Region %d \n", i);
			r = image->regions[i];
			empir = r->empiricalDist;
			n = r->numPixels;
			printf("numPixels: %d\n", n);
			printf("Empirical: \n");
			printD1D(empir,2);
			printf("Neighbours:\n");
			printNeighbours(r);
		}
	}
}
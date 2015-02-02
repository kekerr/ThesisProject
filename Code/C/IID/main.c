#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "iid.h"
#include "printing.h"
#include "IO.h"
#include "merge.h"
#include "evaluation.h"
/* As mentioned in the ReadMe.txt 
The code can either stop merging when:
1. The image has been merged a specific number of times, or
2. The stopping criterion are met (aka a maximum merging cost
or there are only three remaining regions in the image).

The code is automatically set to run with case 2. To implement
case 1, the while loop on line SET is changed. 
*/

/* PARAMS TO SET BEFORE MERGING */
int IWIDTH = 481;			//image pixel width
int IHEIGHT = 321;			//image pixel height
int BINSIZE = 5;			//the ideal atom or initial partition width/height
int QUANT = 10;				//the number of quantization bins
int MCThreshould = 5000;	//minimum MC value for a merge to occur
int MAXMERGE = 10000000;	//the maximum possible Merging Criterion
int NUMMERGES= 500;			//the number of merges the algorithm will perform
char FILENAME[] = "bird_output.txt";		// the input file name
char OUTPUTFILENAME[] = "bird_merged.txt";	// the desired output file name

/* METHODS */
int calcNumAtoms(int pixels);  	//calculates the number of atoms along the width of the image

/* MAIN CODE */
int main(){
	//fills array with pixel values
	int pixels[IHEIGHT][IWIDTH];
	readFile(IWIDTH, IHEIGHT, FILENAME, pixels);

	//copes the pixel array for use in calculating the PSNR value 
	int originalImage[IHEIGHT][IWIDTH];
	int i,j;
	for(i=0; i<IHEIGHT; i++){
		for(j=0; j<IWIDTH; j++) {
			originalImage[i][j]=pixels[i][j];
		}
	}
	
	//Initializes the Image Structure
	Imageptr image = malloc(sizeof(Image));
	int NumAtomsWidth = calcNumAtoms(IWIDTH);
	int NumAtomsHeight = calcNumAtoms(IHEIGHT);
	int NumAtoms = NumAtomsWidth*NumAtomsHeight;
	image->numAtomsWidth = NumAtomsWidth;
	image->numAtomsHeight = NumAtomsHeight;
	image->regions = malloc(sizeof(Regionptr)*(NumAtoms));
	int CurrentRegions[NumAtoms];
	for(i=0; i<NumAtoms; i++) {
		CurrentRegions[i] = 0;
	}
	int *CurrentRegionIDptr = &CurrentRegions[0];
	image->currentRegionID = CurrentRegionIDptr;

	//Initializes all Atoms, Regions and Neighbour list for merging
	printf("Number of Atoms (%d, %d)\n", NumAtomsHeight, NumAtomsWidth);
	int aregionID = 0;
	int aheight;
	int awidth;
	for(i=0; i<(image->numAtomsHeight); i++) {
		for(j=0; j<(image->numAtomsWidth); j++) {
			//initialize atom:
			Atomptr a = aalloc();
			a->x = BINSIZE*j;
			a->y = BINSIZE*i;
			if((aregionID % NumAtomsWidth) == (NumAtomsWidth - 1)){
				awidth = BINSIZE + (IWIDTH % BINSIZE);
			} else {
				awidth = BINSIZE;
			}
			if(aregionID >= (NumAtoms - NumAtomsWidth)){
				aheight = BINSIZE + (IHEIGHT % BINSIZE);
			} else {
				aheight = BINSIZE;
			}
			a->width = awidth;
			a->height = aheight;
			a->nextAtom = NULL;
			a->regionID = aregionID;

			//initialize region:
			Regionptr r = ralloc();
			r->regionID = a->regionID;
			r->numPixels = (a->width) * (a->height);
			r->numAtoms = 1;
			r->headAtom = a;
			double *pEmpir = malloc(sizeof(double)*QUANT);
			pEmpir = calcEmpiricalDist(a->x, a->y, a->height, a->width, pixels, pEmpir);
			r->empiricalDist = pEmpir;

			//initialize neighbour:
			Neighbourptr n = initNeigh(r, n, image->numAtomsWidth, image->numAtomsHeight);
			r->headNeighbour = n;

			//add region into image
			image->regions[aregionID] = r;
			aregionID++;
		}
	}	

	//Calculates the merging criterion for all neighbouring regions
	int bestOverallRegionMerge = MAXMERGE;
	int bestOverallRegionID;
	for(i=0; i<(NumAtomsWidth*NumAtomsHeight); i++) {
		//variables for region
		Regionptr r = image->regions[i];
		double *empirptr = r->empiricalDist;
		int regionSize = r->numPixels;

		//variables for neighbour
		Neighbourptr neighbour = r->headNeighbour;
		int NRegionID;
		double *nempirptr;
		int neighbourSize;

		//variables for merging criterion
		int bestMergeCriterion = MAXMERGE;
		int bestMergeRegion = 0;
		double MergingCriterion;

		//loop through all neighbours for a given region r
		while(neighbour!=NULL) {
			NRegionID = neighbour->regionID;
			nempirptr = image->regions[NRegionID]->empiricalDist;
			neighbourSize = image->regions[NRegionID]->numPixels;
			MergingCriterion = calcMergingCriterion(empirptr, regionSize, nempirptr, neighbourSize);

			//set neighbour's merging criterion
			neighbour->mergingCriterion = MergingCriterion;

			//set's if it's the best so far
			if(bestMergeCriterion > MergingCriterion) {
				bestMergeCriterion = MergingCriterion;
				bestMergeRegion = NRegionID;
			}

			//go to next neighbour:
			neighbour = neighbour->nextNeighbour;
		}

		r->bestNeighbour = bestMergeRegion;
		r->bestMergeCriterion = bestMergeCriterion;

		// image best merge: 
		if(bestMergeCriterion < bestOverallRegionMerge) {
			bestOverallRegionMerge = bestMergeCriterion;
			bestOverallRegionID = r->regionID;
		}
	}

	//variables necessary for merging
	image->bestMergeRegionID = bestOverallRegionID;
	int numRegions = NumAtoms;
	int *currentRegions = image->currentRegionID;
	Regionptr r1;
	Regionptr r2;
	int bestRegion;
	int NeighbourID;
	Regionptr bestr;
	double bestMC;
	i=0;
	int numMerge = 0;

	//CHANGE WHICH COMMENTED OUT WHILE LOOP FOR STOPPING 
	//  FOR REASONS 1 & 2 MENTIONED AT TOP

	while(numRegions>5 && bestMC <= MCThreshould) {
//	while(numMerge<NUMMERGES) {

		//merge r1 and r2
		bestRegion = image->bestMergeRegionID;
		r1 = image->regions[bestRegion];
		NeighbourID = r1->bestNeighbour;
		r2 = image->regions[NeighbourID];
		r1 = Mergeregions(r1, r2, image);

		//update image and best region for merging
		if(bestRegion < NeighbourID) {
			image->regions[bestRegion] = r1;
			image->regions[NeighbourID] = NULL;
			currentRegions[NeighbourID] = -1;
		} else {
			image->regions[NeighbourID] = r1;
			image->regions[bestRegion] = NULL;
			currentRegions[bestRegion] = -1;
		}

		//find the next best merge
		bestRegion = findNextBestRegion(image);
		image->bestMergeRegionID = bestRegion;
		bestr = image->regions[bestRegion];
		bestMC = bestr->bestMergeCriterion;
		numRegions = countCurrentRegions(currentRegions, NumAtoms);
		i++;
		numMerge++;
	}
	//Gives all regions the same pixel values
	printf("Best Merging Criterion: %f\n", bestMC);
	printf("Num Merges: %d\n", numMerge);

	//Change the pixel values to represent the merged regions
	colourRegions(pixels, image);

	//Writes the pixels into a file	
	writeFile(pixels, IHEIGHT, IWIDTH, OUTPUTFILENAME);

	//calculates the PSNR 
	double PSNR = getPSNR(originalImage, pixels, IHEIGHT, IWIDTH);
}


/* calcNumAtomsWidth: calculates the number of atoms along width of image */
int calcNumAtoms(int pixels) {
	return pixels/BINSIZE;
}

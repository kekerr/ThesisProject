//This is to read pixel values from sample.txt file and store into 2d array.
//you can modify this if you want to.
//I referenced last year's project to do this.
#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "IO.h"

extern int IHEIGHT;
extern int IWIDTH;

//colours the regions of the image
void colourRegions(int pixels[IHEIGHT][IWIDTH], Imageptr image) {
	int *remainingRegions = image->currentRegionID;
	int numAtomsWidth = image->numAtomsWidth;
	int numAtomsHeight = image->numAtomsHeight;
	int numAtoms = numAtomsHeight*numAtomsWidth;

	Regionptr r;
	int numRegions = countCurrentRegions(remainingRegions, numAtoms);
	printf("REGIONS REMAINING: %d \n", numRegions);
	int value;
	int i;
	int j = 0;
	//for every region, give it a new pixel value
	for(i=0; i<numAtoms; i++) {
		if(remainingRegions[i] == 0) {//region remains after merging
			r = image->regions[i];
			value = calculateAverage(r, pixels);
			alterPixels(r, pixels, value);
		}
	}
}

//calculates the average pixel value within a region
int calculateAverage(Regionptr r, int pixels[IHEIGHT][IWIDTH]) {
	Atomptr a = r->headAtom;
	int x = a->x;
	int y = a->y;
	int height = a->height;
	int width = a->width;
	int i, j;
	int n =0;
	int sum = 0;
	int value;

	while(a != NULL) {
		x = a->x;
		y = a->y;
		height = a->height;
		width = a->width;

		for(i=y; i<(y+height); i++) {
			for(j=x; j<(x+width); j++){
				sum = sum + pixels[i][j]; 
			}
		}
		n = n + (height*width);
		a = a->nextAtom;
	}
	value = sum/n;
	return value;
}

//changes pixel values within a region in the original pixel array 
// according to the pixValue
void alterPixels(Regionptr r, int pixels[IHEIGHT][IWIDTH], int pixValue) {
	Atomptr a = r->headAtom;
	int x = a->x;
	int y = a->y;
	int height = a->height;
	int width = a->width;
	int i, j;
	int count =0;

	while(a != NULL) {
		x = a->x;
		y = a->y;
		height = a->height;
		width = a->width;

		for(i=y; i<(y+height); i++) {
			for(j=x; j<(x+width); j++){
				pixels[i][j] = pixValue;
			}
		}
		a = a->nextAtom;
		count++;
	}
}

//counts the number of remaining regions within an image
int countCurrentRegions(int *remainingRegions, int numAtoms) {
	int sum =0;
	int i;
	for(i=0; i<numAtoms; i++) {
		if(remainingRegions[i] == 0){ //region still remains
			sum++;
		}
	}
	return sum;
}

//New readfile
void readFile(int width, int height, char *fileName, int pixels[IHEIGHT][IWIDTH]){
	int ret;
	int i,j;

	FILE *fp = fopen(fileName,"rt");
	if(fp==NULL){
        printf("cannot open file \n");
	}

	for(i = 0; i<height;i++){
		for(j=0;j<width;j++){
		ret=fscanf(fp,"%d",&pixels[i][j]);
        /*printf("%d \n",array2d[i][j]);*/
		if(ret==EOF)
			break; //when it reached end of file, break out from the loop.
		}

	}
	fclose(fp);
}

//writes a file with the array values
void writeFile(int outputPixels[IHEIGHT][IWIDTH], int height, int width, char *outputFileName){
	int i,j;
	FILE *fp = fopen(outputFileName, "wb");
	for(i=0; i<height; i++) {
		for(j=0; j<width; j++){
			fprintf(fp, "%d", outputPixels[i][j]);
			fprintf(fp, "%s", "\t");
		}
		fprintf(fp, "%s", "\n");
	}
	fclose(fp);
}

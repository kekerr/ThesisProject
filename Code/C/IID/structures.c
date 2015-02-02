#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "iid.h"

extern int BINSIZE;

//creates a neighbour
Neighbourptr initNeigh(Regionptr r, Neighbourptr n, int numAtomsWidth, int numAtomsHeight) {
	int RRegionID = r->regionID;
	int NRegionID;
	int numAtoms = numAtomsWidth*numAtomsHeight;

	//create head neighbour
	n = nalloc();
	Neighbourptr head = n;

	/* Finds all neighbours for a region and puts them into a linked list*/
	//if necessary, create above neighbour 
	if(RRegionID >= numAtomsWidth){
		NRegionID = RRegionID - numAtomsWidth;
		n = createNeigh(n, NRegionID);
	}
	//if necessary, create below neighbour
	if(RRegionID < (numAtoms-numAtomsWidth)){
		NRegionID = RRegionID + numAtomsWidth;
		n = createNeigh(n, NRegionID);
	}
	//if necessary, create left neighbour
	if((RRegionID % numAtomsWidth) != 0) {
		NRegionID = RRegionID - 1;
		n = createNeigh(n, NRegionID);
	}
	//if necessary, create right neighbour
	if((RRegionID % numAtomsWidth) != (numAtomsWidth -1)){
		NRegionID = RRegionID + 1;
		n = createNeigh(n, NRegionID);
	}

	//remove the last neighbour (it's unnecessarily created)
	n = head;
	while(n->nextNeighbour->nextNeighbour != NULL) {
		n = n->nextNeighbour;
	}
	free(n->nextNeighbour);
	n->nextNeighbour = NULL;


	return head;
}

//creating a new neighbour at n->nextNeighbour
Neighbourptr createNeigh(Neighbourptr n, int RegionID) {
	if(n==NULL) {
		return 0;
	}
	Neighbourptr newN = nalloc();
	n->regionID = RegionID;
	n->mergingCriterion = 0;
	n->nextNeighbour = newN;
	return newN;
}

/* aalloc: make a atom */
Atomptr aalloc(void) {
	return (Atomptr) malloc(sizeof(Atom));
}

/* ralloc: make a region */
Regionptr ralloc(void) {
	return(Regionptr) malloc(sizeof(Region));
}

/* nalloc: make a neighbour list */
Neighbourptr nalloc(void) {
	return(Neighbourptr) malloc(sizeof(Neighbour));
}

void rfree(Regionptr r){
	free(r->empiricalDist);
	free(r);
}

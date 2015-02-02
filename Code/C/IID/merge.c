#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "structures.h"
#include "iid.h"
#include "printing.h"
#include "merge.h"

extern int QUANT;

//updates the atom regions to the new region ID
void updateAtomRegIDs(int ID, Atomptr a){
	while(a!=NULL){
		a->regionID = ID;
		a = a->nextAtom;
	}//end of while loop	
}

//This function will remove a region from another region's neighbourlist
void removeRegLink(int rID, Regionptr r2){
	Neighbourptr n = r2->headNeighbour;
	Neighbourptr before = NULL;
	int nID;
	int count =0;
	int removed = 0;
	while(n!=NULL && removed != 1) {
		nID = n->regionID;
		if(nID == rID) {
			//remove the neighbour!
			if(count ==0 ) { //change the head neighbour
				r2->headNeighbour = n->nextNeighbour;
				free(n);
				removed = 1;
			} else {
				before->nextNeighbour = n->nextNeighbour;
				free(n);
				removed = 1;
			}
		}
		if(removed!=1) {
			before = n;
			n= n->nextNeighbour;
		}
		count++;
	}
} //end of function removeRegLink
					
//This function will find the last neighbour in the neighbourlist
Neighbourptr findlastneighbour(Neighbourptr head){
	Neighbourptr current=head;
	while(current->nextNeighbour!=NULL){
		current=current->nextNeighbour;
	}
	return current;
	}

//this function will merge two neighbour lists
void mergeNeighborLists(Regionptr r1, Regionptr r2) {
	//remove r1 from f2 neighbour list
	//then remove r1 from r2 neighbour list
	removeRegLink(r2->regionID, r1);
	removeRegLink(r1->regionID, r2);

	//Now add r2 neighbour lists to r1's neighbourlist
	Neighbourptr currentr2=r2->headNeighbour;
	if(r1->headNeighbour!=NULL) {
		Neighbourptr lastr1=findlastneighbour(r1->headNeighbour);
		Neighbourptr headr1=r1->headNeighbour;
		int N2inN1;	

		while(currentr2!=NULL){
			//check first if currentr2 is already in r1 neighbourlist
			N2inN1= 0;
			Neighbourptr currentr1 = headr1; 
			while(currentr1!=NULL){
				//checks to see if N2 is already in N1
				if(currentr1->regionID==currentr2->regionID){
					N2inN1 = 1;
				}
				currentr1=currentr1->nextNeighbour;
			}//end of inner while
			//if threre is a duplicate remove from r2 list
			if(N2inN1 > 0) {
				removeRegLink(currentr2->regionID, r2);
			}
			currentr2=currentr2->nextNeighbour;
		}//end of outer while
		lastr1->nextNeighbour = r2->headNeighbour;
		//The new neighbour list for our merged region!
	} else {
		r1->headNeighbour = r2->headNeighbour;
	}
}

//updates the empirical distribution, number of pixels and number of atoms
// for the new region r1
void updateR1Values(Regionptr r1, Regionptr r2) {
	int i;
	double *empir1 = r1->empiricalDist;
	double *empir2 = r2->empiricalDist;
	int size1 = r1->numPixels;
	int size2 = r2->numPixels;
	double total = size1+size2;
	for(i=0;i<QUANT;i++){
		empir1[i]=(empir1[i]*size1+empir2[i]*size2)/total;
	}
	//also update numatoms, numpixels & empiricalDist
	r1->numPixels=total;
	r1->numAtoms=(r1->numAtoms)+(r2->numAtoms);
	r1->empiricalDist = empir1;
}

//merges two regions r1 and r2	
Regionptr Mergeregions(Regionptr r1, Regionptr r2, Imageptr image) {
	//Want to keep the lower region ID, so we switch them if necessary
	if(r1->regionID > r2->regionID){
		Regionptr temp = r1;
		r1 = r2;
		r2 = temp;
	}
	//update region IDs for atoms in merged regions
	//input will be r1->regionID and r2 head atom
	updateAtomRegIDs(r1->regionID, r2->headAtom);
	//Now we need to update neighbor lists for a merged region
	mergeNeighborLists(r1,r2);
	//Now change the distribution of region 1 to merged region
	updateR1Values(r1, r2);
 	//Now we update neighbours of r1
 	updateAllNeighbours(r1, r2, image);

 	//connect the atom list of r2 to r1
 	updateAtomList(r1, r2);
 	
 	//return our new merged region r1
 	return r1;
 }

//adds two atom lists together
 void updateAtomList(Regionptr r1, Regionptr r2) {
 	Atomptr a = r1->headAtom;
 	while(a->nextAtom != NULL) {
 		a = a->nextAtom;
 	}
 	a->nextAtom = r2->headAtom;
 }

//looks at all of the neighbours of the new merged region r1
 void updateAllNeighbours(Regionptr r1, Regionptr r2, Imageptr image){
 	//The neighbours to be changed:  
 	Neighbourptr r1n = r1->headNeighbour;	
 	//Recalculates all the Merging Criterions!
 	Regionptr nr;	//each neighbour's region
 	Neighbourptr nn; //each neighbour's neighbour
 	int nID;
 	double MC;

 	while(r1n!=NULL) {
 		nID = r1n->regionID;		//neighbour regionID
 		nr = image->regions[nID];	//neighbour region
 		nn = nr -> headNeighbour;	//neighbour's neighbour list

 		//rename r2 neighbour region to r1 region:
 		renameNeighbourRegions(r1->regionID, r2->regionID, nn);

 		//remove duplicate r1 regions if neccessary:
 		removeDuplicate(nn, r1->regionID);

 		//update the merging criterion for r1 neighbour and bestMerge Criterion
 		updateR1Neighbour(nn, nr, r1, r1n);
 		r1n = r1n->nextNeighbour;
 	}
 	//update best merge criterion for r1
 	updateBestMergingCriterion(r1);
 }

//renames all of region r2 neighbours to r1
void renameNeighbourRegions(int r1RegionID, int r2RegionID, Neighbourptr headNeighbour) {
	Neighbourptr current = headNeighbour;
	while(current!=NULL) {
		if(current->regionID == r2RegionID) {
			current->regionID = r1RegionID;
		}
		current = current->nextNeighbour;
	}
}

//deletes duplicate neighbours within the region
void removeDuplicate(Neighbourptr headNeighbour, int r1RegionID){
	int count = 0;
	Neighbourptr current = headNeighbour;
	Neighbourptr before = headNeighbour;
	while(current!=NULL) {
		//if neighbour id is the same as r1 id:
		if(current->regionID == r1RegionID){
			//if there are two neighbours with r1 id, remove second one
			if(count>0) {
				if(current->nextNeighbour==NULL){
					free(current);
					before->nextNeighbour = NULL;
				} else {
					before->nextNeighbour = current->nextNeighbour;
				}
			}
			count++;
		}
		before = current;
		current = current->nextNeighbour;
	}
}

//updates the merging criterion
void updateR1Neighbour(Neighbourptr headNeighbour, Regionptr nr, Regionptr r1, Neighbourptr r1n) {
 	//number of pixels for region, neighbour
 	int r1numPix = r1->numPixels; 
 	int nrnumPix = nr->numPixels;
 	double MC;
 	Neighbourptr nr1 = headNeighbour;
 	int done = 0;
 	while(nr1 != NULL && done == 0) {
 		if(nr1->regionID == r1->regionID){
 			//change the merging criterion:
 			MC = calcMergingCriterion(r1->empiricalDist, r1numPix, nr->empiricalDist, nrnumPix);
 			nr1->mergingCriterion = MC;
 			r1n->mergingCriterion = MC;
 			//change the regions best merging criterion 
 			updateBestMergingCriterion(nr);
 			done = 1;
 		}
 		nr1 = nr1->nextNeighbour;
 	}
 	
}

//updates the best merging criterion
void updateBestMergingCriterion(Regionptr r){
	double minMC = 100000000;
 	int minMCNeigh = -1;
 	Neighbourptr current = r->headNeighbour;
 	while(current!=NULL) {
 		if(current->mergingCriterion < minMC) {
 			minMC = current->mergingCriterion;
 			minMCNeigh = current->regionID;
 		}
 		current = current->nextNeighbour;
 	}
 	r->bestNeighbour = minMCNeigh;
 	r->bestMergeCriterion = minMC;
}


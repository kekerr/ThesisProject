
typedef struct atom *Atomptr;
typedef struct region *Regionptr;
typedef struct neighbour *Neighbourptr;
typedef struct image *Imageptr;

typedef struct atom {
	int height;				// number of pixels in height 
	int width;				// number of pixels in width 
	int x;					// x co-ordinate
	int y; 					// y co-ordinate
	struct atom *nextAtom;	// links to next atom in same region 
	int regionID;			// current region ID 
} Atom;

typedef struct region {
	int regionID;
	int numPixels;
	int numAtoms;
	int bestNeighbour;
	double bestMergeCriterion;
	double *empiricalDist;
	struct atom *headAtom;
	struct neighbour *headNeighbour;
}Region;

typedef struct neighbour {
	struct neighbour *nextNeighbour;
	int regionID;
	int mergingCriterion;
}Neighbour;

typedef struct image {
	struct region **regions;
	int bestMergeRegionID;
	int numAtomsWidth;
	int numAtomsHeight;
	int *currentRegionID;
}Image;

/* MEMORY ALLOCATION */ 
Atomptr aalloc(void);  	//assigns memory for an atom
Regionptr ralloc(void);  	//assigns memory for a region
Neighbourptr nalloc(void); //assigns memory for a neighbour

/* MEMORY FREEING */
void rfree(Regionptr r);

/* NEIGHBOUR FUNCTIONS */
Neighbourptr initNeigh(Regionptr r, Neighbourptr n, int numAtomsWidth, int numAtomsHeight); 
Neighbourptr createNeigh(Neighbourptr n, int RegionID);

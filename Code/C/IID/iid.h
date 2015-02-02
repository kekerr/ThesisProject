extern int IWIDTH;
extern int IHEIGHT;

/* All Functions pertaining to calculating according to IID model */

//calculating merging criterion for IID Model
double *calcEmpiricalDist(int x, int y, int height, int width, int pixles[IHEIGHT][IWIDTH], double *empirDist);
double calcMergingCriterion(double *empirptr, double n, double *nempirptr, double nn);
double KLDivergence(double *empirptr, double *jointptr);
int findNextBestRegion(Imageptr image);
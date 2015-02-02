//FILE READING AND WRITING
extern int IWIDTH;
extern int IHEIGHT;

//file reading functions
void cleanup (int **name, int row);
int **createArray (int row, int col);
void readFile(int width, int height, char *fileName, int pixels[IHEIGHT][IWIDTH]);

//file writing functions
void writeFile(int pixels[IHEIGHT][IWIDTH], int height, int width, char *outputFileName);

//to do with altering the pixel values to reflect the merging
void colourRegions(int pixels[IHEIGHT][IWIDTH], Imageptr image);
int calculateAverage(Regionptr r, int pixels[IHEIGHT][IWIDTH]);
void alterPixels(Regionptr r, int pixels[IHEIGHT][IWIDTH], int pixValue);
int countCurrentRegions(int *remainingRegions, int numAtoms);
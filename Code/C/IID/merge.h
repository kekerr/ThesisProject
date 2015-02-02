//Merging Two Regions:
Regionptr Mergeregions(Regionptr r1, Regionptr r2, Imageptr image);

//Updating Neighbour lists:
void removeRegLink(int r1ID, Regionptr r2);
void mergeNeighborLists(Regionptr r1, Regionptr r2);
void updateAllNeighbours(Regionptr r1, Regionptr r2, Imageptr image);
Neighbourptr findlastneighbour(Neighbourptr head);
void updateBestMergingCriterion(Regionptr r);
void updateR1Neighbour(Neighbourptr headNeighbour, Regionptr nr, Regionptr r1, Neighbourptr r1n);
void removeDuplicate(Neighbourptr headNeighbour, int r1RegionID);
void renameNeighbourRegions(int r1RegionID, int r2RegionID, Neighbourptr headNeighbour);

//Updating other parts of the regions:
void updateAtomRegIDs(int ID, Atomptr a);
void updateR1Values(Regionptr r1, Regionptr r2);
void updateAtomList(Regionptr r1, Regionptr r2);








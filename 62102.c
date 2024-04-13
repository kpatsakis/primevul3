static dynamicPtr *newDynamic(int initialSize, void *data, int freeOKFlag)
{
	dynamicPtr *dp;

	dp = (dynamicPtr *) gdMalloc(sizeof (dynamicPtr));
	if(dp == NULL) {
		return NULL;
	}

	if(!allocDynamic(dp, initialSize, data)) {
		gdFree(dp);
		return NULL;
	}

	dp->pos = 0;
	dp->freeOK = freeOKFlag;

	return dp;
}

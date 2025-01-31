static int trimDynamic(dynamicPtr *dp)
{
	/* 2.0.21: we don't reallocate memory we don't own */
	if(!dp->freeOK) {
		return TRUE;
	}

	return gdReallocDynamic(dp, dp->logicalSize);
}

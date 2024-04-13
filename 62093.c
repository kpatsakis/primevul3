static int allocDynamic(dynamicPtr *dp, int initialSize, void *data)
{
	if(data == NULL) {
		dp->logicalSize = 0;
		dp->dataGood = FALSE;
		dp->data = gdMalloc(initialSize);
	} else {
		dp->logicalSize = initialSize;
		dp->dataGood = TRUE;
		dp->data = data;
	}

	if(dp->data != NULL) {
		dp->realSize = initialSize;
		dp->dataGood = TRUE;
		dp->pos = 0;
		return TRUE;
	} else {
		dp->realSize = 0;
		return FALSE;
	}
}

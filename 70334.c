uint32_t GPMF_SizeOfComplexTYPE(char *type, uint32_t typestringlength)
{
	char *typearray = type;
	uint32_t size = 0, expand = 0;
	uint32_t i, len = typestringlength;


	for (i = 0; i < len; i++)
		if (typearray[i] == '[')
			expand = 1;
			
	if (expand)
	{
		char exptypearray[64];
		uint32_t dstsize = sizeof(exptypearray);

		if (GPMF_OK == GPMF_ExpandComplexTYPE(typearray, len, exptypearray, &dstsize))
		{
			typearray = exptypearray;
			len = dstsize;
		}
		else
			return 0;
	}


	for (i = 0; i < len; i++)
	{
		uint32_t typesize = GPMF_SizeofType((GPMF_SampleType)typearray[i]);

		if (typesize < 1) return 0;
		size += typesize;
	}

	return size;
}

validate_sfinfo (SF_INFO *sfinfo)
{	if (sfinfo->samplerate < 1)
		return 0 ;
	if (sfinfo->frames < 0)
		return 0 ;
	if (sfinfo->channels < 1)
		return 0 ;
	if ((SF_CONTAINER (sfinfo->format)) == 0)
		return 0 ;
	if ((SF_CODEC (sfinfo->format)) == 0)
		return 0 ;
	if (sfinfo->sections < 1)
		return 0 ;
	return 1 ;
} /* validate_sfinfo */

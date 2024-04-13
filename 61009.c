t2p_sample_rgbaa_to_rgb(tdata_t data, uint32 samplecount)
{
	uint32 i;
	
    /* For the 3 first samples, there is overlapping between souce and
       destination, so use memmove().
       See http://bugzilla.maptools.org/show_bug.cgi?id=2577 */
    for(i = 0; i < 3 && i < samplecount; i++)
        memmove((uint8*)data + i * 3, (uint8*)data + i * 4, 3);
	for(; i < samplecount; i++)
		memcpy((uint8*)data + i * 3, (uint8*)data + i * 4, 3);

	return(i * 3);
}

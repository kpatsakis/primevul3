void CloseSource(size_t handle)
{
	mp4object *mp4 = (mp4object *)handle;
	if (mp4 == NULL) return;

	if (mp4->mediafp) fclose(mp4->mediafp), mp4->mediafp = NULL;
	if (mp4->metasizes) free(mp4->metasizes), mp4->metasizes = 0;
	if (mp4->metaoffsets) free(mp4->metaoffsets), mp4->metaoffsets = 0;

	free(mp4);
 }

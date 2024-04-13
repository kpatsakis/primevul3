uint32_t GetPayloadSize(size_t handle, uint32_t index)
{
	mp4object *mp4 = (mp4object *)handle;
	if (mp4 == NULL) return 0;

	if (mp4->metasizes && mp4->metasize_count > index)
		return mp4->metasizes[index];

 	return 0;
 }

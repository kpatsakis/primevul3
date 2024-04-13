uint32_t GetNumberPayloads(size_t handle)
{
	mp4object *mp4 = (mp4object *)handle;

	if (mp4)
	{
		return mp4->indexcount;
	}

 	return 0;
 }

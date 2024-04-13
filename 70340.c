float GetDuration(size_t handle)
{
	mp4object *mp4 = (mp4object *)handle;
	if (mp4 == NULL) return 0.0;

	return (float)mp4->metadatalength;
}

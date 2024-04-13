static int _match_jobid(void *listentry, void *key)
{
	uint32_t *job0 = (uint32_t *)listentry;
	uint32_t *job1 = (uint32_t *)key;

	return (*job0 == *job1);
}

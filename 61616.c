static void _note_batch_job_finished(uint32_t job_id)
{
	slurm_mutex_lock(&fini_mutex);
	fini_job_id[next_fini_job_inx] = job_id;
	if (++next_fini_job_inx >= FINI_JOB_CNT)
		next_fini_job_inx = 0;
	slurm_mutex_unlock(&fini_mutex);
}

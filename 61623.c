_remove_starting_step(uint16_t type, void *req)
{
	uint32_t job_id, step_id;
	ListIterator iter;
	starting_step_t *starting_step;
	int rc = SLURM_SUCCESS;
	bool found = false;

	slurm_mutex_lock(&conf->starting_steps_lock);

	switch(type) {
	case LAUNCH_BATCH_JOB:
		job_id =  ((batch_job_launch_msg_t *)req)->job_id;
		step_id = ((batch_job_launch_msg_t *)req)->step_id;
		break;
	case LAUNCH_TASKS:
		job_id =  ((launch_tasks_request_msg_t *)req)->job_id;
		step_id = ((launch_tasks_request_msg_t *)req)->job_step_id;
		break;
	default:
		error("%s called with an invalid type: %u", __func__, type);
		rc = SLURM_FAILURE;
		goto fail;
	}

	iter = list_iterator_create(conf->starting_steps);
	while ((starting_step = list_next(iter))) {
		if (starting_step->job_id  == job_id &&
		    starting_step->step_id == step_id) {
			starting_step = list_remove(iter);
			xfree(starting_step);

			found = true;
			pthread_cond_broadcast(&conf->starting_steps_cond);
			break;
		}
	}
	if (!found) {
		error("%s: step %u.%u not found", __func__, job_id, step_id);
		rc = SLURM_FAILURE;
	}
fail:
	slurm_mutex_unlock(&conf->starting_steps_lock);
	return rc;
}

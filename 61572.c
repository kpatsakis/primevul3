_add_starting_step(uint16_t type, void *req)
{
	starting_step_t *starting_step;
	int rc = SLURM_SUCCESS;

	/* Add the step info to a list of starting processes that
	   cannot reliably be contacted. */
	slurm_mutex_lock(&conf->starting_steps_lock);
	starting_step = xmalloc(sizeof(starting_step_t));
	if (!starting_step) {
		error("%s failed to allocate memory", __func__);
		rc = SLURM_FAILURE;
		goto fail;
	}
	switch (type) {
	case LAUNCH_BATCH_JOB:
		starting_step->job_id =
			((batch_job_launch_msg_t *)req)->job_id;
		starting_step->step_id =
			((batch_job_launch_msg_t *)req)->step_id;
		break;
	case LAUNCH_TASKS:
		starting_step->job_id =
			((launch_tasks_request_msg_t *)req)->job_id;
		starting_step->step_id =
			((launch_tasks_request_msg_t *)req)->job_step_id;
		break;
	case REQUEST_LAUNCH_PROLOG:
		starting_step->job_id  = ((prolog_launch_msg_t *)req)->job_id;
		starting_step->step_id = SLURM_EXTERN_CONT;
		break;
	default:
		error("%s called with an invalid type: %u", __func__, type);
		rc = SLURM_FAILURE;
		xfree(starting_step);
		goto fail;
	}

	if (!list_append(conf->starting_steps, starting_step)) {
		error("%s failed to allocate memory for list", __func__);
		rc = SLURM_FAILURE;
		xfree(starting_step);
		goto fail;
	}

fail:
	slurm_mutex_unlock(&conf->starting_steps_lock);
	return rc;
}

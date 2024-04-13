slurmd_req(slurm_msg_t *msg)
{
	int rc;

	if (msg == NULL) {
		if (startup == 0)
			startup = time(NULL);
		FREE_NULL_LIST(waiters);
		slurm_mutex_lock(&job_limits_mutex);
		if (job_limits_list) {
			FREE_NULL_LIST(job_limits_list);
			job_limits_loaded = false;
		}
		slurm_mutex_unlock(&job_limits_mutex);
		return;
	}

	switch (msg->msg_type) {
	case REQUEST_LAUNCH_PROLOG:
		debug2("Processing RPC: REQUEST_LAUNCH_PROLOG");
		_rpc_prolog(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_BATCH_JOB_LAUNCH:
		debug2("Processing RPC: REQUEST_BATCH_JOB_LAUNCH");
		/* Mutex locking moved into _rpc_batch_job() due to
		 * very slow prolog on Blue Gene system. Only batch
		 * jobs are supported on Blue Gene (no job steps). */
		_rpc_batch_job(msg, true);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_LAUNCH_TASKS:
		debug2("Processing RPC: REQUEST_LAUNCH_TASKS");
		slurm_mutex_lock(&launch_mutex);
		_rpc_launch_tasks(msg);
		slurm_mutex_unlock(&launch_mutex);
		break;
	case REQUEST_SIGNAL_TASKS:
		debug2("Processing RPC: REQUEST_SIGNAL_TASKS");
		_rpc_signal_tasks(msg);
		break;
	case REQUEST_CHECKPOINT_TASKS:
		debug2("Processing RPC: REQUEST_CHECKPOINT_TASKS");
		_rpc_checkpoint_tasks(msg);
		break;
	case REQUEST_TERMINATE_TASKS:
		debug2("Processing RPC: REQUEST_TERMINATE_TASKS");
		_rpc_terminate_tasks(msg);
		break;
	case REQUEST_KILL_PREEMPTED:
		debug2("Processing RPC: REQUEST_KILL_PREEMPTED");
		last_slurmctld_msg = time(NULL);
		_rpc_timelimit(msg);
		break;
	case REQUEST_KILL_TIMELIMIT:
		debug2("Processing RPC: REQUEST_KILL_TIMELIMIT");
		last_slurmctld_msg = time(NULL);
		_rpc_timelimit(msg);
		break;
	case REQUEST_REATTACH_TASKS:
		debug2("Processing RPC: REQUEST_REATTACH_TASKS");
		_rpc_reattach_tasks(msg);
		break;
	case REQUEST_SIGNAL_JOB:
		debug2("Processing RPC: REQUEST_SIGNAL_JOB");
		_rpc_signal_job(msg);
		break;
	case REQUEST_SUSPEND_INT:
		debug2("Processing RPC: REQUEST_SUSPEND_INT");
		_rpc_suspend_job(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_ABORT_JOB:
		debug2("Processing RPC: REQUEST_ABORT_JOB");
		last_slurmctld_msg = time(NULL);
		_rpc_abort_job(msg);
		break;
	case REQUEST_TERMINATE_JOB:
		debug2("Processing RPC: REQUEST_TERMINATE_JOB");
		last_slurmctld_msg = time(NULL);
		_rpc_terminate_job(msg);
		break;
	case REQUEST_COMPLETE_BATCH_SCRIPT:
		debug2("Processing RPC: REQUEST_COMPLETE_BATCH_SCRIPT");
		_rpc_complete_batch(msg);
		break;
	case REQUEST_UPDATE_JOB_TIME:
		debug2("Processing RPC: REQUEST_UPDATE_JOB_TIME");
		_rpc_update_time(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_SHUTDOWN:
		debug2("Processing RPC: REQUEST_SHUTDOWN");
		_rpc_shutdown(msg);
		break;
	case REQUEST_RECONFIGURE:
		debug2("Processing RPC: REQUEST_RECONFIGURE");
		_rpc_reconfig(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_REBOOT_NODES:
		debug2("Processing RPC: REQUEST_REBOOT_NODES");
		_rpc_reboot(msg);
		break;
	case REQUEST_NODE_REGISTRATION_STATUS:
		debug2("Processing RPC: REQUEST_NODE_REGISTRATION_STATUS");
		/* Treat as ping (for slurmctld agent, just return SUCCESS) */
		rc = _rpc_ping(msg);
		last_slurmctld_msg = time(NULL);
		/* Then initiate a separate node registration */
		if (rc == SLURM_SUCCESS)
			send_registration_msg(SLURM_SUCCESS, true);
		break;
	case REQUEST_PING:
		_rpc_ping(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_HEALTH_CHECK:
		debug2("Processing RPC: REQUEST_HEALTH_CHECK");
		_rpc_health_check(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_ACCT_GATHER_UPDATE:
		debug2("Processing RPC: REQUEST_ACCT_GATHER_UPDATE");
		_rpc_acct_gather_update(msg);
		last_slurmctld_msg = time(NULL);
		break;
	case REQUEST_ACCT_GATHER_ENERGY:
		debug2("Processing RPC: REQUEST_ACCT_GATHER_ENERGY");
		_rpc_acct_gather_energy(msg);
		break;
	case REQUEST_JOB_ID:
		_rpc_pid2jid(msg);
		break;
	case REQUEST_FILE_BCAST:
		rc = _rpc_file_bcast(msg);
		slurm_send_rc_msg(msg, rc);
		break;
	case REQUEST_STEP_COMPLETE:
		(void) _rpc_step_complete(msg);
		break;
	case REQUEST_STEP_COMPLETE_AGGR:
		(void) _rpc_step_complete_aggr(msg);
		break;
	case REQUEST_JOB_STEP_STAT:
		(void) _rpc_stat_jobacct(msg);
		break;
	case REQUEST_JOB_STEP_PIDS:
		(void) _rpc_list_pids(msg);
		break;
	case REQUEST_DAEMON_STATUS:
		_rpc_daemon_status(msg);
		break;
	case REQUEST_JOB_NOTIFY:
		_rpc_job_notify(msg);
		break;
	case REQUEST_FORWARD_DATA:
		_rpc_forward_data(msg);
		break;
	case REQUEST_NETWORK_CALLERID:
		debug2("Processing RPC: REQUEST_NETWORK_CALLERID");
		_rpc_network_callerid(msg);
		break;
	case MESSAGE_COMPOSITE:
		error("Processing RPC: MESSAGE_COMPOSITE: "
		      "This should never happen");
		msg_aggr_add_msg(msg, 0, NULL);
		break;
	case RESPONSE_MESSAGE_COMPOSITE:
		debug2("Processing RPC: RESPONSE_MESSAGE_COMPOSITE");
		msg_aggr_resp(msg);
		break;
	default:
		error("slurmd_req: invalid request msg type %d",
		      msg->msg_type);
		slurm_send_rc_msg(msg, EINVAL);
		break;
	}
	return;
}

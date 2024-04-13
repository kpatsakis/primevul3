megasas_queue_command(struct Scsi_Host *shost, struct scsi_cmnd *scmd)
{
	struct megasas_instance *instance;
	struct MR_PRIV_DEVICE *mr_device_priv_data;

	instance = (struct megasas_instance *)
	    scmd->device->host->hostdata;

	if (instance->unload == 1) {
		scmd->result = DID_NO_CONNECT << 16;
		scmd->scsi_done(scmd);
		return 0;
	}

	if (instance->issuepend_done == 0)
		return SCSI_MLQUEUE_HOST_BUSY;


	/* Check for an mpio path and adjust behavior */
	if (atomic_read(&instance->adprecovery) == MEGASAS_ADPRESET_SM_INFAULT) {
		if (megasas_check_mpio_paths(instance, scmd) ==
		    (DID_REQUEUE << 16)) {
			return SCSI_MLQUEUE_HOST_BUSY;
		} else {
			scmd->result = DID_NO_CONNECT << 16;
			scmd->scsi_done(scmd);
			return 0;
		}
	}

	if (atomic_read(&instance->adprecovery) == MEGASAS_HW_CRITICAL_ERROR) {
		scmd->result = DID_NO_CONNECT << 16;
		scmd->scsi_done(scmd);
		return 0;
	}

	mr_device_priv_data = scmd->device->hostdata;
	if (!mr_device_priv_data) {
		scmd->result = DID_NO_CONNECT << 16;
		scmd->scsi_done(scmd);
		return 0;
	}

	if (atomic_read(&instance->adprecovery) != MEGASAS_HBA_OPERATIONAL)
		return SCSI_MLQUEUE_HOST_BUSY;

	if (mr_device_priv_data->tm_busy)
		return SCSI_MLQUEUE_DEVICE_BUSY;


	scmd->result = 0;

	if (MEGASAS_IS_LOGICAL(scmd->device) &&
	    (scmd->device->id >= instance->fw_supported_vd_count ||
		scmd->device->lun)) {
		scmd->result = DID_BAD_TARGET << 16;
		goto out_done;
	}

	if ((scmd->cmnd[0] == SYNCHRONIZE_CACHE) &&
	    MEGASAS_IS_LOGICAL(scmd->device) &&
	    (!instance->fw_sync_cache_support)) {
		scmd->result = DID_OK << 16;
		goto out_done;
	}

	return instance->instancet->build_and_issue_cmd(instance, scmd);

 out_done:
	scmd->scsi_done(scmd);
	return 0;
}

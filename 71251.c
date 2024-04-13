megasas_aen_polling(struct work_struct *work)
{
	struct megasas_aen_event *ev =
		container_of(work, struct megasas_aen_event, hotplug_work.work);
	struct megasas_instance *instance = ev->instance;
	union megasas_evt_class_locale class_locale;
	int event_type = 0;
	u32 seq_num, wait_time = MEGASAS_RESET_WAIT_TIME;
	int error;
	u8  dcmd_ret = DCMD_SUCCESS;

	if (!instance) {
		printk(KERN_ERR "invalid instance!\n");
		kfree(ev);
		return;
	}

	/* Adjust event workqueue thread wait time for VF mode */
	if (instance->requestorId)
		wait_time = MEGASAS_ROUTINE_WAIT_TIME_VF;

	/* Don't run the event workqueue thread if OCR is running */
	mutex_lock(&instance->reset_mutex);

	instance->ev = NULL;
	if (instance->evt_detail) {
		megasas_decode_evt(instance);

		switch (le32_to_cpu(instance->evt_detail->code)) {

		case MR_EVT_PD_INSERTED:
		case MR_EVT_PD_REMOVED:
			event_type = SCAN_PD_CHANNEL;
			break;

		case MR_EVT_LD_OFFLINE:
		case MR_EVT_CFG_CLEARED:
		case MR_EVT_LD_DELETED:
		case MR_EVT_LD_CREATED:
			event_type = SCAN_VD_CHANNEL;
			break;

		case MR_EVT_CTRL_HOST_BUS_SCAN_REQUESTED:
		case MR_EVT_FOREIGN_CFG_IMPORTED:
		case MR_EVT_LD_STATE_CHANGE:
			event_type = SCAN_PD_CHANNEL | SCAN_VD_CHANNEL;
			dev_info(&instance->pdev->dev, "scanning for scsi%d...\n",
				instance->host->host_no);
			break;

		case MR_EVT_CTRL_PROP_CHANGED:
			dcmd_ret = megasas_get_ctrl_info(instance);
			if (dcmd_ret == DCMD_SUCCESS &&
			    instance->snapdump_wait_time) {
				megasas_get_snapdump_properties(instance);
				dev_info(&instance->pdev->dev,
					 "Snap dump wait time\t: %d\n",
					 instance->snapdump_wait_time);
			}
			break;
		default:
			event_type = 0;
			break;
		}
	} else {
		dev_err(&instance->pdev->dev, "invalid evt_detail!\n");
		mutex_unlock(&instance->reset_mutex);
		kfree(ev);
		return;
	}

	if (event_type)
		dcmd_ret = megasas_update_device_list(instance, event_type);

	mutex_unlock(&instance->reset_mutex);

	if (event_type && dcmd_ret == DCMD_SUCCESS)
		megasas_add_remove_devices(instance, event_type);

	if (dcmd_ret == DCMD_SUCCESS)
		seq_num = le32_to_cpu(instance->evt_detail->seq_num) + 1;
	else
		seq_num = instance->last_seq_num;

	/* Register AEN with FW for latest sequence number plus 1 */
	class_locale.members.reserved = 0;
	class_locale.members.locale = MR_EVT_LOCALE_ALL;
	class_locale.members.class = MR_EVT_CLASS_DEBUG;

	if (instance->aen_cmd != NULL) {
		kfree(ev);
		return;
	}

	mutex_lock(&instance->reset_mutex);
	error = megasas_register_aen(instance, seq_num,
					class_locale.word);
	if (error)
		dev_err(&instance->pdev->dev,
			"register aen failed error %x\n", error);

	mutex_unlock(&instance->reset_mutex);
	kfree(ev);
}

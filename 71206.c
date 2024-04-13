static void set_maintenance_mode(void *send_info, bool enable)
{
	struct smi_info   *smi_info = send_info;

	if (!enable)
		atomic_set(&smi_info->req_events, 0);
}

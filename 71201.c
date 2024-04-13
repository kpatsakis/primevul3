static void request_events(void *send_info)
{
	struct smi_info *smi_info = send_info;

	if (!smi_info->has_event_buffer)
		return;

	atomic_set(&smi_info->req_events, 1);
}

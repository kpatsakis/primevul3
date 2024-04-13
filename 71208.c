static void set_run_to_completion(void *send_info, bool i_run_to_completion)
{
	struct smi_info   *smi_info = send_info;

	smi_info->run_to_completion = i_run_to_completion;
	if (i_run_to_completion)
		flush_messages(smi_info);
}

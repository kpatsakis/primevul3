lspci_process_line(const char *line, void *data)
{
	UNUSED(data);
	char *lspci_command[5] = { "lspci", "-m", "-n", "-v", NULL };

	if (!strcmp(line, "LSPCI"))
	{
		memset(&current_device, 0, sizeof(current_device));
		subprocess(lspci_command, handle_child_line, NULL);
		/* Send single dot to indicate end of enumeration */
		lspci_send(".\n");
	}
	else
	{
		logger(Core, Error, "lspci_process_line(), invalid line '%s'", line);
	}
	return True;
}

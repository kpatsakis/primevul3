seamless_line_handler(const char *line, void *data)
{
	if (!seamless_process_line(line, data))
	{
		logger(Core, Warning, "seamless_line_handler(), invalid request '%s'", line);
	}
	return True;
}

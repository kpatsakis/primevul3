rdpsnddbg_line_handler(const char *line, void *data)
{
	UNUSED(data);
	logger(Sound, Debug, "rdpsnddbg_line_handler(), \"%s\"", line);
	return True;
}

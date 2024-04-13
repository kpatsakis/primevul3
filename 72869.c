rdpsnd_auto_select(void)
{
	static RD_BOOL failed = False;

	if (!failed)
	{
		current_driver = drivers;
		while (current_driver != NULL)
		{
			logger(Sound, Debug, "rdpsnd_auto_select(), trying driver '%s'",
			       current_driver->name);
			if (current_driver->wave_out_open())
			{
				logger(Sound, Verbose, "rdpsnd_auto_select(), using driver '%s'",
				       current_driver->name);
				current_driver->wave_out_close();
				return True;
			}
			current_driver = current_driver->next;
		}

		logger(Sound, Debug, "no working audio-driver found");
		failed = True;
		current_driver = NULL;
	}

	return False;
}

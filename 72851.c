set_system_pointer(uint32 ptr)
{
	switch (ptr)
	{
		case SYSPTR_NULL:
			ui_set_null_cursor();
			break;
		case SYSPTR_DEFAULT:
			ui_set_standard_cursor();
			break;
		default:
			logger(Protocol, Warning,
			       "set_system_pointer(), unhandled pointer type 0x%x", ptr);
	}
}

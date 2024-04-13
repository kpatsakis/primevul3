early_console_setup (char *cmdline)
{
	int earlycons = 0;

#ifdef CONFIG_SERIAL_SGI_L1_CONSOLE
	{
		extern int sn_serial_console_early_setup(void);
		if (!sn_serial_console_early_setup())
			earlycons++;
	}
#endif
#ifdef CONFIG_EFI_PCDP
	if (!efi_setup_pcdp_console(cmdline))
		earlycons++;
#endif
	if (!simcons_register())
		earlycons++;

	return (earlycons) ? 0 : -1;
}

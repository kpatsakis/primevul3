static int __init run_dmi_scan(void)
{
	dmi_scan_machine();
	return 0;
}

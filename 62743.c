static int __init parse_elfcorehdr(char *arg)
{
	if (!arg)
		return -EINVAL;

        elfcorehdr_addr = memparse(arg, &arg);
	return 0;
}

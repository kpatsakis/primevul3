static int __init max_loop_setup(char *str)
{
	max_loop = simple_strtol(str, NULL, 0);
	return 1;
}

static int __init check_crashkernel_memory(unsigned long pbase, size_t size)
{
	if (ia64_platform_is("sn2") || ia64_platform_is("uv"))
		return 1;
	else
		return pbase < (1UL << 32);
}

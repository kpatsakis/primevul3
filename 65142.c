static inline bool retp_compiler(void)
{
	return __is_defined(RETPOLINE);
}

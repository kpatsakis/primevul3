static void dec_user_namespaces(struct ucounts *ucounts)
{
	return dec_ucount(ucounts, UCOUNT_USER_NAMESPACES);
}

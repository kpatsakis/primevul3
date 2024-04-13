static struct ucounts *inc_user_namespaces(struct user_namespace *ns, kuid_t uid)
{
	return inc_ucount(ns, uid, UCOUNT_USER_NAMESPACES);
}

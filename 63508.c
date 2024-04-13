static void set_cred_user_ns(struct cred *cred, struct user_namespace *user_ns)
{
	/* Start with the same capabilities as init but useless for doing
	 * anything as the capabilities are bound to the new user namespace.
	 */
	cred->securebits = SECUREBITS_DEFAULT;
	cred->cap_inheritable = CAP_EMPTY_SET;
	cred->cap_permitted = CAP_FULL_SET;
	cred->cap_effective = CAP_FULL_SET;
	cred->cap_ambient = CAP_EMPTY_SET;
	cred->cap_bset = CAP_FULL_SET;
#ifdef CONFIG_KEYS
	key_put(cred->request_key_auth);
	cred->request_key_auth = NULL;
#endif
	/* tgcred will be cleared in our caller bc CLONE_THREAD won't be set */
	cred->user_ns = user_ns;
}

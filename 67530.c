X509_VERIFY_PARAM_inherit(X509_VERIFY_PARAM *dest, const X509_VERIFY_PARAM *src)
{
	unsigned long inh_flags;
	int to_default, to_overwrite;
	X509_VERIFY_PARAM_ID *id;

	if (!src)
		return 1;
	id = src->id;
	inh_flags = dest->inh_flags | src->inh_flags;

	if (inh_flags & X509_VP_FLAG_ONCE)
		dest->inh_flags = 0;

	if (inh_flags & X509_VP_FLAG_LOCKED)
		return 1;

	if (inh_flags & X509_VP_FLAG_DEFAULT)
		to_default = 1;
	else
		to_default = 0;

	if (inh_flags & X509_VP_FLAG_OVERWRITE)
		to_overwrite = 1;
	else
		to_overwrite = 0;

	x509_verify_param_copy(purpose, 0);
	x509_verify_param_copy(trust, 0);
	x509_verify_param_copy(depth, -1);

	/* If overwrite or check time not set, copy across */

	if (to_overwrite || !(dest->flags & X509_V_FLAG_USE_CHECK_TIME)) {
		dest->check_time = src->check_time;
		dest->flags &= ~X509_V_FLAG_USE_CHECK_TIME;
		/* Don't need to copy flag: that is done below */
	}

	if (inh_flags & X509_VP_FLAG_RESET_FLAGS)
		dest->flags = 0;

	dest->flags |= src->flags;

	if (test_x509_verify_param_copy(policies, NULL)) {
		if (!X509_VERIFY_PARAM_set1_policies(dest, src->policies))
			return 0;
	}

	/* Copy the host flags if and only if we're copying the host list */
	if (test_x509_verify_param_copy_id(hosts, NULL)) {
		if (dest->id->hosts) {
			string_stack_free(dest->id->hosts);
			dest->id->hosts = NULL;
		}
		if (id->hosts) {
			dest->id->hosts =
			    sk_deep_copy(id->hosts, strdup, str_free);
			if (dest->id->hosts == NULL)
				return 0;
			dest->id->hostflags = id->hostflags;
		}
	}

	if (test_x509_verify_param_copy_id(email, NULL)) {
		if (!X509_VERIFY_PARAM_set1_email(dest, id->email,
		    id->emaillen))
			return 0;
	}

	if (test_x509_verify_param_copy_id(ip, NULL)) {
		if (!X509_VERIFY_PARAM_set1_ip(dest, id->ip, id->iplen))
			return 0;
	}

	return 1;
}

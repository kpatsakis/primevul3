PAM_EXTERN int pam_sm_authenticate(pam_handle_t * pamh, int flags, int argc,
		const char **argv)
{
	int r;
	PKCS11_CTX *ctx;
	unsigned int nslots;
	PKCS11_KEY *authkey;
	PKCS11_SLOT *slots, *authslot;
	const char *user;
	const char *pin_regex;

	r = module_refresh(pamh, flags, argc, argv,
			&user, &ctx, &slots, &nslots, &pin_regex);
	if (PAM_SUCCESS != r) {
		goto err;
	}

	if (1 != key_find(pamh, flags, user, ctx, slots, nslots,
				&authslot, &authkey)) {
		r = PAM_AUTHINFO_UNAVAIL;
		goto err;
	}
	if (1 != key_login(pamh, flags, authslot, pin_regex)
			|| 1 != key_verify(pamh, flags, authkey)) {
		if (authslot->token->userPinLocked) {
			r = PAM_MAXTRIES;
		} else {
			r = PAM_AUTH_ERR;
		}
		goto err;
	}

	r = PAM_SUCCESS;

err:
#ifdef TEST
	module_data_cleanup(pamh, global_module_data, r);
#endif
	return r;
}

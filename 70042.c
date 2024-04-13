PAM_EXTERN int pam_sm_chauthtok(pam_handle_t * pamh, int flags, int argc,
		const char **argv)
{
	int r;
	PKCS11_CTX *ctx;
	unsigned int nslots;
	PKCS11_KEY *authkey;
	PKCS11_SLOT *slots, *authslot;
	const char *user, *pin_regex;

	r = module_refresh(pamh, flags, argc, argv,
			&user, &ctx, &slots, &nslots, &pin_regex);
	if (PAM_SUCCESS != r) {
		goto err;
	}

	if (flags & PAM_CHANGE_EXPIRED_AUTHTOK) {
		/* Yes, we explicitly don't want to check CRLs, OCSP or exparation of
		 * certificates (use pam_pkcs11 for this). */
		r = PAM_SUCCESS;
		goto err;
	}

	if (1 != key_find(pamh, flags, user, ctx, slots, nslots,
				&authslot, &authkey)) {
		r = PAM_AUTHINFO_UNAVAIL;
		goto err;
	}

	if (flags & PAM_PRELIM_CHECK) {
		r = PAM_TRY_AGAIN;
		goto err;
	}

	if (flags & PAM_UPDATE_AUTHTOK) {
		if (1 != key_change_login(pamh, flags, authslot, pin_regex)) {
			if (authslot->token->userPinLocked) {
				r = PAM_MAXTRIES;
			} else {
				r = PAM_AUTH_ERR;
			}
			goto err;
		}
	}

	r = PAM_SUCCESS;

err:
#ifdef TEST
	module_data_cleanup(pamh, global_module_data, r);
#endif
	return r;
}

static int module_refresh(pam_handle_t *pamh,
		int flags, int argc, const char **argv,
		const char **user, PKCS11_CTX **ctx,
		PKCS11_SLOT **slots, unsigned int *nslots,
		const char **pin_regex)
{
	int r;
	struct module_data *module_data;

	if (PAM_SUCCESS != pam_get_data(pamh, PACKAGE, (void *)&module_data)
			|| NULL == module_data) {
		r = module_initialize(pamh, flags, argc, argv, &module_data);
		if (PAM_SUCCESS != r) {
			goto err;
		}
	} else {
		/* refresh all known slots */
		PKCS11_release_all_slots(module_data->ctx,
				module_data->slots, module_data->nslots);
		module_data->slots = NULL;
		module_data->nslots = 0;
		if (0 != PKCS11_enumerate_slots(module_data->ctx,
					&module_data->slots, &module_data->nslots)) {
			pam_syslog(pamh, LOG_ALERT, "Initializing PKCS#11 engine failed: %s\n",
					ERR_reason_error_string(ERR_get_error()));
			prompt(flags, pamh, PAM_ERROR_MSG , NULL, _("Error initializing PKCS#11 module"));
			r = PAM_AUTHINFO_UNAVAIL;
			goto err;
		}
	}

	if (1 < argc) {
		*pin_regex = argv[1];
	} else {
#ifdef __APPLE__
		/* If multiple PAMs are allowed for macOS' login, then the captured
		 * password is used for all possible modules. To not block the token's
		 * PIN if the user enters his standard password, we're refusing to use
		 * anything that doesn't look like a PIN. */
		*pin_regex = "^[[:digit:]]*$";
#else
		*pin_regex = NULL;
#endif
	}

	r = pam_get_user(pamh, user, NULL);
	if (PAM_SUCCESS != r) {
		pam_syslog(pamh, LOG_ERR, "pam_get_user() failed %s",
				pam_strerror(pamh, r));
		r = PAM_USER_UNKNOWN;
		goto err;
	}

	*ctx = module_data->ctx;
	*nslots = module_data->nslots;
	*slots = module_data->slots;

err:
	return r;
}

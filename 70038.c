static int module_initialize(pam_handle_t * pamh,
		int flags, int argc, const char **argv,
		struct module_data **module_data)
{
	int r;
	struct module_data *data = calloc(1, sizeof *data);
	if (NULL == data) {
		pam_syslog(pamh, LOG_CRIT, "calloc() failed: %s",
				strerror(errno));
		r = PAM_BUF_ERR;
		goto err;
	}

#ifdef ENABLE_NLS
	setlocale(LC_ALL, "");
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
#endif

	/* Initialize OpenSSL */
	OpenSSL_add_all_algorithms();
	ERR_load_crypto_strings();

	/* Load and initialize PKCS#11 module */
	data->ctx = PKCS11_CTX_new();
	if (0 == argc || NULL == data->ctx
			|| 0 != PKCS11_CTX_load(data->ctx, argv[0])) {
		pam_syslog(pamh, LOG_ALERT, "Loading PKCS#11 engine failed: %s\n",
				ERR_reason_error_string(ERR_get_error()));
		prompt(flags, pamh, PAM_ERROR_MSG , NULL, _("Error loading PKCS#11 module"));
		r = PAM_NO_MODULE_DATA;
		goto err;
	}
	data->module_loaded = 1;
	if (0 != PKCS11_enumerate_slots(data->ctx, &data->slots, &data->nslots)) {
		pam_syslog(pamh, LOG_ALERT, "Initializing PKCS#11 engine failed: %s\n",
				ERR_reason_error_string(ERR_get_error()));
		prompt(flags, pamh, PAM_ERROR_MSG , NULL, _("Error initializing PKCS#11 module"));
		r = PAM_AUTHINFO_UNAVAIL;
		goto err;
	}

#ifdef TEST
	/* pam_set_data() is reserved for actual modules. For testing this would
	 * return PAM_SYSTEM_ERR, so we're saving the module data in a static
	 * variable. */
	r = PAM_SUCCESS;
	global_module_data = data;
#else
	r = pam_set_data(pamh, PACKAGE, data, module_data_cleanup);
	if (PAM_SUCCESS != r) {
		goto err;
	}
#endif

	*module_data = data;
	data = NULL;

err:
	module_data_cleanup(pamh, data, r);

	return r;
}

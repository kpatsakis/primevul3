user_key_allowed(struct ssh *ssh, struct passwd *pw, struct sshkey *key,
    int auth_attempt, struct sshauthopt **authoptsp)
{
	u_int success, i;
	char *file;
	struct sshauthopt *opts = NULL;
	if (authoptsp != NULL)
		*authoptsp = NULL;

	if (auth_key_is_revoked(key))
		return 0;
	if (sshkey_is_cert(key) &&
	    auth_key_is_revoked(key->cert->signature_key))
		return 0;

	if ((success = user_cert_trusted_ca(ssh, pw, key, &opts)) != 0)
		goto out;
	sshauthopt_free(opts);
	opts = NULL;

	if ((success = user_key_command_allowed2(ssh, pw, key, &opts)) != 0)
		goto out;
	sshauthopt_free(opts);
	opts = NULL;

	for (i = 0; !success && i < options.num_authkeys_files; i++) {
		if (strcasecmp(options.authorized_keys_files[i], "none") == 0)
			continue;
		file = expand_authorized_keys(
		    options.authorized_keys_files[i], pw);
		success = user_key_allowed2(ssh, pw, key, file, &opts);
		free(file);
	}

 out:
	if (success && authoptsp != NULL) {
		*authoptsp = opts;
		opts = NULL;
	}
	sshauthopt_free(opts);
	return success;
}

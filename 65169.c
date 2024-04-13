user_cert_trusted_ca(struct ssh *ssh, struct passwd *pw, struct sshkey *key,
    struct sshauthopt **authoptsp)
{
	char *ca_fp, *principals_file = NULL;
	const char *reason;
	struct sshauthopt *principals_opts = NULL, *cert_opts = NULL;
	struct sshauthopt *final_opts = NULL;
	int r, ret = 0, found_principal = 0, use_authorized_principals;

	if (authoptsp != NULL)
		*authoptsp = NULL;

	if (!sshkey_is_cert(key) || options.trusted_user_ca_keys == NULL)
		return 0;

	if ((ca_fp = sshkey_fingerprint(key->cert->signature_key,
	    options.fingerprint_hash, SSH_FP_DEFAULT)) == NULL)
		return 0;

	if ((r = sshkey_in_file(key->cert->signature_key,
	    options.trusted_user_ca_keys, 1, 0)) != 0) {
		debug2("%s: CA %s %s is not listed in %s: %s", __func__,
		    sshkey_type(key->cert->signature_key), ca_fp,
		    options.trusted_user_ca_keys, ssh_err(r));
		goto out;
	}
	/*
	 * If AuthorizedPrincipals is in use, then compare the certificate
	 * principals against the names in that file rather than matching
	 * against the username.
	 */
	if ((principals_file = authorized_principals_file(pw)) != NULL) {
		if (match_principals_file(ssh, pw, principals_file,
		    key->cert, &principals_opts))
			found_principal = 1;
	}
	/* Try querying command if specified */
	if (!found_principal && match_principals_command(ssh, pw, key,
	    &principals_opts))
		found_principal = 1;
	/* If principals file or command is specified, then require a match */
	use_authorized_principals = principals_file != NULL ||
            options.authorized_principals_command != NULL;
	if (!found_principal && use_authorized_principals) {
		reason = "Certificate does not contain an authorized principal";
		goto fail_reason;
	}
	if (use_authorized_principals && principals_opts == NULL)
		fatal("%s: internal error: missing principals_opts", __func__);
	if (sshkey_cert_check_authority(key, 0, 1,
	    use_authorized_principals ? NULL : pw->pw_name, &reason) != 0)
		goto fail_reason;

	/* Check authority from options in key and from principals file/cmd */
	if ((cert_opts = sshauthopt_from_cert(key)) == NULL) {
		reason = "Invalid certificate options";
		goto fail_reason;
	}
	if (auth_authorise_keyopts(ssh, pw, cert_opts, 0, "cert") != 0) {
		reason = "Refused by certificate options";
		goto fail_reason;
	}
	if (principals_opts == NULL) {
		final_opts = cert_opts;
		cert_opts = NULL;
	} else {
		if (auth_authorise_keyopts(ssh, pw, principals_opts, 0,
		    "principals") != 0) {
			reason = "Refused by certificate principals options";
			goto fail_reason;
		}
		if ((final_opts = sshauthopt_merge(principals_opts,
		    cert_opts, &reason)) == NULL) {
 fail_reason:
			error("%s", reason);
			auth_debug_add("%s", reason);
			goto out;
		}
	}

	/* Success */
	verbose("Accepted certificate ID \"%s\" (serial %llu) signed by "
	    "%s CA %s via %s", key->cert->key_id,
	    (unsigned long long)key->cert->serial,
	    sshkey_type(key->cert->signature_key), ca_fp,
	    options.trusted_user_ca_keys);
	if (authoptsp != NULL) {
		*authoptsp = final_opts;
		final_opts = NULL;
	}
	ret = 1;
 out:
	sshauthopt_free(principals_opts);
	sshauthopt_free(cert_opts);
	sshauthopt_free(final_opts);
	free(principals_file);
	free(ca_fp);
	return ret;
}

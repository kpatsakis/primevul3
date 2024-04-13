check_authkey_line(struct ssh *ssh, struct passwd *pw, struct sshkey *key,
    char *cp, const char *loc, struct sshauthopt **authoptsp)
{
	int want_keytype = sshkey_is_cert(key) ? KEY_UNSPEC : key->type;
	struct sshkey *found = NULL;
	struct sshauthopt *keyopts = NULL, *certopts = NULL, *finalopts = NULL;
	char *key_options = NULL, *fp = NULL;
	const char *reason = NULL;
	int ret = -1;

	if (authoptsp != NULL)
		*authoptsp = NULL;

	if ((found = sshkey_new(want_keytype)) == NULL) {
		debug3("%s: keytype %d failed", __func__, want_keytype);
		goto out;
	}

	/* XXX djm: peek at key type in line and skip if unwanted */

	if (sshkey_read(found, &cp) != 0) {
		/* no key?  check for options */
		debug2("%s: check options: '%s'", loc, cp);
		key_options = cp;
		if (advance_past_options(&cp) != 0) {
			reason = "invalid key option string";
			goto fail_reason;
		}
		skip_space(&cp);
		if (sshkey_read(found, &cp) != 0) {
			/* still no key?  advance to next line*/
			debug2("%s: advance: '%s'", loc, cp);
			goto out;
		}
	}
	/* Parse key options now; we need to know if this is a CA key */
	if ((keyopts = sshauthopt_parse(key_options, &reason)) == NULL) {
		debug("%s: bad key options: %s", loc, reason);
		auth_debug_add("%s: bad key options: %s", loc, reason);
		goto out;
	}
	/* Ignore keys that don't match or incorrectly marked as CAs */
	if (sshkey_is_cert(key)) {
		/* Certificate; check signature key against CA */
		if (!sshkey_equal(found, key->cert->signature_key) ||
		    !keyopts->cert_authority)
			goto out;
	} else {
		/* Plain key: check it against key found in file */
		if (!sshkey_equal(found, key) || keyopts->cert_authority)
			goto out;
	}

	/* We have a candidate key, perform authorisation checks */
	if ((fp = sshkey_fingerprint(found,
	    options.fingerprint_hash, SSH_FP_DEFAULT)) == NULL)
		fatal("%s: fingerprint failed", __func__);

	debug("%s: matching %s found: %s %s", loc,
	    sshkey_is_cert(key) ? "CA" : "key", sshkey_type(found), fp);

	if (auth_authorise_keyopts(ssh, pw, keyopts,
	    sshkey_is_cert(key), loc) != 0) {
		reason = "Refused by key options";
		goto fail_reason;
	}
	/* That's all we need for plain keys. */
	if (!sshkey_is_cert(key)) {
		verbose("Accepted key %s %s found at %s",
		    sshkey_type(found), fp, loc);
		finalopts = keyopts;
		keyopts = NULL;
		goto success;
	}

	/*
	 * Additional authorisation for certificates.
	 */

	/* Parse and check options present in certificate */
	if ((certopts = sshauthopt_from_cert(key)) == NULL) {
		reason = "Invalid certificate options";
		goto fail_reason;
	}
	if (auth_authorise_keyopts(ssh, pw, certopts, 0, loc) != 0) {
		reason = "Refused by certificate options";
		goto fail_reason;
	}
	if ((finalopts = sshauthopt_merge(keyopts, certopts, &reason)) == NULL)
		goto fail_reason;

	/*
	 * If the user has specified a list of principals as
	 * a key option, then prefer that list to matching
	 * their username in the certificate principals list.
	 */
	if (keyopts->cert_principals != NULL &&
	    !match_principals_option(keyopts->cert_principals, key->cert)) {
		reason = "Certificate does not contain an authorized principal";
		goto fail_reason;
	}
	if (sshkey_cert_check_authority(key, 0, 0,
	   keyopts->cert_principals == NULL ? pw->pw_name : NULL, &reason) != 0)
		goto fail_reason;

	verbose("Accepted certificate ID \"%s\" (serial %llu) "
	    "signed by CA %s %s found at %s",
	    key->cert->key_id,
	    (unsigned long long)key->cert->serial,
	    sshkey_type(found), fp, loc);

 success:
	if (finalopts == NULL)
		fatal("%s: internal error: missing options", __func__);
	if (authoptsp != NULL) {
		*authoptsp = finalopts;
		finalopts = NULL;
	}
	/* success */
	ret = 0;
	goto out;

 fail_reason:
	error("%s", reason);
	auth_debug_add("%s", reason);
 out:
	free(fp);
	sshauthopt_free(keyopts);
	sshauthopt_free(certopts);
	sshauthopt_free(finalopts);
	sshkey_free(found);
	return ret;
}

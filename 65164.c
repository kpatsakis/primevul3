match_principals_command(struct ssh *ssh, struct passwd *user_pw,
    const struct sshkey *key, struct sshauthopt **authoptsp)
{
	struct passwd *runas_pw = NULL;
	const struct sshkey_cert *cert = key->cert;
	FILE *f = NULL;
	int r, ok, found_principal = 0;
	int i, ac = 0, uid_swapped = 0;
	pid_t pid;
	char *tmp, *username = NULL, *command = NULL, **av = NULL;
	char *ca_fp = NULL, *key_fp = NULL, *catext = NULL, *keytext = NULL;
	char serial_s[16], uidstr[32];
	void (*osigchld)(int);

	if (authoptsp != NULL)
		*authoptsp = NULL;
	if (options.authorized_principals_command == NULL)
		return 0;
	if (options.authorized_principals_command_user == NULL) {
		error("No user for AuthorizedPrincipalsCommand specified, "
		    "skipping");
		return 0;
	}

	/*
	 * NB. all returns later this function should go via "out" to
	 * ensure the original SIGCHLD handler is restored properly.
	 */
	osigchld = signal(SIGCHLD, SIG_DFL);

	/* Prepare and verify the user for the command */
	username = percent_expand(options.authorized_principals_command_user,
	    "u", user_pw->pw_name, (char *)NULL);
	runas_pw = getpwnam(username);
	if (runas_pw == NULL) {
		error("AuthorizedPrincipalsCommandUser \"%s\" not found: %s",
		    username, strerror(errno));
		goto out;
	}

	/* Turn the command into an argument vector */
	if (argv_split(options.authorized_principals_command, &ac, &av) != 0) {
		error("AuthorizedPrincipalsCommand \"%s\" contains "
		    "invalid quotes", command);
		goto out;
	}
	if (ac == 0) {
		error("AuthorizedPrincipalsCommand \"%s\" yielded no arguments",
		    command);
		goto out;
	}
	if ((ca_fp = sshkey_fingerprint(cert->signature_key,
	    options.fingerprint_hash, SSH_FP_DEFAULT)) == NULL) {
		error("%s: sshkey_fingerprint failed", __func__);
		goto out;
	}
	if ((key_fp = sshkey_fingerprint(key,
	    options.fingerprint_hash, SSH_FP_DEFAULT)) == NULL) {
		error("%s: sshkey_fingerprint failed", __func__);
		goto out;
	}
	if ((r = sshkey_to_base64(cert->signature_key, &catext)) != 0) {
		error("%s: sshkey_to_base64 failed: %s", __func__, ssh_err(r));
		goto out;
	}
	if ((r = sshkey_to_base64(key, &keytext)) != 0) {
		error("%s: sshkey_to_base64 failed: %s", __func__, ssh_err(r));
		goto out;
	}
	snprintf(serial_s, sizeof(serial_s), "%llu",
	    (unsigned long long)cert->serial);
	snprintf(uidstr, sizeof(uidstr), "%llu",
	    (unsigned long long)user_pw->pw_uid);
	for (i = 1; i < ac; i++) {
		tmp = percent_expand(av[i],
		    "U", uidstr,
		    "u", user_pw->pw_name,
		    "h", user_pw->pw_dir,
		    "t", sshkey_ssh_name(key),
		    "T", sshkey_ssh_name(cert->signature_key),
		    "f", key_fp,
		    "F", ca_fp,
		    "k", keytext,
		    "K", catext,
		    "i", cert->key_id,
		    "s", serial_s,
		    (char *)NULL);
		if (tmp == NULL)
			fatal("%s: percent_expand failed", __func__);
		free(av[i]);
		av[i] = tmp;
	}
	/* Prepare a printable command for logs, etc. */
	command = argv_assemble(ac, av);

	if ((pid = subprocess("AuthorizedPrincipalsCommand", runas_pw, command,
	    ac, av, &f,
	    SSH_SUBPROCESS_STDOUT_CAPTURE|SSH_SUBPROCESS_STDERR_DISCARD)) == 0)
		goto out;

	uid_swapped = 1;
	temporarily_use_uid(runas_pw);

	ok = process_principals(ssh, f, "(command)", cert, authoptsp);

	fclose(f);
	f = NULL;

	if (exited_cleanly(pid, "AuthorizedPrincipalsCommand", command, 0) != 0)
		goto out;

	/* Read completed successfully */
	found_principal = ok;
 out:
	if (f != NULL)
		fclose(f);
	signal(SIGCHLD, osigchld);
	for (i = 0; i < ac; i++)
		free(av[i]);
	free(av);
	if (uid_swapped)
		restore_uid();
	free(command);
	free(username);
	free(ca_fp);
	free(key_fp);
	free(catext);
	free(keytext);
	return found_principal;
}

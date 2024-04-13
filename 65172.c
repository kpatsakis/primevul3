user_key_command_allowed2(struct ssh *ssh, struct passwd *user_pw,
    struct sshkey *key, struct sshauthopt **authoptsp)
{
	struct passwd *runas_pw = NULL;
	FILE *f = NULL;
	int r, ok, found_key = 0;
	int i, uid_swapped = 0, ac = 0;
	pid_t pid;
	char *username = NULL, *key_fp = NULL, *keytext = NULL;
	char uidstr[32], *tmp, *command = NULL, **av = NULL;
	void (*osigchld)(int);

	if (authoptsp != NULL)
		*authoptsp = NULL;
	if (options.authorized_keys_command == NULL)
		return 0;
	if (options.authorized_keys_command_user == NULL) {
		error("No user for AuthorizedKeysCommand specified, skipping");
		return 0;
	}

	/*
	 * NB. all returns later this function should go via "out" to
	 * ensure the original SIGCHLD handler is restored properly.
	 */
	osigchld = signal(SIGCHLD, SIG_DFL);

	/* Prepare and verify the user for the command */
	username = percent_expand(options.authorized_keys_command_user,
	    "u", user_pw->pw_name, (char *)NULL);
	runas_pw = getpwnam(username);
	if (runas_pw == NULL) {
		error("AuthorizedKeysCommandUser \"%s\" not found: %s",
		    username, strerror(errno));
		goto out;
	}

	/* Prepare AuthorizedKeysCommand */
	if ((key_fp = sshkey_fingerprint(key, options.fingerprint_hash,
	    SSH_FP_DEFAULT)) == NULL) {
		error("%s: sshkey_fingerprint failed", __func__);
		goto out;
	}
	if ((r = sshkey_to_base64(key, &keytext)) != 0) {
		error("%s: sshkey_to_base64 failed: %s", __func__, ssh_err(r));
		goto out;
	}

	/* Turn the command into an argument vector */
	if (argv_split(options.authorized_keys_command, &ac, &av) != 0) {
		error("AuthorizedKeysCommand \"%s\" contains invalid quotes",
		    command);
		goto out;
	}
	if (ac == 0) {
		error("AuthorizedKeysCommand \"%s\" yielded no arguments",
		    command);
		goto out;
	}
	snprintf(uidstr, sizeof(uidstr), "%llu",
	    (unsigned long long)user_pw->pw_uid);
	for (i = 1; i < ac; i++) {
		tmp = percent_expand(av[i],
		    "U", uidstr,
		    "u", user_pw->pw_name,
		    "h", user_pw->pw_dir,
		    "t", sshkey_ssh_name(key),
		    "f", key_fp,
		    "k", keytext,
		    (char *)NULL);
		if (tmp == NULL)
			fatal("%s: percent_expand failed", __func__);
		free(av[i]);
		av[i] = tmp;
	}
	/* Prepare a printable command for logs, etc. */
	command = argv_assemble(ac, av);

	/*
	 * If AuthorizedKeysCommand was run without arguments
	 * then fall back to the old behaviour of passing the
	 * target username as a single argument.
	 */
	if (ac == 1) {
		av = xreallocarray(av, ac + 2, sizeof(*av));
		av[1] = xstrdup(user_pw->pw_name);
		av[2] = NULL;
		/* Fix up command too, since it is used in log messages */
		free(command);
		xasprintf(&command, "%s %s", av[0], av[1]);
	}

	if ((pid = subprocess("AuthorizedKeysCommand", runas_pw, command,
	    ac, av, &f,
	    SSH_SUBPROCESS_STDOUT_CAPTURE|SSH_SUBPROCESS_STDERR_DISCARD)) == 0)
		goto out;

	uid_swapped = 1;
	temporarily_use_uid(runas_pw);

	ok = check_authkeys_file(ssh, user_pw, f,
	    options.authorized_keys_command, key, authoptsp);

	fclose(f);
	f = NULL;

	if (exited_cleanly(pid, "AuthorizedKeysCommand", command, 0) != 0)
		goto out;

	/* Read completed successfully */
	found_key = ok;
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
	free(key_fp);
	free(keytext);
	return found_key;
}

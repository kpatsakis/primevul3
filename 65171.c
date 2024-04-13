user_key_allowed2(struct ssh *ssh, struct passwd *pw, struct sshkey *key,
    char *file, struct sshauthopt **authoptsp)
{
	FILE *f;
	int found_key = 0;

	if (authoptsp != NULL)
		*authoptsp = NULL;

	/* Temporarily use the user's uid. */
	temporarily_use_uid(pw);

	debug("trying public key file %s", file);
	if ((f = auth_openkeyfile(file, pw, options.strict_modes)) != NULL) {
		found_key = check_authkeys_file(ssh, pw, f, file,
		    key, authoptsp);
		fclose(f);
	}

	restore_uid();
	return found_key;
}

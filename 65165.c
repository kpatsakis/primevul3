match_principals_file(struct ssh *ssh, struct passwd *pw, char *file,
    struct sshkey_cert *cert, struct sshauthopt **authoptsp)
{
	FILE *f;
	int success;

	if (authoptsp != NULL)
		*authoptsp = NULL;

	temporarily_use_uid(pw);
	debug("trying authorized principals file %s", file);
	if ((f = auth_openprincipals(file, pw, options.strict_modes)) == NULL) {
		restore_uid();
		return 0;
	}
	success = process_principals(ssh, f, file, cert, authoptsp);
	fclose(f);
	restore_uid();
	return success;
}

int prompt(int flags, pam_handle_t *pamh, int style, char **response,
		const char *fmt, ...)
{
	int r;

	if (PAM_SILENT == (flags & PAM_SILENT)
			&& style != PAM_TEXT_INFO
			&& style != PAM_PROMPT_ECHO_OFF) {
		/* PAM_SILENT does not override the prompting of the user for passwords
		 * etc., it only stops informative messages from being generated. We
		 * use PAM_TEXT_INFO and PAM_PROMPT_ECHO_OFF exclusively for the
		 * password prompt. */
		r = PAM_SUCCESS;
	} else {
		va_list args;
		va_start (args, fmt);
		if (!response) {
			char *p = NULL;
			r = pam_vprompt(pamh, style, &p, fmt, args);
			free(p);
		} else {
			r = pam_vprompt(pamh, style, response, fmt, args);
		}
		va_end(args);
	}

	return r;
}

static int pam_vprompt(pam_handle_t *pamh, int style, char **response,
		const char *fmt, va_list args)
{
	int r = PAM_CRED_INSUFFICIENT;
	const struct pam_conv *conv;
	struct pam_message msg;
	struct pam_response *resp = NULL;
	struct pam_message *(msgp[1]);

	char text[128];
	vsnprintf(text, sizeof text, fmt, args);

	msgp[0] = &msg;
	msg.msg_style = style;
	msg.msg = text;

	if (PAM_SUCCESS != pam_get_item(pamh, PAM_CONV, (const void **) &conv)
			|| NULL == conv || NULL == conv->conv
			|| conv->conv(1, (const struct pam_message **) msgp, &resp, conv->appdata_ptr)
			|| NULL == resp) {
		goto err;
	}
	if (NULL != response) {
		if (resp[0].resp) {
			*response = strdup(resp[0].resp);
			if (NULL == *response) {
				pam_syslog(pamh, LOG_CRIT, "strdup() failed: %s",
						strerror(errno));
				goto err;
			}
		} else {
			*response = NULL;
		}
	}

	r = PAM_SUCCESS;
err:
	if (resp) {
		OPENSSL_cleanse(&resp[0].resp, sizeof resp[0].resp);
		free(&resp[0]);
	}
	return r;
}

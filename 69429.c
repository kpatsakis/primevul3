gstd_errstring(char **str, int min_stat)
{
	gss_buffer_desc	 status;
	OM_uint32	 new_stat;
	OM_uint32	 msg_ctx = 0;
	OM_uint32	 ret;
	int		 len = 0;
	char		*tmp;
	char		*statstr;

	/* XXXrcd this is not correct yet */
	/* XXXwps ...and now it is. */

	if (!str)
		return -1;

	*str = NULL;
	tmp = NULL;

	do {
		ret = gss_display_status(&new_stat, min_stat,
		    GSS_C_MECH_CODE, GSS_C_NO_OID, &msg_ctx,
		    &status);

		/* GSSAPI strings are not NUL terminated */
		if ((statstr = (char *)malloc(status.length + 1)) == NULL) {
			LOG(LOG_ERR, ("unable to malloc status string "
				      "of length %ld", status.length));
			gss_release_buffer(&new_stat, &status);
			free(statstr);
			free(tmp);
			return 0;
		}

		memcpy(statstr, status.value, status.length);
		statstr[status.length] = '\0';

		if (GSS_ERROR(ret)) {
			free(statstr);
			free(tmp);
			break;
		}

		if (*str) {
			if ((*str = malloc(strlen(*str) + status.length +
					   3)) == NULL) {
				LOG(LOG_ERR, ("unable to malloc error "
						"string"));
				gss_release_buffer(&new_stat, &status);
				free(statstr);
				free(tmp);
				return 0;
			}

			len = sprintf(*str, "%s, %s", tmp, statstr);
		} else {
			*str = malloc(status.length + 1);
			if (!*str) {
				LOG(LOG_ERR, ("unable to malloc error "
						"string"));
				gss_release_buffer(&new_stat, &status);
				free(statstr);
				free(tmp);
				return 0;
			}
			len = sprintf(*str, "%s", (char *)statstr);
		}

		gss_release_buffer(&new_stat, &status);
		free(statstr);
		free(tmp);

		tmp = *str;
	} while (msg_ctx != 0);

	return len;
}

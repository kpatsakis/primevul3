PAM_EXTERN int pam_sm_open_session(pam_handle_t * pamh, int flags, int argc,
		const char **argv)
{
	pam_syslog(pamh, LOG_DEBUG,
			"Function pam_sm_open_session() is not implemented in this module");
	return PAM_SERVICE_ERR;
}

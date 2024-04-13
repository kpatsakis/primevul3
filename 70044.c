PAM_EXTERN int pam_sm_setcred(pam_handle_t * pamh, int flags, int argc,
		const char **argv)
{
	/* Actually, we should return the same value as pam_sm_authenticate(). */
	return PAM_SUCCESS;
}

PAM_EXTERN int pam_sm_acct_mgmt(pam_handle_t * pamh, int flags, int argc,
		const char **argv)
{
	/* if the user has been authenticated (precondition of this call), then
	 * everything is OK. Yes, we explicitly don't want to check CRLs, OCSP or
	 * exparation of certificates (use pam_pkcs11 for this). */
	return PAM_SUCCESS;
}

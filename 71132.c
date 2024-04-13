krb5_get_init_creds_opt_set_pkinit_user_certs(krb5_context context,
					      krb5_get_init_creds_opt *opt,
					      struct hx509_certs_data *certs)
{
#ifdef PKINIT
    if (opt->opt_private == NULL) {
	krb5_set_error_message(context, EINVAL,
			       N_("PKINIT: on non extendable opt", ""));
	return EINVAL;
    }
    if (opt->opt_private->pk_init_ctx == NULL) {
	krb5_set_error_message(context, EINVAL,
			       N_("PKINIT: on pkinit context", ""));
	return EINVAL;
    }

    _krb5_pk_set_user_id(context, NULL, opt->opt_private->pk_init_ctx, certs);

    return 0;
#else
    krb5_set_error_message(context, EINVAL,
			   N_("no support for PKINIT compiled in", ""));
    return EINVAL;
#endif
}

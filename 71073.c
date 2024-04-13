check_fast(krb5_context context, struct fast_state *state)
{
    if (state->flags & KRB5_FAST_EXPECTED) {
	krb5_set_error_message(context, KRB5KRB_AP_ERR_MODIFIED,
			       "Expected FAST, but no FAST "
			       "was in the response from the KDC");
	return KRB5KRB_AP_ERR_MODIFIED;
    }
    return 0;
}

make_fast_ap_fxarmor(krb5_context context,
		     struct fast_state *state,
		     const char *realm,
		     KrbFastArmor **armor)
{
    KrbFastArmor *fxarmor = NULL;
    krb5_error_code ret;

    if (state->armor_crypto)
	krb5_crypto_destroy(context, state->armor_crypto);
    krb5_free_keyblock_contents(context, &state->armor_key);


    ALLOC(fxarmor, 1);
    if (fxarmor == NULL)
	return krb5_enomem(context);

    if (state->flags & KRB5_FAST_AP_ARMOR_SERVICE) {
#ifdef WIN32
	krb5_set_error_message(context, ENOTSUP, "Fast armor IPC service not supportted yet on Windows");
	ret = ENOTSUP;
        goto out;
#else /* WIN32 */
	KERB_ARMOR_SERVICE_REPLY msg;
	krb5_data request, reply;

	heim_base_once_f(&armor_service_once, &armor_service, fast_armor_init_ipc);
	if (armor_service == NULL) {
	    krb5_set_error_message(context, ENOENT, "Failed to open fast armor service");
            ret = ENOENT;
	    goto out;
	}

	krb5_data_zero(&reply);

	request.data = rk_UNCONST(realm);
	request.length = strlen(realm);

	ret = heim_ipc_call(armor_service, &request, &reply, NULL);
	heim_release(send);
	if (ret) {
	    krb5_set_error_message(context, ret, "Failed to get armor service credential");
	    goto out;
	}

	ret = decode_KERB_ARMOR_SERVICE_REPLY(reply.data, reply.length, &msg, NULL);
	krb5_data_free(&reply);
	if (ret)
	    goto out;

	ret = copy_KrbFastArmor(fxarmor, &msg.armor);
	if (ret) {
	    free_KERB_ARMOR_SERVICE_REPLY(&msg);
	    goto out;
	}

	ret = krb5_copy_keyblock_contents(context, &msg.armor_key, &state->armor_key);
	free_KERB_ARMOR_SERVICE_REPLY(&msg);
	if (ret)
	    goto out;

	ret = krb5_crypto_init(context, &state->armor_key, 0, &state->armor_crypto);
	if (ret)
	    goto out;
#endif /* WIN32 */
    } else {

	fxarmor->armor_type = 1;

	ret = _krb5_make_fast_ap_fxarmor(context,
					 state->armor_ccache,
					 &fxarmor->armor_value,
					 &state->armor_key,
					 &state->armor_crypto);
	if (ret)
	    goto out;
    }
    

    *armor = fxarmor;
    fxarmor = NULL;
 out:
    if (fxarmor) {
	free_KrbFastArmor(fxarmor);
	free(fxarmor);
    }
    return ret;
}

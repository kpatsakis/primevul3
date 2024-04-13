fast_unwrap_as_rep(krb5_context context, int32_t nonce,
		   krb5_data *chksumdata,
		   struct fast_state *state, AS_REP *rep)
{
    PA_FX_FAST_REPLY fxfastrep;
    KrbFastResponse fastrep;
    krb5_error_code ret;
    PA_DATA *pa = NULL;
    int idx = 0;

    if (state->armor_crypto == NULL || rep->padata == NULL)
	return check_fast(context, state);

    /* find PA_FX_FAST_REPLY */

    pa = krb5_find_padata(rep->padata->val, rep->padata->len,
			  KRB5_PADATA_FX_FAST, &idx);
    if (pa == NULL)
	return check_fast(context, state);

    memset(&fxfastrep, 0, sizeof(fxfastrep));
    memset(&fastrep, 0, sizeof(fastrep));

    ret = decode_PA_FX_FAST_REPLY(pa->padata_value.data, pa->padata_value.length, &fxfastrep, NULL);
    if (ret)
	return ret;

    if (fxfastrep.element == choice_PA_FX_FAST_REPLY_armored_data) {
	krb5_data data;
	ret = krb5_decrypt_EncryptedData(context,
					 state->armor_crypto,
					 KRB5_KU_FAST_REP,
					 &fxfastrep.u.armored_data.enc_fast_rep,
					 &data);
	if (ret)
	    goto out;

	ret = decode_KrbFastResponse(data.data, data.length, &fastrep, NULL);
	krb5_data_free(&data);
	if (ret)
	    goto out;

    } else {
	ret = KRB5KDC_ERR_PREAUTH_FAILED;
	goto out;
    }

    free_METHOD_DATA(rep->padata);
    ret = copy_METHOD_DATA(&fastrep.padata, rep->padata);
    if (ret)
	goto out;

    if (fastrep.strengthen_key) {
	if (state->strengthen_key)
	    krb5_free_keyblock(context, state->strengthen_key);

	ret = krb5_copy_keyblock(context, fastrep.strengthen_key, &state->strengthen_key);
	if (ret)
	    goto out;
    }

    if (nonce != fastrep.nonce) {
	ret = KRB5KDC_ERR_PREAUTH_FAILED;
	goto out;
    }
    if (fastrep.finished) {
	PrincipalName cname;
	krb5_realm crealm = NULL;

	if (chksumdata == NULL) {
	    ret = KRB5KDC_ERR_PREAUTH_FAILED;
	    goto out;
	}

	ret = krb5_verify_checksum(context, state->armor_crypto,
				   KRB5_KU_FAST_FINISHED,
				   chksumdata->data, chksumdata->length,
				   &fastrep.finished->ticket_checksum);
	if (ret)
	    goto out;

	/* update */
	ret = copy_Realm(&fastrep.finished->crealm, &crealm);
	if (ret)
	    goto out;
	free_Realm(&rep->crealm);
	rep->crealm = crealm;

	ret = copy_PrincipalName(&fastrep.finished->cname, &cname);
	if (ret)
	    goto out;
	free_PrincipalName(&rep->cname);
	rep->cname = cname;
	
#if 0 /* store authenticated checksum as kdc-offset */
	fastrep->finished.timestamp;
	fastrep->finished.usec = 0;
#endif

    } else if (chksumdata) {
	/* expected fastrep.finish but didn't get it */
	ret = KRB5KDC_ERR_PREAUTH_FAILED;
    }

 out:
    free_PA_FX_FAST_REPLY(&fxfastrep);

    return ret;
}

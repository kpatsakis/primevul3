init_as_req (krb5_context context,
	     KDCOptions opts,
	     const krb5_creds *creds,
	     const krb5_addresses *addrs,
	     const krb5_enctype *etypes,
	     AS_REQ *a)
{
    krb5_error_code ret;

    memset(a, 0, sizeof(*a));

    a->pvno = 5;
    a->msg_type = krb_as_req;
    a->req_body.kdc_options = opts;
    a->req_body.cname = malloc(sizeof(*a->req_body.cname));
    if (a->req_body.cname == NULL) {
	ret = krb5_enomem(context);
	goto fail;
    }
    a->req_body.sname = malloc(sizeof(*a->req_body.sname));
    if (a->req_body.sname == NULL) {
	ret = krb5_enomem(context);
	goto fail;
    }

    ret = _krb5_principal2principalname (a->req_body.cname, creds->client);
    if (ret)
	goto fail;
    ret = copy_Realm(&creds->client->realm, &a->req_body.realm);
    if (ret)
	goto fail;

    ret = _krb5_principal2principalname (a->req_body.sname, creds->server);
    if (ret)
	goto fail;

    if(creds->times.starttime) {
	a->req_body.from = malloc(sizeof(*a->req_body.from));
	if (a->req_body.from == NULL) {
	    ret = krb5_enomem(context);
	    goto fail;
	}
	*a->req_body.from = creds->times.starttime;
    }
    if(creds->times.endtime){
	if ((ALLOC(a->req_body.till, 1)) != NULL)
            *a->req_body.till = creds->times.endtime;
        else {
            ret = krb5_enomem(context);
            goto fail;
        }
    }
    if(creds->times.renew_till){
	a->req_body.rtime = malloc(sizeof(*a->req_body.rtime));
	if (a->req_body.rtime == NULL) {
	    ret = krb5_enomem(context);
	    goto fail;
	}
	*a->req_body.rtime = creds->times.renew_till;
    }
    a->req_body.nonce = 0;
    ret = _krb5_init_etype(context,
			   KRB5_PDU_AS_REQUEST,
			   &a->req_body.etype.len,
			   &a->req_body.etype.val,
			   etypes);
    if (ret)
	goto fail;

    /*
     * This means no addresses
     */

    if (addrs && addrs->len == 0) {
	a->req_body.addresses = NULL;
    } else {
	a->req_body.addresses = malloc(sizeof(*a->req_body.addresses));
	if (a->req_body.addresses == NULL) {
	    ret = krb5_enomem(context);
	    goto fail;
	}

	if (addrs)
	    ret = krb5_copy_addresses(context, addrs, a->req_body.addresses);
	else {
	    ret = krb5_get_all_client_addrs (context, a->req_body.addresses);
	    if(ret == 0 && a->req_body.addresses->len == 0) {
		free(a->req_body.addresses);
		a->req_body.addresses = NULL;
	    }
	}
	if (ret)
	    goto fail;
    }

    a->req_body.enc_authorization_data = NULL;
    a->req_body.additional_tickets = NULL;

    a->padata = NULL;

    return 0;
 fail:
    free_AS_REQ(a);
    memset(a, 0, sizeof(*a));
    return ret;
}

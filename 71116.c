_krb5_pk_load_id(krb5_context context,
		 struct krb5_pk_identity **ret_id,
		 const char *user_id,
		 const char *anchor_id,
		 char * const *chain_list,
		 char * const *revoke_list,
		 krb5_prompter_fct prompter,
		 void *prompter_data,
		 char *password)
{
    struct krb5_pk_identity *id = NULL;
    struct prompter p;
    int ret;

    *ret_id = NULL;

    if (anchor_id == NULL) {
	krb5_set_error_message(context, HEIM_PKINIT_NO_VALID_CA,
			       N_("PKINIT: No anchor given", ""));
	return HEIM_PKINIT_NO_VALID_CA;
    }

    /* load cert */

    id = calloc(1, sizeof(*id));
    if (id == NULL)
	return krb5_enomem(context);

    if (user_id) {
	hx509_lock lock;

	ret = hx509_lock_init(context->hx509ctx, &lock);
	if (ret) {
	    pk_copy_error(context, context->hx509ctx, ret, "Failed init lock");
	    goto out;
	}

	if (password && password[0])
	    hx509_lock_add_password(lock, password);

	if (prompter) {
	    p.context = context;
	    p.prompter = prompter;
	    p.prompter_data = prompter_data;

	    ret = hx509_lock_set_prompter(lock, hx_pass_prompter, &p);
	    if (ret) {
		hx509_lock_free(lock);
		goto out;
	    }
	}

	ret = hx509_certs_init(context->hx509ctx, user_id, 0, lock, &id->certs);
        hx509_lock_free(lock);
	if (ret) {
	    pk_copy_error(context, context->hx509ctx, ret,
			  "Failed to init cert certs");
	    goto out;
	}
    } else {
	id->certs = NULL;
    }

    ret = hx509_certs_init(context->hx509ctx, anchor_id, 0, NULL, &id->anchors);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to init anchors");
	goto out;
    }

    ret = hx509_certs_init(context->hx509ctx, "MEMORY:pkinit-cert-chain",
			   0, NULL, &id->certpool);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed to init chain");
	goto out;
    }

    while (chain_list && *chain_list) {
	ret = hx509_certs_append(context->hx509ctx, id->certpool,
				 NULL, *chain_list);
	if (ret) {
	    pk_copy_error(context, context->hx509ctx, ret,
			  "Failed to laod chain %s",
			  *chain_list);
	    goto out;
	}
	chain_list++;
    }

    if (revoke_list) {
	ret = hx509_revoke_init(context->hx509ctx, &id->revokectx);
	if (ret) {
	    pk_copy_error(context, context->hx509ctx, ret,
			  "Failed init revoke list");
	    goto out;
	}

	while (*revoke_list) {
	    ret = hx509_revoke_add_crl(context->hx509ctx,
				       id->revokectx,
				       *revoke_list);
	    if (ret) {
		pk_copy_error(context, context->hx509ctx, ret,
			      "Failed load revoke list");
		goto out;
	    }
	    revoke_list++;
	}
    } else
	hx509_context_set_missing_revoke(context->hx509ctx, 1);

    ret = hx509_verify_init_ctx(context->hx509ctx, &id->verify_ctx);
    if (ret) {
	pk_copy_error(context, context->hx509ctx, ret,
		      "Failed init verify context");
	goto out;
    }

    hx509_verify_attach_anchors(id->verify_ctx, id->anchors);
    hx509_verify_attach_revoke(id->verify_ctx, id->revokectx);

 out:
    if (ret) {
	hx509_verify_destroy_ctx(id->verify_ctx);
	hx509_certs_free(&id->certs);
	hx509_certs_free(&id->anchors);
	hx509_certs_free(&id->certpool);
	hx509_revoke_free(&id->revokectx);
	free(id);
    } else
	*ret_id = id;

    return ret;
}

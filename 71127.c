get_reply_key(krb5_context context,
	      const krb5_data *content,
	      const krb5_data *req_buffer,
	      krb5_keyblock **key)
{
    ReplyKeyPack key_pack;
    krb5_error_code ret;
    size_t size;

    ret = decode_ReplyKeyPack(content->data,
			      content->length,
			      &key_pack,
			      &size);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("PKINIT decoding reply key failed", ""));
	free_ReplyKeyPack(&key_pack);
	return ret;
    }

    {
	krb5_crypto crypto;

	/*
	 * XXX Verify kp.replyKey is a allowed enctype in the
	 * configuration file
	 */

	ret = krb5_crypto_init(context, &key_pack.replyKey, 0, &crypto);
	if (ret) {
	    free_ReplyKeyPack(&key_pack);
	    return ret;
	}

	ret = krb5_verify_checksum(context, crypto, 6,
				   req_buffer->data, req_buffer->length,
				   &key_pack.asChecksum);
	krb5_crypto_destroy(context, crypto);
	if (ret) {
	    free_ReplyKeyPack(&key_pack);
	    return ret;
	}
    }

    *key = malloc (sizeof (**key));
    if (*key == NULL) {
	free_ReplyKeyPack(&key_pack);
	return krb5_enomem(context);
    }

    ret = copy_EncryptionKey(&key_pack.replyKey, *key);
    free_ReplyKeyPack(&key_pack);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("PKINIT failed copying reply key", ""));
	free(*key);
	*key = NULL;
    }

    return ret;
}

get_reply_key_win(krb5_context context,
		  const krb5_data *content,
		  unsigned nonce,
		  krb5_keyblock **key)
{
    ReplyKeyPack_Win2k key_pack;
    krb5_error_code ret;
    size_t size;

    ret = decode_ReplyKeyPack_Win2k(content->data,
				    content->length,
				    &key_pack,
				    &size);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("PKINIT decoding reply key failed", ""));
	free_ReplyKeyPack_Win2k(&key_pack);
	return ret;
    }

    if ((unsigned)key_pack.nonce != nonce) {
	krb5_set_error_message(context, ret,
			       N_("PKINIT enckey nonce is wrong", ""));
	free_ReplyKeyPack_Win2k(&key_pack);
	return KRB5KRB_AP_ERR_MODIFIED;
    }

    *key = malloc (sizeof (**key));
    if (*key == NULL) {
	free_ReplyKeyPack_Win2k(&key_pack);
	return krb5_enomem(context);
    }

    ret = copy_EncryptionKey(&key_pack.replyKey, *key);
    free_ReplyKeyPack_Win2k(&key_pack);
    if (ret) {
	krb5_set_error_message(context, ret,
			       N_("PKINIT failed copying reply key", ""));
	free(*key);
	*key = NULL;
    }

    return ret;
}

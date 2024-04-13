krb5_init_creds_set_keyblock(krb5_context context,
			     krb5_init_creds_context ctx,
			     krb5_keyblock *keyblock)
{
    ctx->keyseed = (void *)keyblock;
    ctx->keyproc = keyblock_key_proc;

    return 0;
}

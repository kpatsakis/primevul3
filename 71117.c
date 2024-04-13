_krb5_pk_mk_ContentInfo(krb5_context context,
			const krb5_data *buf,
			const heim_oid *oid,
			struct ContentInfo *content_info)
{
    krb5_error_code ret;

    ret = der_copy_oid(oid, &content_info->contentType);
    if (ret)
	return ret;
    ALLOC(content_info->content, 1);
    if (content_info->content == NULL)
	return ENOMEM;
    content_info->content->data = malloc(buf->length);
    if (content_info->content->data == NULL)
	return ENOMEM;
    memcpy(content_info->content->data, buf->data, buf->length);
    content_info->content->length = buf->length;
    return 0;
}

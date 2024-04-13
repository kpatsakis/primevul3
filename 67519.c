X509_VERIFY_PARAM_add0_policy(X509_VERIFY_PARAM *param, ASN1_OBJECT *policy)
{
	if (!param->policies) {
		param->policies = sk_ASN1_OBJECT_new_null();
		if (!param->policies)
			return 0;
	}
	if (!sk_ASN1_OBJECT_push(param->policies, policy))
		return 0;
	return 1;
}

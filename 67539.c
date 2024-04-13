X509_VERIFY_PARAM_set1_policies(X509_VERIFY_PARAM *param,
    STACK_OF(ASN1_OBJECT) *policies)
{
	int i;
	ASN1_OBJECT *oid, *doid;

	if (!param)
		return 0;
	if (param->policies)
		sk_ASN1_OBJECT_pop_free(param->policies, ASN1_OBJECT_free);

	if (!policies) {
		param->policies = NULL;
		return 1;
	}

	param->policies = sk_ASN1_OBJECT_new_null();
	if (!param->policies)
		return 0;

	for (i = 0; i < sk_ASN1_OBJECT_num(policies); i++) {
		oid = sk_ASN1_OBJECT_value(policies, i);
		doid = OBJ_dup(oid);
		if (!doid)
			return 0;
		if (!sk_ASN1_OBJECT_push(param->policies, doid)) {
			ASN1_OBJECT_free(doid);
			return 0;
		}
	}
	param->flags |= X509_V_FLAG_POLICY_CHECK;
	return 1;
}

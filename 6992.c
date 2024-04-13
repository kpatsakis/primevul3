bool RWFunction::GetVoidValue(const char *name, const std::type_info &valueType, void *pValue) const
{
	return GetValueHelper(this, name, valueType, pValue).Assignable()
		CRYPTOPP_GET_FUNCTION_ENTRY(Modulus)
		;
}
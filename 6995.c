bool InvertibleRWFunction::GetVoidValue(const char *name, const std::type_info &valueType, void *pValue) const
{
	return GetValueHelper<RWFunction>(this, name, valueType, pValue).Assignable()
		CRYPTOPP_GET_FUNCTION_ENTRY(Prime1)
		CRYPTOPP_GET_FUNCTION_ENTRY(Prime2)
		CRYPTOPP_GET_FUNCTION_ENTRY(MultiplicativeInverseOfPrime2ModPrime1)
		;
}
void InvertibleRWFunction::AssignFrom(const NameValuePairs &source)
{
	AssignFromHelper<RWFunction>(this, source)
		CRYPTOPP_SET_FUNCTION_ENTRY(Prime1)
		CRYPTOPP_SET_FUNCTION_ENTRY(Prime2)
		CRYPTOPP_SET_FUNCTION_ENTRY(MultiplicativeInverseOfPrime2ModPrime1)
		;
}
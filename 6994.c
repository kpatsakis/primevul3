Integer RWFunction::ApplyFunction(const Integer &in) const
{
	DoQuickSanityCheck();

	Integer out = in.Squared()%m_n;
	const word r = 12;
	// this code was written to handle both r = 6 and r = 12,
	// but now only r = 12 is used in P1363
	const word r2 = r/2;
	const word r3a = (16 + 5 - r) % 16;	// n%16 could be 5 or 13
	const word r3b = (16 + 13 - r) % 16;
	const word r4 = (8 + 5 - r/2) % 8;	// n%8 == 5
	switch (out % 16)
	{
	case r:
		break;
	case r2:
	case r2+8:
		out <<= 1;
		break;
	case r3a:
	case r3b:
		out.Negate();
		out += m_n;
		break;
	case r4:
	case r4+8:
		out.Negate();
		out += m_n;
		out <<= 1;
		break;
	default:
		out = Integer::Zero();
	}
	return out;
}
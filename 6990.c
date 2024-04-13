bool RWFunction::Validate(RandomNumberGenerator &rng, unsigned int level) const
{
	bool pass = true;
	pass = pass && m_n > Integer::One() && m_n%8 == 5;
	return pass;
}
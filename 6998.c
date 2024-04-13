bool InvertibleRWFunction::Validate(RandomNumberGenerator &rng, unsigned int level) const
{
	bool pass = RWFunction::Validate(rng, level);
	pass = pass && m_p > Integer::One() && m_p%8 == 3 && m_p < m_n;
	pass = pass && m_q > Integer::One() && m_q%8 == 7 && m_q < m_n;
	pass = pass && m_u.IsPositive() && m_u < m_p;
	if (level >= 1)
	{
		pass = pass && m_p * m_q == m_n;
		pass = pass && m_u * m_q % m_p == 1;
	}
	if (level >= 2)
		pass = pass && VerifyPrime(rng, m_p, level-2) && VerifyPrime(rng, m_q, level-2);
	return pass;
}
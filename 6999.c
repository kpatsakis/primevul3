void InvertibleRWFunction::DEREncode(BufferedTransformation &bt) const
{
	DERSequenceEncoder seq(bt);
	m_n.DEREncode(seq);
	m_p.DEREncode(seq);
	m_q.DEREncode(seq);
	m_u.DEREncode(seq);
	seq.MessageEnd();
}
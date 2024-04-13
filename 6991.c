void RWFunction::DEREncode(BufferedTransformation &bt) const
{
	DERSequenceEncoder seq(bt);
	m_n.DEREncode(seq);
	seq.MessageEnd();
}
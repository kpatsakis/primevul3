void InvertibleRWFunction::BERDecode(BufferedTransformation &bt)
{
	BERSequenceDecoder seq(bt);
	m_n.BERDecode(seq);
	m_p.BERDecode(seq);
	m_q.BERDecode(seq);
	m_u.BERDecode(seq);
	seq.MessageEnd();
}
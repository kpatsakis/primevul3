void RWFunction::BERDecode(BufferedTransformation &bt)
{
	BERSequenceDecoder seq(bt);
	m_n.BERDecode(seq);
	seq.MessageEnd();
}
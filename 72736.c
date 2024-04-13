ber_out_sequence(STREAM out, STREAM content)
{
	size_t length;
	length = (content ? s_length(content) : 0);
	ber_out_header(out, BER_TAG_SEQUENCE | BER_TAG_CONSTRUCTED, length);
	if (content)
		out_stream(out, content);
}

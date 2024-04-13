cliprdr_send_data(uint8 * data, uint32 length)
{
	logger(Clipboard, Debug, "cliprdr_send_data(), length %d bytes", length);
	cliprdr_send_packet(CLIPRDR_DATA_RESPONSE, CLIPRDR_RESPONSE, data, length);
}

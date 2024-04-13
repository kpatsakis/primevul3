cliprdr_send_simple_native_format_announce(uint32 format)
{
	uint8 buffer[36];

	logger(Clipboard, Debug, "cliprdr_send_simple_native_format_announce() format 0x%x",
	       format);

	buf_out_uint32(buffer, format);
	memset(buffer + 4, 0, sizeof(buffer) - 4);	/* description */
	cliprdr_send_native_format_announce(buffer, sizeof(buffer));
}

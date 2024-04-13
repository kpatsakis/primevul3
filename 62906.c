static UINT drdynvc_write_variable_uint(wStream* s, UINT32 val)
{
	UINT cb;

	if (val <= 0xFF)
	{
		cb = 0;
		Stream_Write_UINT8(s, val);
	}
	else if (val <= 0xFFFF)
	{
		cb = 1;
		Stream_Write_UINT16(s, val);
	}
	else
	{
		cb = 2;
		Stream_Write_UINT32(s, val);
	}

	return cb;
}

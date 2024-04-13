format_timestamp(uint32_t timestamp)
{
	static char buffer[32];

	if ((timestamp & 0xff000000) == 0xff000000)
		snprintf(buffer, sizeof(buffer), "boot + %us", timestamp &
		0x00ffffff);
	else
		snprintf(buffer, sizeof(buffer), "%us", timestamp);
	return buffer;
}

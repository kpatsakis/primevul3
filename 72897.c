seamless_send_persistent(RD_BOOL enable)
{
	unsigned int res;
	if (!g_seamless_rdp)
		return (unsigned int) -1;

	logger(Core, Debug, "seamless_send_persistent(), %s persistent seamless mode",
	       enable ? "enable" : "disable");
	res = seamless_send("PERSISTENT", "%d", enable);

	return res;
}

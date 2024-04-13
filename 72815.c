rdp_disconnect(void)
{
 	logger(Protocol, Debug, "%s()", __func__);
 	sec_disconnect();
 }

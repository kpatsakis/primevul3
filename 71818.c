ping_device (u2fh_devs * devs, unsigned index)
{
  unsigned char data[1] = { 0 };
  unsigned char resp[1024];
  size_t resplen = sizeof (resp);
  return u2fh_sendrecv (devs, index, U2FHID_PING, data, sizeof (data), resp,
			&resplen);
}

u2fh_get_device_description (u2fh_devs * devs, unsigned index, char *out,
			     size_t * len)
{
  size_t i;
  struct u2fdevice *dev = get_device (devs, index);

  if (!dev)
    {
      return U2FH_NO_U2F_DEVICE;
    }
  i = strlen (dev->device_string);
  if (i < *len)
    {
      *len = i;
    }
  else
    {
      return U2FH_MEMORY_ERROR;
    }
  strcpy (out, dev->device_string);
  return U2FH_OK;
}

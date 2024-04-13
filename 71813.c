get_device (u2fh_devs * devs, unsigned id)
{
  struct u2fdevice *dev;
  for (dev = devs->first; dev != NULL; dev = dev->next)
    {
      if (dev->id == id)
	{
	  return dev;
	}
    }
  return NULL;
}

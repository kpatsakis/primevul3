close_devices (u2fh_devs * devs)
{
  struct u2fdevice *dev;
  if (devs == NULL)
    {
      return;
    }
  dev = devs->first;

  while (dev)
    {
      dev = close_device (devs, dev);
    }
}

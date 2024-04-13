close_device (u2fh_devs * devs, struct u2fdevice *dev)
{
  struct u2fdevice *next = dev->next;
  hid_close (dev->devh);
  free (dev->device_path);
  free (dev->device_string);
  if (dev == devs->first)
    {
      devs->first = next;
      free (dev);
    }
  else
    {
      struct u2fdevice *d;
      for (d = devs->first; d != NULL; d = d->next)
	{
	  if (d->next == dev)
	    {
	      d->next = next;
	      free (dev);
	      break;
	    }
	}
    }
  return next;
}

get_usages (struct hid_device_info *dev, unsigned short *usage_page,
	    unsigned short *usage)
{
#ifdef __linux
  int res, desc_size;
  int ret = U2FH_TRANSPORT_ERROR;
  struct hidraw_report_descriptor rpt_desc;
  int handle = open (dev->path, O_RDWR);
  if (handle > 0)
    {
      memset (&rpt_desc, 0, sizeof (rpt_desc));
      res = ioctl (handle, HIDIOCGRDESCSIZE, &desc_size);
      if (res >= 0)
	{
	  rpt_desc.size = desc_size;
	  res = ioctl (handle, HIDIOCGRDESC, &rpt_desc);
	  if (res >= 0)
	    {
	      res =
		get_usage (rpt_desc.value, rpt_desc.size, usage_page, usage);
	      if (res >= 0)
		{
		  ret = U2FH_OK;
		}
	    }
	}
      close (handle);
    }
  return ret;
#else
  *usage_page = dev->usage_page;
  *usage = dev->usage;
  return U2FH_OK;
#endif
}

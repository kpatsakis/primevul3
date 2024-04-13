u2fh_devs_discover (u2fh_devs * devs, unsigned *max_index)
{
  struct hid_device_info *di, *cur_dev;
  u2fh_rc res = U2FH_NO_U2F_DEVICE;
  struct u2fdevice *dev;

  di = hid_enumerate (0, 0);
  for (cur_dev = di; cur_dev; cur_dev = cur_dev->next)
    {
      int found = 0;
      unsigned short usage_page = 0, usage = 0;

      /* check if we already opened this device */
      for (dev = devs->first; dev != NULL; dev = dev->next)
	{
	  if (strcmp (dev->device_path, cur_dev->path) == 0)
	    {
	      if (ping_device (devs, dev->id) == U2FH_OK)
		{
		  found = 1;
		  res = U2FH_OK;
		}
	      else
		{
		  if (debug)
		    {
		      fprintf (stderr, "Device %s failed ping, dead.\n",
			       dev->device_path);
		    }
		  close_device (devs, dev);
		}
	      break;
	    }
	}
      if (found)
	{
	  continue;
	}

      get_usages (cur_dev, &usage_page, &usage);
      if (usage_page == FIDO_USAGE_PAGE && usage == FIDO_USAGE_U2FHID)
	{
	  dev = new_device (devs);
	  dev->devh = hid_open_path (cur_dev->path);
	  if (dev->devh != NULL)
	    {
	      dev->device_path = strdup (cur_dev->path);
	      if (dev->device_path == NULL)
		{
		  close_device (devs, dev);
		  goto out;
		}
	      if (init_device (devs, dev) == U2FH_OK)
		{
		  if (cur_dev->product_string)
		    {
		      size_t len =
			wcstombs (NULL, cur_dev->product_string, 0);
		      dev->device_string = malloc (len + 1);
		      if (dev->device_string == NULL)
			{
			  close_device (devs, dev);
			  goto out;
			}
		      memset (dev->device_string, 0, len + 1);
		      wcstombs (dev->device_string, cur_dev->product_string,
				len);
		      if (debug)
			{
			  fprintf (stderr, "device %s discovered as '%s'\n",
				   dev->device_path, dev->device_string);
			  fprintf (stderr,
				   "  version (Interface, Major, "
				   "Minor, Build): %d, %d, "
				   "%d, %d  capFlags: %d\n",
				   dev->versionInterface,
				   dev->versionMajor,
				   dev->versionMinor,
				   dev->versionBuild, dev->capFlags);
			}
		    }
		  res = U2FH_OK;
		  continue;
		}
	    }
	  close_device (devs, dev);
	}
    }


  /* loop through all open devices and make sure we find them in the enumeration */
  dev = devs->first;
  while (dev)
    {
      int found = 0;

      for (cur_dev = di; cur_dev; cur_dev = cur_dev->next)
	{
	  if (strcmp (cur_dev->path, dev->device_path) == 0)
	    {
	      found = 1;
	      dev = dev->next;
	      break;
	    }
	}
      if (!found)
	{
	  if (debug)
	    {
	      fprintf (stderr, "device %s looks dead.\n", dev->device_path);
	    }
	  dev = close_device (devs, dev);
	}
    }

out:
  hid_free_enumeration (di);
  if (res == U2FH_OK && max_index)
    *max_index = devs->max_id - 1;

  return res;
}

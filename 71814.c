get_usage (uint8_t * report_descriptor, size_t size,
	   unsigned short *usage_page, unsigned short *usage)
{
  size_t i = 0;
  int size_code;
  int data_len, key_size;
  int usage_found = 0, usage_page_found = 0;

  while (i < size)
    {
      int key = report_descriptor[i];
      int key_cmd = key & 0xfc;

      if ((key & 0xf0) == 0xf0)
	{
	  fprintf (stderr, "invalid data received.\n");
	  return -1;
	}
      else
	{
	  size_code = key & 0x3;
	  switch (size_code)
	    {
	    case 0:
	    case 1:
	    case 2:
	      data_len = size_code;
	      break;
	    case 3:
	      data_len = 4;
	      break;
	    default:
	      /* Can't ever happen since size_code is & 0x3 */
	      data_len = 0;
	      break;
	    };
	  key_size = 1;
	}

      if (key_cmd == 0x4)
	{
	  *usage_page = get_bytes (report_descriptor, size, data_len, i);
	  usage_page_found = 1;
	}
      if (key_cmd == 0x8)
	{
	  *usage = get_bytes (report_descriptor, size, data_len, i);
	  usage_found = 1;
	}

      if (usage_page_found && usage_found)
	return 0;		/* success */

      i += data_len + key_size;
    }

  return -1;			/* failure */
}

png_handle_sCAL(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_charp ep;
#ifdef PNG_FLOATING_POINT_SUPPORTED
   double width, height;
   png_charp vp;
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
   png_charp swidth, sheight;
#endif
#endif
   png_size_t slength;

   png_debug(1, "in png_handle_sCAL");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before sCAL");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid sCAL after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_sCAL))
   {
      png_warning(png_ptr, "Duplicate sCAL chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   /* Need unit type, width, \0, height: minimum 4 bytes */
   else if (length < 4)
   {
      png_warning(png_ptr, "sCAL chunk too short");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_debug1(2, "Allocating and reading sCAL chunk data (%lu bytes)",
      length + 1);
   png_ptr->chunkdata = (png_charp)png_malloc_warn(png_ptr, length + 1);
   if (png_ptr->chunkdata == NULL)
   {
      png_warning(png_ptr, "Out of memory while processing sCAL chunk");
      png_crc_finish(png_ptr, length);
      return;
   }
   slength = (png_size_t)length;
   png_crc_read(png_ptr, (png_bytep)png_ptr->chunkdata, slength);

   if (png_crc_finish(png_ptr, 0))
   {
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

   png_ptr->chunkdata[slength] = 0x00; /* Null terminate the last string */

   ep = png_ptr->chunkdata + 1;        /* Skip unit byte */

#ifdef PNG_FLOATING_POINT_SUPPORTED
   width = png_strtod(png_ptr, ep, &vp);
   if (*vp)
   {
      png_warning(png_ptr, "malformed width string in sCAL chunk");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
   swidth = (png_charp)png_malloc_warn(png_ptr, png_strlen(ep) + 1);
   if (swidth == NULL)
   {
      png_warning(png_ptr, "Out of memory while processing sCAL chunk width");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }
   png_memcpy(swidth, ep, (png_size_t)png_strlen(ep));
#endif
#endif

   for (ep = png_ptr->chunkdata; *ep; ep++)
      /* Empty loop */ ;
   ep++;

   if (png_ptr->chunkdata + slength < ep)
   {
      png_warning(png_ptr, "Truncated sCAL chunk");
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
      png_free(png_ptr, swidth);
#endif
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }

#ifdef PNG_FLOATING_POINT_SUPPORTED
   height = png_strtod(png_ptr, ep, &vp);
   if (*vp)
   {
      png_warning(png_ptr, "malformed height string in sCAL chunk");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
      png_free(png_ptr, swidth);
#endif
      return;
   }
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
   sheight = (png_charp)png_malloc_warn(png_ptr, png_strlen(ep) + 1);
   if (sheight == NULL)
   {
      png_warning(png_ptr, "Out of memory while processing sCAL chunk height");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
      png_free(png_ptr, swidth);
#endif
      return;
   }
   png_memcpy(sheight, ep, (png_size_t)png_strlen(ep));
#endif
#endif

   if (png_ptr->chunkdata + slength < ep
#ifdef PNG_FLOATING_POINT_SUPPORTED
      || width <= 0. || height <= 0.
#endif
      )
   {
      png_warning(png_ptr, "Invalid sCAL data");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
      png_free(png_ptr, swidth);
      png_free(png_ptr, sheight);
#endif
      return;
   }


#ifdef PNG_FLOATING_POINT_SUPPORTED
   png_set_sCAL(png_ptr, info_ptr, png_ptr->chunkdata[0], width, height);
#else
#ifdef PNG_FIXED_POINT_SUPPORTED
   png_set_sCAL_s(png_ptr, info_ptr, png_ptr->chunkdata[0], swidth, sheight);
#endif
#endif

   png_free(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
#if defined(PNG_FIXED_POINT_SUPPORTED) && !defined(PNG_FLOATING_POINT_SUPPORTED)
   png_free(png_ptr, swidth);
   png_free(png_ptr, sheight);
#endif
}

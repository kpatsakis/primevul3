png_handle_zTXt(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_textp text_ptr;
   png_charp text;
   int comp_type;
   int ret;
   png_size_t slength, prefix_len, data_len;

   png_debug(1, "in png_handle_zTXt");

#ifdef PNG_USER_LIMITS_SUPPORTED
   if (png_ptr->user_chunk_cache_max != 0)
   {
      if (png_ptr->user_chunk_cache_max == 1)
      {
         png_crc_finish(png_ptr, length);
         return;
      }
      if (--png_ptr->user_chunk_cache_max == 1)
      {
         png_warning(png_ptr, "No space in chunk cache for zTXt");
         png_crc_finish(png_ptr, length);
         return;
      }
   }
#endif

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before zTXt");

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

#ifdef PNG_MAX_MALLOC_64K
   /* We will no doubt have problems with chunks even half this size, but
      there is no hard and fast rule to tell us where to stop. */
   if (length > (png_uint_32)65535L)
   {
     png_warning(png_ptr, "zTXt chunk too large to fit in memory");
     png_crc_finish(png_ptr, length);
     return;
   }
#endif

   png_free(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = (png_charp)png_malloc_warn(png_ptr, length + 1);
   if (png_ptr->chunkdata == NULL)
   {
     png_warning(png_ptr, "Out of memory processing zTXt chunk.");
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

   png_ptr->chunkdata[slength] = 0x00;

   for (text = png_ptr->chunkdata; *text; text++)
      /* Empty loop */ ;

   /* zTXt must have some text after the chunkdataword */
   if (text >= png_ptr->chunkdata + slength - 2)
   {
      png_warning(png_ptr, "Truncated zTXt chunk");
      png_free(png_ptr, png_ptr->chunkdata);
      png_ptr->chunkdata = NULL;
      return;
   }
   else
   {
       comp_type = *(++text);
       if (comp_type != PNG_TEXT_COMPRESSION_zTXt)
       {
          png_warning(png_ptr, "Unknown compression type in zTXt chunk");
          comp_type = PNG_TEXT_COMPRESSION_zTXt;
       }
       text++;        /* Skip the compression_method byte */
   }
   prefix_len = text - png_ptr->chunkdata;

   png_decompress_chunk(png_ptr, comp_type,
     (png_size_t)length, prefix_len, &data_len);

   text_ptr = (png_textp)png_malloc_warn(png_ptr,
      (png_uint_32)png_sizeof(png_text));
   if (text_ptr == NULL)
   {
     png_warning(png_ptr, "Not enough memory to process zTXt chunk.");
     png_free(png_ptr, png_ptr->chunkdata);
     png_ptr->chunkdata = NULL;
     return;
   }
   text_ptr->compression = comp_type;
   text_ptr->key = png_ptr->chunkdata;
#ifdef PNG_iTXt_SUPPORTED
   text_ptr->lang = NULL;
   text_ptr->lang_key = NULL;
   text_ptr->itxt_length = 0;
#endif
   text_ptr->text = png_ptr->chunkdata + prefix_len;
   text_ptr->text_length = data_len;

   ret = png_set_text_2(png_ptr, info_ptr, text_ptr, 1);

   png_free(png_ptr, text_ptr);
   png_free(png_ptr, png_ptr->chunkdata);
   png_ptr->chunkdata = NULL;
   if (ret)
     png_error(png_ptr, "Insufficient memory to store zTXt chunk.");
}

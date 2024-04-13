png_handle_tIME(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_byte buf[7];
   png_time mod_time;

   png_debug(1, "in png_handle_tIME");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Out of place tIME chunk");
   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_tIME))
   {
      png_warning(png_ptr, "Duplicate tIME chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (png_ptr->mode & PNG_HAVE_IDAT)
      png_ptr->mode |= PNG_AFTER_IDAT;

   if (length != 7)
   {
      png_warning(png_ptr, "Incorrect tIME chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, 7);
   if (png_crc_finish(png_ptr, 0))
      return;

   mod_time.second = buf[6];
   mod_time.minute = buf[5];
   mod_time.hour = buf[4];
   mod_time.day = buf[3];
   mod_time.month = buf[2];
   mod_time.year = png_get_uint_16(buf);

   png_set_tIME(png_ptr, info_ptr, &mod_time);
}

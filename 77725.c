png_handle_bKGD(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_size_t truelen;
   png_byte buf[6];

   png_debug(1, "in png_handle_bKGD");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before bKGD");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid bKGD after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE &&
            !(png_ptr->mode & PNG_HAVE_PLTE))
   {
      png_warning(png_ptr, "Missing PLTE before bKGD");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_bKGD))
   {
      png_warning(png_ptr, "Duplicate bKGD chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
      truelen = 1;
   else if (png_ptr->color_type & PNG_COLOR_MASK_COLOR)
      truelen = 6;
   else
      truelen = 2;

   if (length != truelen)
   {
      png_warning(png_ptr, "Incorrect bKGD chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, truelen);
   if (png_crc_finish(png_ptr, 0))
      return;

   /* We convert the index value into RGB components so that we can allow
    * arbitrary RGB values for background when we have transparency, and
    * so it is easy to determine the RGB values of the background color
    * from the info_ptr struct. */
   if (png_ptr->color_type == PNG_COLOR_TYPE_PALETTE)
   {
      png_ptr->background.index = buf[0];
      if (info_ptr && info_ptr->num_palette)
      {
          if (buf[0] >= info_ptr->num_palette)
          {
             png_warning(png_ptr, "Incorrect bKGD chunk index value");
             return;
          }
          png_ptr->background.red =
             (png_uint_16)png_ptr->palette[buf[0]].red;
          png_ptr->background.green =
             (png_uint_16)png_ptr->palette[buf[0]].green;
          png_ptr->background.blue =
             (png_uint_16)png_ptr->palette[buf[0]].blue;
      }
   }
   else if (!(png_ptr->color_type & PNG_COLOR_MASK_COLOR)) /* GRAY */
   {
      png_ptr->background.red =
      png_ptr->background.green =
      png_ptr->background.blue =
      png_ptr->background.gray = png_get_uint_16(buf);
   }
   else
   {
      png_ptr->background.red = png_get_uint_16(buf);
      png_ptr->background.green = png_get_uint_16(buf + 2);
      png_ptr->background.blue = png_get_uint_16(buf + 4);
   }

   png_set_bKGD(png_ptr, info_ptr, &(png_ptr->background));
}

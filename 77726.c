png_handle_cHRM(png_structp png_ptr, png_infop info_ptr, png_uint_32 length)
{
   png_byte buf[32];
#ifdef PNG_FLOATING_POINT_SUPPORTED
   float white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y;
#endif
   png_fixed_point int_x_white, int_y_white, int_x_red, int_y_red, int_x_green,
      int_y_green, int_x_blue, int_y_blue;

   png_uint_32 uint_x, uint_y;

   png_debug(1, "in png_handle_cHRM");

   if (!(png_ptr->mode & PNG_HAVE_IHDR))
      png_error(png_ptr, "Missing IHDR before cHRM");
   else if (png_ptr->mode & PNG_HAVE_IDAT)
   {
      png_warning(png_ptr, "Invalid cHRM after IDAT");
      png_crc_finish(png_ptr, length);
      return;
   }
   else if (png_ptr->mode & PNG_HAVE_PLTE)
      /* Should be an error, but we can cope with it */
      png_warning(png_ptr, "Missing PLTE before cHRM");

   if (info_ptr != NULL && (info_ptr->valid & PNG_INFO_cHRM)
#ifdef PNG_READ_sRGB_SUPPORTED
      && !(info_ptr->valid & PNG_INFO_sRGB)
#endif
      )
   {
      png_warning(png_ptr, "Duplicate cHRM chunk");
      png_crc_finish(png_ptr, length);
      return;
   }

   if (length != 32)
   {
      png_warning(png_ptr, "Incorrect cHRM chunk length");
      png_crc_finish(png_ptr, length);
      return;
   }

   png_crc_read(png_ptr, buf, 32);
   if (png_crc_finish(png_ptr, 0))
      return;

   uint_x = png_get_uint_32(buf);
   uint_y = png_get_uint_32(buf + 4);
   int_x_white = (png_fixed_point)uint_x;
   int_y_white = (png_fixed_point)uint_y;

   uint_x = png_get_uint_32(buf + 8);
   uint_y = png_get_uint_32(buf + 12);
   int_x_red = (png_fixed_point)uint_x;
   int_y_red = (png_fixed_point)uint_y;

   uint_x = png_get_uint_32(buf + 16);
   uint_y = png_get_uint_32(buf + 20);
   int_x_green = (png_fixed_point)uint_x;
   int_y_green = (png_fixed_point)uint_y;

   uint_x = png_get_uint_32(buf + 24);
   uint_y = png_get_uint_32(buf + 28);
   int_x_blue = (png_fixed_point)uint_x;
   int_y_blue = (png_fixed_point)uint_y;

#ifdef PNG_FLOATING_POINT_SUPPORTED
   white_x = (float)int_x_white / (float)100000.0;
   white_y = (float)int_y_white / (float)100000.0;
   red_x   = (float)int_x_red   / (float)100000.0;
   red_y   = (float)int_y_red   / (float)100000.0;
   green_x = (float)int_x_green / (float)100000.0;
   green_y = (float)int_y_green / (float)100000.0;
   blue_x  = (float)int_x_blue  / (float)100000.0;
   blue_y  = (float)int_y_blue  / (float)100000.0;
#endif

#ifdef PNG_READ_sRGB_SUPPORTED
   if ((info_ptr != NULL) && (info_ptr->valid & PNG_INFO_sRGB))
      {
      if (PNG_OUT_OF_RANGE(int_x_white, 31270,  1000) ||
          PNG_OUT_OF_RANGE(int_y_white, 32900,  1000) ||
          PNG_OUT_OF_RANGE(int_x_red,   64000L, 1000) ||
          PNG_OUT_OF_RANGE(int_y_red,   33000,  1000) ||
          PNG_OUT_OF_RANGE(int_x_green, 30000,  1000) ||
          PNG_OUT_OF_RANGE(int_y_green, 60000L, 1000) ||
          PNG_OUT_OF_RANGE(int_x_blue,  15000,  1000) ||
          PNG_OUT_OF_RANGE(int_y_blue,   6000,  1000))
         {
            png_warning(png_ptr,
              "Ignoring incorrect cHRM value when sRGB is also present");
#ifdef PNG_CONSOLE_IO_SUPPORTED
#ifdef PNG_FLOATING_POINT_SUPPORTED
            fprintf(stderr, "wx=%f, wy=%f, rx=%f, ry=%f\n",
               white_x, white_y, red_x, red_y);
            fprintf(stderr, "gx=%f, gy=%f, bx=%f, by=%f\n",
               green_x, green_y, blue_x, blue_y);
#else
            fprintf(stderr, "wx=%ld, wy=%ld, rx=%ld, ry=%ld\n",
               (long)int_x_white, (long)int_y_white,
               (long)int_x_red, (long)int_y_red);
            fprintf(stderr, "gx=%ld, gy=%ld, bx=%ld, by=%ld\n",
               (long)int_x_green, (long)int_y_green,
               (long)int_x_blue, (long)int_y_blue);
#endif
#endif /* PNG_CONSOLE_IO_SUPPORTED */
         }
         return;
      }
#endif /* PNG_READ_sRGB_SUPPORTED */

#ifdef PNG_FLOATING_POINT_SUPPORTED
   png_set_cHRM(png_ptr, info_ptr,
      white_x, white_y, red_x, red_y, green_x, green_y, blue_x, blue_y);
#endif
#ifdef PNG_FIXED_POINT_SUPPORTED
   png_set_cHRM_fixed(png_ptr, info_ptr,
      int_x_white, int_y_white, int_x_red, int_y_red, int_x_green,
      int_y_green, int_x_blue, int_y_blue);
#endif
}

png_crc_read(png_structp png_ptr, png_bytep buf, png_size_t length)
{
   if (png_ptr == NULL)
      return;
   png_read_data(png_ptr, buf, length);
   png_calculate_crc(png_ptr, buf, length);
}

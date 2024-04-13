png_zlib_inflate(png_structrp png_ptr, int flush)
{
   if (png_ptr->zstream_start && png_ptr->zstream.avail_in > 0)
   {
      if ((*png_ptr->zstream.next_in >> 4) > 7)
      {
         png_ptr->zstream.msg = "invalid window size (libpng)";
         return Z_DATA_ERROR;
      }

      png_ptr->zstream_start = 0;
   }

   return inflate(&png_ptr->zstream, flush);
}

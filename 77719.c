png_check_chunk_name(png_structp png_ptr, png_bytep chunk_name)
{
   png_debug(1, "in png_check_chunk_name");
   if (isnonalpha(chunk_name[0]) || isnonalpha(chunk_name[1]) ||
       isnonalpha(chunk_name[2]) || isnonalpha(chunk_name[3]))
   {
      png_chunk_error(png_ptr, "invalid chunk type");
   }
}

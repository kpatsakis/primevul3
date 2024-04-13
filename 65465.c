png_do_read_interlace(png_row_infop row_info, png_bytep row, int pass,
    png_uint_32 transformations /* Because these may affect the byte layout */)
{
   /* Arrays to facilitate easy interlacing - use pass (0 - 6) as index */
   /* Offset to next interlace block */
   static PNG_CONST unsigned int png_pass_inc[7] = {8, 8, 4, 4, 2, 2, 1};

   png_debug(1, "in png_do_read_interlace");
   if (row != NULL && row_info != NULL)
   {
      png_uint_32 final_width;

      final_width = row_info->width * png_pass_inc[pass];

      switch (row_info->pixel_depth)
      {
         case 1:
         {
            png_bytep sp = row + (size_t)((row_info->width - 1) >> 3);
            png_bytep dp = row + (size_t)((final_width - 1) >> 3);
            unsigned int sshift, dshift;
            unsigned int s_start, s_end;
            int s_inc;
            int jstop = (int)png_pass_inc[pass];
            png_byte v;
            png_uint_32 i;
            int j;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if ((transformations & PNG_PACKSWAP) != 0)
            {
                sshift = ((row_info->width + 7) & 0x07);
                dshift = ((final_width + 7) & 0x07);
                s_start = 7;
                s_end = 0;
                s_inc = -1;
            }

            else
#endif
            {
                sshift = 7 - ((row_info->width + 7) & 0x07);
                dshift = 7 - ((final_width + 7) & 0x07);
                s_start = 0;
                s_end = 7;
                s_inc = 1;
            }

            for (i = 0; i < row_info->width; i++)
            {
               v = (png_byte)((*sp >> sshift) & 0x01);
               for (j = 0; j < jstop; j++)
               {
                  unsigned int tmp = *dp & (0x7f7f >> (7 - dshift));
                  tmp |= (unsigned int)(v << dshift);
                  *dp = (png_byte)(tmp & 0xff);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift = (unsigned int)((int)dshift + s_inc);
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift = (unsigned int)((int)sshift + s_inc);
            }
            break;
         }

         case 2:
         {
            png_bytep sp = row + (png_uint_32)((row_info->width - 1) >> 2);
            png_bytep dp = row + (png_uint_32)((final_width - 1) >> 2);
            unsigned int sshift, dshift;
            unsigned int s_start, s_end;
            int s_inc;
            int jstop = (int)png_pass_inc[pass];
            png_uint_32 i;

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if ((transformations & PNG_PACKSWAP) != 0)
            {
               sshift = (((row_info->width + 3) & 0x03) << 1);
               dshift = (((final_width + 3) & 0x03) << 1);
               s_start = 6;
               s_end = 0;
               s_inc = -2;
            }

            else
#endif
            {
               sshift = ((3 - ((row_info->width + 3) & 0x03)) << 1);
               dshift = ((3 - ((final_width + 3) & 0x03)) << 1);
               s_start = 0;
               s_end = 6;
               s_inc = 2;
            }

            for (i = 0; i < row_info->width; i++)
            {
               png_byte v;
               int j;

               v = (png_byte)((*sp >> sshift) & 0x03);
               for (j = 0; j < jstop; j++)
               {
                  unsigned int tmp = *dp & (0x3f3f >> (6 - dshift));
                  tmp |= (unsigned int)(v << dshift);
                  *dp = (png_byte)(tmp & 0xff);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift = (unsigned int)((int)dshift + s_inc);
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift = (unsigned int)((int)sshift + s_inc);
            }
            break;
         }

         case 4:
         {
            png_bytep sp = row + (size_t)((row_info->width - 1) >> 1);
            png_bytep dp = row + (size_t)((final_width - 1) >> 1);
            unsigned int sshift, dshift;
            unsigned int s_start, s_end;
            int s_inc;
            png_uint_32 i;
            int jstop = (int)png_pass_inc[pass];

#ifdef PNG_READ_PACKSWAP_SUPPORTED
            if ((transformations & PNG_PACKSWAP) != 0)
            {
               sshift = (((row_info->width + 1) & 0x01) << 2);
               dshift = (((final_width + 1) & 0x01) << 2);
               s_start = 4;
               s_end = 0;
               s_inc = -4;
            }

            else
#endif
            {
               sshift = ((1 - ((row_info->width + 1) & 0x01)) << 2);
               dshift = ((1 - ((final_width + 1) & 0x01)) << 2);
               s_start = 0;
               s_end = 4;
               s_inc = 4;
            }

            for (i = 0; i < row_info->width; i++)
            {
               png_byte v = (png_byte)((*sp >> sshift) & 0x0f);
               int j;

               for (j = 0; j < jstop; j++)
               {
                  unsigned int tmp = *dp & (0xf0f >> (4 - dshift));
                  tmp |= (unsigned int)(v << dshift);
                  *dp = (png_byte)(tmp & 0xff);

                  if (dshift == s_end)
                  {
                     dshift = s_start;
                     dp--;
                  }

                  else
                     dshift = (unsigned int)((int)dshift + s_inc);
               }

               if (sshift == s_end)
               {
                  sshift = s_start;
                  sp--;
               }

               else
                  sshift = (unsigned int)((int)sshift + s_inc);
            }
            break;
         }

         default:
         {
            size_t pixel_bytes = (row_info->pixel_depth >> 3);

            png_bytep sp = row + (size_t)(row_info->width - 1)
                * pixel_bytes;

            png_bytep dp = row + (size_t)(final_width - 1) * pixel_bytes;

            int jstop = (int)png_pass_inc[pass];
            png_uint_32 i;

            for (i = 0; i < row_info->width; i++)
            {
               png_byte v[8]; /* SAFE; pixel_depth does not exceed 64 */
               int j;

               memcpy(v, sp, pixel_bytes);

               for (j = 0; j < jstop; j++)
               {
                  memcpy(dp, v, pixel_bytes);
                  dp -= pixel_bytes;
               }

               sp -= pixel_bytes;
            }
            break;
         }
      }

      row_info->width = final_width;
      row_info->rowbytes = PNG_ROWBYTES(row_info->pixel_depth, final_width);
   }
#ifndef PNG_READ_PACKSWAP_SUPPORTED
   PNG_UNUSED(transformations)  /* Silence compiler warning */
#endif
}

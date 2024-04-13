pngquant_error rwpng_write_image24(FILE *outfile, const png24_image *mainprog_ptr)
{
    png_structp png_ptr;
    png_infop info_ptr;

    pngquant_error retval = rwpng_write_image_init((rwpng_png_image*)mainprog_ptr, &png_ptr, &info_ptr, 0);
    if (retval) return retval;

    png_init_io(png_ptr, outfile);

    rwpng_set_gamma(info_ptr, png_ptr, mainprog_ptr->gamma, mainprog_ptr->output_color);

    png_set_IHDR(png_ptr, info_ptr, mainprog_ptr->width, mainprog_ptr->height,
                 8, PNG_COLOR_TYPE_RGB_ALPHA,
                 0, PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_BASE);


    png_bytepp row_pointers = rwpng_create_row_pointers(info_ptr, png_ptr, mainprog_ptr->rgba_data, mainprog_ptr->height, 0);

    rwpng_write_end(&info_ptr, &png_ptr, row_pointers);

    free(row_pointers);

    return SUCCESS;
}

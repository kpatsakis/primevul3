static void user_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    struct rwpng_read_data *read_data = (struct rwpng_read_data *)png_get_io_ptr(png_ptr);

    png_size_t read = fread(data, 1, length, read_data->fp);
    if (!read) {
        png_error(png_ptr, "Read error");
    }
    read_data->bytes_read += read;
}

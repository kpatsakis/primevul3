static void user_write_data(png_structp png_ptr, png_bytep data, png_size_t length)
{
    struct rwpng_write_state *write_state = (struct rwpng_write_state *)png_get_io_ptr(png_ptr);

    if (SUCCESS != write_state->retval) {
        return;
    }

    if (!fwrite(data, length, 1, write_state->outfile)) {
        write_state->retval = CANT_WRITE_ERROR;
    }

    write_state->bytes_written += length;
}

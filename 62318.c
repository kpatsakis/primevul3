static png_bytepp rwpng_create_row_pointers(png_infop info_ptr, png_structp png_ptr, unsigned char *base, unsigned int height, png_size_t rowbytes)
{
    if (!rowbytes) {
        rowbytes = png_get_rowbytes(png_ptr, info_ptr);
    }

    png_bytepp row_pointers = malloc(height * sizeof(row_pointers[0]));
    if (!row_pointers) return NULL;
    for(size_t row = 0; row < height; row++) {
        row_pointers[row] = base + row * rowbytes;
    }
    return row_pointers;
}

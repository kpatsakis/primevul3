void rwpng_free_image24(png24_image *image)
{
    free(image->row_pointers);
    image->row_pointers = NULL;

    free(image->rgba_data);
    image->rgba_data = NULL;

    rwpng_free_chunks(image->chunks);
    image->chunks = NULL;
}

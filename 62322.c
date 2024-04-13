void rwpng_free_image8(png8_image *image)
{
    free(image->indexed_data);
    image->indexed_data = NULL;

    free(image->row_pointers);
    image->row_pointers = NULL;

    rwpng_free_chunks(image->chunks);
    image->chunks = NULL;
}

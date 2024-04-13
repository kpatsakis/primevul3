static int read_chunk_callback(png_structp png_ptr, png_unknown_chunkp in_chunk)
{
    if (0 == memcmp("iCCP", in_chunk->name, 5) ||
        0 == memcmp("cHRM", in_chunk->name, 5) ||
        0 == memcmp("gAMA", in_chunk->name, 5)) {
        return 0; // not handled
    }

    struct rwpng_chunk **head = (struct rwpng_chunk **)png_get_user_chunk_ptr(png_ptr);

    struct rwpng_chunk *chunk = malloc(sizeof(struct rwpng_chunk));
    memcpy(chunk->name, in_chunk->name, 5);
    chunk->size = in_chunk->size;
    chunk->location = in_chunk->location;
    chunk->data = in_chunk->size ? malloc(in_chunk->size) : NULL;
    if (in_chunk->size) {
        memcpy(chunk->data, in_chunk->data, in_chunk->size);
    }

    chunk->next = *head;
    *head = chunk;

    return 1; // marks as "handled", libpng won't store it
}

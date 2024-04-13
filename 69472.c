crm_ipc_decompress(crm_ipc_t * client)
{
    struct crm_ipc_response_header *header = (struct crm_ipc_response_header *)(void*)client->buffer;

    if (header->size_compressed) {
        int rc = 0;
        unsigned int size_u = 1 + header->size_uncompressed;
        /* never let buf size fall below our max size required for ipc reads. */
        unsigned int new_buf_size = QB_MAX((hdr_offset + size_u), client->max_buf_size);
        char *uncompressed = calloc(1, new_buf_size);

        crm_trace("Decompressing message data %u bytes into %u bytes",
                 header->size_compressed, size_u);

        rc = BZ2_bzBuffToBuffDecompress(uncompressed + hdr_offset, &size_u,
                                        client->buffer + hdr_offset, header->size_compressed, 1, 0);

        if (rc != BZ_OK) {
            crm_err("Decompression failed: %s (%d)", bz2_strerror(rc), rc);
            free(uncompressed);
            return -EILSEQ;
        }

        /*
         * This assert no longer holds true.  For an identical msg, some clients may
         * require compression, and others may not. If that same msg (event) is sent
         * to multiple clients, it could result in some clients receiving a compressed
         * msg even though compression was not explicitly required for them.
         *
         * CRM_ASSERT((header->size_uncompressed + hdr_offset) >= ipc_buffer_max);
         */
        CRM_ASSERT(size_u == header->size_uncompressed);

        memcpy(uncompressed, client->buffer, hdr_offset);       /* Preserve the header */
        header = (struct crm_ipc_response_header *)(void*)uncompressed;

        free(client->buffer);
        client->buf_size = new_buf_size;
        client->buffer = uncompressed;
    }

    CRM_ASSERT(client->buffer[hdr_offset + header->size_uncompressed - 1] == 0);
    return pcmk_ok;
}

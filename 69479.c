crm_ipc_prepare(uint32_t request, xmlNode * message, struct iovec ** result, uint32_t max_send_size)
{
    static unsigned int biggest = 0;
    struct iovec *iov;
    unsigned int total = 0;
    char *compressed = NULL;
    char *buffer = dump_xml_unformatted(message);
    struct crm_ipc_response_header *header = calloc(1, sizeof(struct crm_ipc_response_header));

    CRM_ASSERT(result != NULL);

    crm_ipc_init();

    if (max_send_size == 0) {
        max_send_size = ipc_buffer_max;
    }

    CRM_LOG_ASSERT(max_send_size != 0);

    *result = NULL;
    iov = calloc(2, sizeof(struct iovec));


    iov[0].iov_len = hdr_offset;
    iov[0].iov_base = header;

    header->version = PCMK_IPC_VERSION;
    header->size_uncompressed = 1 + strlen(buffer);
    total = iov[0].iov_len + header->size_uncompressed;

    if (total < max_send_size) {
        iov[1].iov_base = buffer;
        iov[1].iov_len = header->size_uncompressed;

    } else {
        unsigned int new_size = 0;

        if (crm_compress_string
            (buffer, header->size_uncompressed, max_send_size, &compressed, &new_size)) {

            header->flags |= crm_ipc_compressed;
            header->size_compressed = new_size;

            iov[1].iov_len = header->size_compressed;
            iov[1].iov_base = compressed;

            free(buffer);

            biggest = QB_MAX(header->size_compressed, biggest);

        } else {
            ssize_t rc = -EMSGSIZE;

            crm_log_xml_trace(message, "EMSGSIZE");
            biggest = QB_MAX(header->size_uncompressed, biggest);

            crm_err
                ("Could not compress the message (%u bytes) into less than the configured ipc limit (%u bytes). "
                 "Set PCMK_ipc_buffer to a higher value (%u bytes suggested)",
                 header->size_uncompressed, max_send_size, 4 * biggest);

            free(compressed);
            free(buffer);
            free(header);
            free(iov);

            return rc;
        }
    }

    header->qb.size = iov[0].iov_len + iov[1].iov_len;
    header->qb.id = (int32_t)request;    /* Replying to a specific request */

    *result = iov;
    CRM_ASSERT(header->qb.size > 0);
    return header->qb.size;
}

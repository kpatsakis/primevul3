crm_ipcs_recv(crm_client_t * c, void *data, size_t size, uint32_t * id, uint32_t * flags)
{
    xmlNode *xml = NULL;
    char *uncompressed = NULL;
    char *text = ((char *)data) + sizeof(struct crm_ipc_response_header);
    struct crm_ipc_response_header *header = data;

    if (id) {
        *id = ((struct qb_ipc_response_header *)data)->id;
    }
    if (flags) {
        *flags = header->flags;
    }

    if (is_set(header->flags, crm_ipc_proxied)) {
        /* mark this client as being the endpoint of a proxy connection.
         * Proxy connections responses are sent on the event channel to avoid
         * blocking the proxy daemon (crmd) */
        c->flags |= crm_client_flag_ipc_proxied;
    }

    if(header->version > PCMK_IPC_VERSION) {
        crm_err("Filtering incompatible v%d IPC message, we only support versions <= %d",
                header->version, PCMK_IPC_VERSION);
        return NULL;
    }

    if (header->size_compressed) {
        int rc = 0;
        unsigned int size_u = 1 + header->size_uncompressed;
        uncompressed = calloc(1, size_u);

        crm_trace("Decompressing message data %u bytes into %u bytes",
                  header->size_compressed, size_u);

        rc = BZ2_bzBuffToBuffDecompress(uncompressed, &size_u, text, header->size_compressed, 1, 0);
        text = uncompressed;

        if (rc != BZ_OK) {
            crm_err("Decompression failed: %s (%d)", bz2_strerror(rc), rc);
            free(uncompressed);
            return NULL;
        }
    }

    CRM_ASSERT(text[header->size_uncompressed - 1] == 0);

    crm_trace("Received %.200s", text);
    xml = string2xml(text);

    free(uncompressed);
    return xml;
}

crm_ipc_buffer_flags(crm_ipc_t * client)
{
    struct crm_ipc_response_header *header = NULL;

    CRM_ASSERT(client != NULL);
    if (client->buffer == NULL) {
        return 0;
    }

    header = (struct crm_ipc_response_header *)(void*)client->buffer;
    return header->flags;
}

crm_ipcs_send_ack(crm_client_t * c, uint32_t request, uint32_t flags, const char *tag, const char *function,
                  int line)
{
    if (flags & crm_ipc_client_response) {
        xmlNode *ack = create_xml_node(NULL, tag);

        crm_trace("Ack'ing msg from %s (%p)", crm_client_name(c), c);
        c->request_id = 0;
        crm_xml_add(ack, "function", function);
        crm_xml_add_int(ack, "line", line);
        crm_ipcs_send(c, request, ack, flags);
        free_xml(ack);
    }
}

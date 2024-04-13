HandleBlockDNSMessage(const block_dns_message_t *msg, undo_lists_t *lists)
{
    DWORD err = 0;
    block_dns_data_t *interface_data;
    HANDLE engine = NULL;
    LPCWSTR exe_path;

#ifdef UNICODE
    exe_path = settings.exe_path;
#else
    WCHAR wide_path[MAX_PATH];
    MultiByteToWideChar(CP_UTF8, 0, settings.exe_path, MAX_PATH, wide_path, MAX_PATH);
    exe_path = wide_path;
#endif

    if (msg->header.type == msg_add_block_dns)
    {
        err = add_block_dns_filters(&engine, msg->iface.index, exe_path, BlockDNSErrHandler);
        if (!err)
        {
            interface_data = malloc(sizeof(block_dns_data_t));
            if (!interface_data)
            {
                return ERROR_OUTOFMEMORY;
            }
            interface_data->engine = engine;
            interface_data->index = msg->iface.index;
            int is_auto = 0;
            interface_data->metric_v4 = get_interface_metric(msg->iface.index,
                                                             AF_INET, &is_auto);
            if (is_auto)
            {
                interface_data->metric_v4 = 0;
            }
            interface_data->metric_v6 = get_interface_metric(msg->iface.index,
                                                             AF_INET6, &is_auto);
            if (is_auto)
            {
                interface_data->metric_v6 = 0;
            }
            err = AddListItem(&(*lists)[block_dns], interface_data);
            if (!err)
            {
                err = set_interface_metric(msg->iface.index, AF_INET,
                                           BLOCK_DNS_IFACE_METRIC);
                if (!err)
                {
                    set_interface_metric(msg->iface.index, AF_INET6,
                                         BLOCK_DNS_IFACE_METRIC);
                }
            }
        }
    }
    else
    {
        interface_data = RemoveListItem(&(*lists)[block_dns], CmpEngine, NULL);
        if (interface_data)
        {
            engine = interface_data->engine;
            err = delete_block_dns_filters(engine);
            engine = NULL;
            if (interface_data->metric_v4 >= 0)
            {
                set_interface_metric(msg->iface.index, AF_INET,
                                     interface_data->metric_v4);
            }
            if (interface_data->metric_v6 >= 0)
            {
                set_interface_metric(msg->iface.index, AF_INET6,
                                     interface_data->metric_v6);
            }
            free(interface_data);
        }
        else
        {
            MsgToEventLog(M_ERR, TEXT("No previous block DNS filters to delete"));
        }
    }

    if (err && engine)
    {
        delete_block_dns_filters(engine);
    }

    return err;
}

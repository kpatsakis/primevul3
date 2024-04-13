crm_client_disconnect_all(qb_ipcs_service_t *service)
{
    qb_ipcs_connection_t *c = NULL;

    if (service == NULL) {
        return;
    }

    c = qb_ipcs_connection_first_get(service);

    while (c != NULL) {
        qb_ipcs_connection_t *last = c;

        c = qb_ipcs_connection_next_get(service, last);

        /* There really shouldn't be anyone connected at this point */
        crm_notice("Disconnecting client %p, pid=%d...", last, crm_ipcs_client_pid(last));
        qb_ipcs_disconnect(last);
        qb_ipcs_connection_unref(last);
    }
}

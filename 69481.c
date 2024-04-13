crm_ipc_ready(crm_ipc_t *client)
{
    int rc;

    CRM_ASSERT(client != NULL);

    if (crm_ipc_connected(client) == FALSE) {
        return -ENOTCONN;
    }

    client->pfd.revents = 0;
    rc = poll(&(client->pfd), 1, 0);
    return (rc < 0)? -errno : rc;
}

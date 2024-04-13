crm_ipc_get_fd(crm_ipc_t * client)
{
    int fd = 0;

    if (client && client->ipc && (qb_ipcc_fd_get(client->ipc, &fd) == 0)) {
        return fd;
    }
    errno = EINVAL;
    crm_perror(LOG_ERR, "Could not obtain file IPC descriptor for %s",
               (client? client->name : "unspecified client"));
    return -errno;
}

virDomainInterfaceFree(virDomainInterfacePtr iface)
{
    size_t i;

    if (!iface)
        return;

    VIR_FREE(iface->name);
    VIR_FREE(iface->hwaddr);

    for (i = 0; i < iface->naddrs; i++)
        VIR_FREE(iface->addrs[i].addr);
    VIR_FREE(iface->addrs);

    VIR_FREE(iface);
}

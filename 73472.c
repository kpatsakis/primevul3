virDomainMemoryPeek(virDomainPtr dom,
                    unsigned long long start /* really 64 bits */,
                    size_t size,
                    void *buffer,
                    unsigned int flags)
{
    virConnectPtr conn;

    VIR_DOMAIN_DEBUG(dom, "start=%lld, size=%zi, buffer=%p, flags=%x",
                     start, size, buffer, flags);

    virResetLastError();

    virCheckDomainReturn(dom, -1);
    conn = dom->conn;

    virCheckReadOnlyGoto(conn->flags, error);

    /* Note on access to physical memory: A VIR_MEMORY_PHYSICAL flag is
     * a possibility.  However it isn't really useful unless the caller
     * can also access registers, particularly CR3 on x86 in order to
     * get the Page Table Directory.  Since registers are different on
     * every architecture, that would imply another call to get the
     * machine registers.
     *
     * The QEMU driver handles VIR_MEMORY_VIRTUAL, mapping it
     * to the qemu 'memsave' command which does the virtual to physical
     * mapping inside qemu.
     *
     * The QEMU driver also handles VIR_MEMORY_PHYSICAL, mapping it
     * to the qemu 'pmemsave' command.
     *
     * At time of writing there is no Xen driver.  However the Xen
     * hypervisor only lets you map physical pages from other domains,
     * and so the Xen driver would have to do the virtual to physical
     * mapping by chasing 2, 3 or 4-level page tables from the PTD.
     * There is example code in libxc (xc_translate_foreign_address)
     * which does this, although we cannot copy this code directly
     * because of incompatible licensing.
     */

    VIR_EXCLUSIVE_FLAGS_GOTO(VIR_MEMORY_VIRTUAL, VIR_MEMORY_PHYSICAL, error);

    /* Allow size == 0 as an access test. */
    if (size > 0)
        virCheckNonNullArgGoto(buffer, error);

    if (conn->driver->domainMemoryPeek) {
        int ret;
        ret = conn->driver->domainMemoryPeek(dom, start, size,
                                             buffer, flags);
        if (ret < 0)
            goto error;
        return ret;
    }

    virReportUnsupportedError();

 error:
    virDispatchError(dom->conn);
    return -1;
}

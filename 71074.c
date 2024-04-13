fast_armor_init_ipc(void *ctx)
{
    heim_ipc *ipc = ctx;
    heim_ipc_init_context("ANY:org.h5l.armor-service", ipc);
}

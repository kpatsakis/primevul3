ofproto_unixctl_init(void)
{
    static bool registered;
    if (registered) {
        return;
    }
    registered = true;

    unixctl_command_register("ofproto/list", "", 0, 0,
                             ofproto_unixctl_list, NULL);
}

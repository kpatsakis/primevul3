static void opt_debug(void)
{
    config.debug = 1;
    snprintf(config.logfilename, sizeof(config.logfilename), "-");
}

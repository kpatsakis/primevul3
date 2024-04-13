write_via_header (int fd, orderedmap hashofheaders,
                  unsigned int major, unsigned int minor)
{
        char hostname[512];
        char *data;
        int ret;

        if (config->disable_viaheader) {
                ret = 0;
                goto done;
        }

        if (config->via_proxy_name) {
                strlcpy (hostname, config->via_proxy_name, sizeof (hostname));
        } else if (gethostname (hostname, sizeof (hostname)) < 0) {
                strlcpy (hostname, "unknown", 512);
        }

        /*
         * See if there is a "Via" header.  If so, again we need to do a bit
         * of processing.
         */
        data = orderedmap_find (hashofheaders, "via");
        if (data) {
                ret = write_message (fd,
                                     "Via: %s, %hu.%hu %s (%s/%s)\r\n",
                                     data, major, minor, hostname, PACKAGE,
                                     VERSION);

                orderedmap_remove (hashofheaders, "via");
        } else {
                ret = write_message (fd,
                                     "Via: %hu.%hu %s (%s/%s)\r\n",
                                     major, minor, hostname, PACKAGE, VERSION);
        }

done:
        return ret;
}
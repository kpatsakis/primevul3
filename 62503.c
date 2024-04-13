static int dump_suid_policy()
{
    /*
     - values are:
       0 - don't dump suided programs - in this case the hook is not called by kernel
       1 - create coredump readable by fs_uid
       2 - create coredump readable by root only
    */
    int c;
    int suid_dump_policy = 0;
    const char *filename = "/proc/sys/fs/suid_dumpable";
    FILE *f  = fopen(filename, "r");
    if (!f)
    {
        log("Can't open %s", filename);
        return suid_dump_policy;
    }

    c = fgetc(f);
    fclose(f);
    if (c != EOF)
        suid_dump_policy = c - '0';

    return suid_dump_policy;
}

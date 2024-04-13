static int dumpacl(struct protstream *pin, struct protstream *pout,
                   const char *mboxname, const char *acl_in)
{
    int r = 0;
    char tag[128];
    int tagnum = 1;
    char *rights, *nextid;
    char *acl_safe = acl_in ? xstrdup(acl_in) : NULL;
    char *acl = acl_safe;

    while (acl) {
        rights = strchr(acl, '\t');
        if (!rights) break;
        *rights++ = '\0';

        nextid = strchr(rights, '\t');
        if (!nextid) break;
        *nextid++ = '\0';

        snprintf(tag, sizeof(tag), "SACL%d", tagnum++);

        prot_printf(pout, "%s SETACL {" SIZE_T_FMT "+}\r\n%s"
                    " {" SIZE_T_FMT "+}\r\n%s {" SIZE_T_FMT "+}\r\n%s\r\n",
                    tag,
                    strlen(mboxname), mboxname,
                    strlen(acl), acl,
                    strlen(rights), rights);

        r = getresult(pin, tag);
        if (r) break;

        acl = nextid;
    }

    if(acl_safe) free(acl_safe);

    return r;
}

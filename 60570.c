static void free_acl_list(FFServerIPAddressACL *in_acl)
{
    FFServerIPAddressACL *pacl, *pacl2;

    pacl = in_acl;
    while(pacl) {
        pacl2 = pacl;
        pacl = pacl->next;
        av_freep(pacl2);
    }
}

static void free_snippetargs(struct snippetargs **sap)
{
    while (*sap) {
        struct snippetargs *sa = *sap;
        *sap = sa->next;
        free(sa->mboxname);
        free(sa->uids.data);
        free(sa);
    }
}

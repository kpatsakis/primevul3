static void free_windowargs(struct windowargs *wa)
{
    if (!wa)
        return;
    free(wa->anchorfolder);
    free(wa);
}

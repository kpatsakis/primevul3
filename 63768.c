static void init_cs_entry(cs_entry *cs)
{
    cs->data = NULL;
    cs->name = NULL;
    cs->len = 0;
    cs->cslen = 0;
    cs->used = false;
    cs->valid = false;
}

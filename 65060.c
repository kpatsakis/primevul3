FindLedByName(KeyNamesInfo *info, xkb_atom_t name,
              xkb_led_index_t *idx_out)
{
    for (xkb_led_index_t idx = 0; idx < info->num_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];
        if (ledi->name == name) {
            *idx_out = idx;
            return ledi;
        }
    }

    return NULL;
}

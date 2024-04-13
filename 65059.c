FindKeyByName(KeyNamesInfo *info, xkb_atom_t name)
{
    xkb_keycode_t i;

    for (i = info->min_key_code; i <= info->max_key_code; i++)
        if (darray_item(info->key_names, i) == name)
            return i;

    return XKB_KEYCODE_INVALID;
}

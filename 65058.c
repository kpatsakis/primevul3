CopyLedNamesToKeymap(struct xkb_keymap *keymap, KeyNamesInfo *info)
{
    keymap->num_leds = info->num_led_names;
    for (xkb_led_index_t idx = 0; idx < info->num_led_names; idx++) {
        LedNameInfo *ledi = &info->led_names[idx];

        if (ledi->name == XKB_ATOM_NONE)
            continue;

        keymap->leds[idx].name = ledi->name;
    }

    return true;
}

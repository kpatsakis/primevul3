AddLedMap(CompatInfo *info, LedInfo *new, bool same_file)
{
    enum led_field collide;
    const int verbosity = xkb_context_get_log_verbosity(info->ctx);
    const bool report = (same_file && verbosity > 0) || verbosity > 9;

    for (xkb_led_index_t i = 0; i < info->num_leds; i++) {
        LedInfo *old = &info->leds[i];

        if (old->led.name != new->led.name)
            continue;

        if (old->led.mods.mods == new->led.mods.mods &&
            old->led.groups == new->led.groups &&
            old->led.ctrls == new->led.ctrls &&
            old->led.which_mods == new->led.which_mods &&
            old->led.which_groups == new->led.which_groups) {
            old->defined |= new->defined;
            return true;
        }

        if (new->merge == MERGE_REPLACE) {
            if (report)
                log_warn(info->ctx,
                         "Map for indicator %s redefined; "
                         "Earlier definition ignored\n",
                         xkb_atom_text(info->ctx, old->led.name));
            *old = *new;
            return true;
        }

        collide = 0;
        if (UseNewLEDField(LED_FIELD_MODS, old, new, report, &collide)) {
            old->led.which_mods = new->led.which_mods;
            old->led.mods = new->led.mods;
            old->defined |= LED_FIELD_MODS;
        }
        if (UseNewLEDField(LED_FIELD_GROUPS, old, new, report, &collide)) {
            old->led.which_groups = new->led.which_groups;
            old->led.groups = new->led.groups;
            old->defined |= LED_FIELD_GROUPS;
        }
        if (UseNewLEDField(LED_FIELD_CTRLS, old, new, report, &collide)) {
            old->led.ctrls = new->led.ctrls;
            old->defined |= LED_FIELD_CTRLS;
        }

        if (collide) {
            log_warn(info->ctx,
                     "Map for indicator %s redefined; "
                     "Using %s definition for duplicate fields\n",
                     xkb_atom_text(info->ctx, old->led.name),
                     (new->merge == MERGE_AUGMENT ? "first" : "last"));
        }

        return true;
    }

    if (info->num_leds >= XKB_MAX_LEDS) {
        log_err(info->ctx,
                "Too many LEDs defined (maximum %d)\n",
                XKB_MAX_LEDS);
        return false;
    }
    info->leds[info->num_leds++] = *new;
    return true;
}

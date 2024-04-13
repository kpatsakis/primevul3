UseNewLEDField(enum led_field field, LedInfo *old, LedInfo *new,
               bool report, enum led_field *collide)
{
    if (!(old->defined & field))
        return true;

    if (new->defined & field) {
        if (report)
            *collide |= field;

        if (new->merge != MERGE_AUGMENT)
            return true;
    }

    return false;
}

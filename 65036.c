UseNewInterpField(enum si_field field, SymInterpInfo *old, SymInterpInfo *new,
                  bool report, enum si_field *collide)
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

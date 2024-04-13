ReportLedBadType(CompatInfo *info, LedInfo *ledi, const char *field,
                 const char *wanted)
{
    return ReportBadType(info->ctx, "indicator map", field,
                         xkb_atom_text(info->ctx, ledi->led.name),
                         wanted);
}

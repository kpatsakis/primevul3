ReportSIBadType(CompatInfo *info, SymInterpInfo *si, const char *field,
                const char *wanted)
{
    return ReportBadType(info->ctx, "symbol interpretation", field,
                         siText(si, info), wanted);
}

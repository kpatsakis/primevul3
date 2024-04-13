xkb_file_type_to_string(enum xkb_file_type type)
{
    if (type > _FILE_TYPE_NUM_ENTRIES)
        return "unknown";
    return xkb_file_type_strings[type];
}

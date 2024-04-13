static int namespacedata(struct findall_data *data, void *rock)
{
    if (!data) return 0;
    int* sawone = (int*) rock;

    switch (data->mb_category) {
    case MBNAME_INBOX:
    case MBNAME_ALTINBOX:
    case MBNAME_ALTPREFIX:
        sawone[NAMESPACE_INBOX] = 1;
        break;

    case MBNAME_OTHERUSER:
        sawone[NAMESPACE_USER] = 1;
        break;

    case MBNAME_SHARED:
        sawone[NAMESPACE_SHARED] = 1;
        break;
    }

    return 0;
}

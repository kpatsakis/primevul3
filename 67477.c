CmpAddress(LPVOID item, LPVOID address)
{
    return memcmp(item, address, sizeof(MIB_UNICASTIPADDRESS_ROW)) == 0 ? TRUE : FALSE;
}

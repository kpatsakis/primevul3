CmpWString(LPVOID item, LPVOID str)
{
    return (wcscmp(item, str) == 0) ? TRUE : FALSE;
}

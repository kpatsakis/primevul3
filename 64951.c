int IsMyBlock(const cmsUInt8Number* Buffer, cmsUInt32Number n)
{
    int words = 1, space = 0, quot = 0;
    cmsUInt32Number i;

    if (n < 10) return 0;   // Too small

    if (n > 132)
        n = 132;

    for (i = 1; i < n; i++) {

        switch(Buffer[i])
        {
        case '\n':
        case '\r':
            return ((quot == 1) || (words > 2)) ? 0 : words;
        case '\t':
        case ' ':
            if(!quot && !space)
                space = 1;
            break;
        case '\"':
            quot = !quot;
            break;
        default:
            if (Buffer[i] < 32) return 0;
            if (Buffer[i] > 127) return 0;
            words += space;
            space = 0;
            break;
        }
    }

    return 0;
}

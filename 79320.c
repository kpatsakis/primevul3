static inline bool isKanaLetter(UChar character)
{
    if (character >= 0x3041 && character <= 0x3096)
        return true;

    if (character >= 0x30A1 && character <= 0x30FA)
        return true;
    if (character >= 0x31F0 && character <= 0x31FF)
        return true;

    if (character >= 0xFF66 && character <= 0xFF9D && character != 0xFF70)
        return true;

    return false;
}

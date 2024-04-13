static bool isNonLatin1Separator(UChar32 character)
{
    ASSERT_ARG(character, character >= 256);

    return U_GET_GC_MASK(character) & (U_GC_S_MASK | U_GC_P_MASK | U_GC_Z_MASK | U_GC_CF_MASK);
}

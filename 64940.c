cmsBool Check(cmsIT8* it8, SYMBOL sy, const char* Err)
{
        if (it8 -> sy != sy)
                return SynError(it8, NoMeta(Err));
        return TRUE;
}

cmsBool GetVal(cmsIT8* it8, char* Buffer, cmsUInt32Number max, const char* ErrorTitle)
{
    switch (it8->sy) {

    case SEOLN:   // Empty value
                  Buffer[0]=0;
                  break;
    case SIDENT:  strncpy(Buffer, it8->id, max);
                  Buffer[max-1]=0;
                  break;
    case SINUM:   snprintf(Buffer, max, "%d", it8 -> inum); break;
    case SDNUM:   snprintf(Buffer, max, it8->DoubleFormatter, it8 -> dnum); break;
    case SSTRING: strncpy(Buffer, it8->str, max);
                  Buffer[max-1] = 0;
                  break;


    default:
         return SynError(it8, "%s", ErrorTitle);
    }

    Buffer[max] = 0;
    return TRUE;
}

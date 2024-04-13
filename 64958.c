void ReadType(cmsIT8* it8, char* SheetTypePtr)
{
    cmsInt32Number cnt = 0;


    while (isseparator(it8->ch))
            NextCh(it8);

    while (it8->ch != '\r' && it8 ->ch != '\n' && it8->ch != '\t' && it8 -> ch != 0) {

        if (cnt++ < MAXSTR) 
            *SheetTypePtr++= (char) it8 ->ch;
        NextCh(it8);
    }

    *SheetTypePtr = 0;
}

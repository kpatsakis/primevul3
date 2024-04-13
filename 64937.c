void AllocTable(cmsIT8* it8)
{
    TABLE* t;

    t = it8 ->Tab + it8 ->TablesCount;

    t->HeaderList = NULL;
    t->DataFormat = NULL;
    t->Data       = NULL;

    it8 ->TablesCount++;
}

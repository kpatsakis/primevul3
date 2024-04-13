cmsInt32Number CMSEXPORT cmsIT8SetTable(cmsHANDLE  IT8, cmsUInt32Number nTable)
{
     cmsIT8* it8 = (cmsIT8*) IT8;

     if (nTable >= it8 ->TablesCount) {

         if (nTable == it8 ->TablesCount) {

             AllocTable(it8);
         }
         else {
             SynError(it8, "Table %d is out of sequence", nTable);
             return -1;
         }
     }

     it8 ->nTable = nTable;

     return (cmsInt32Number) nTable;
}

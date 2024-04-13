cmsHANDLE  CMSEXPORT cmsIT8LoadFromFile(cmsContext ContextID, const char* cFileName)
{

     cmsHANDLE hIT8;
     cmsIT8*  it8;
     int type;

     _cmsAssert(cFileName != NULL);

     type = IsMyFile(cFileName);
     if (type == 0) return NULL;

     hIT8 = cmsIT8Alloc(ContextID);
     it8 = (cmsIT8*) hIT8;
     if (!hIT8) return NULL;


     it8 ->FileStack[0]->Stream = fopen(cFileName, "rt");

     if (!it8 ->FileStack[0]->Stream) {
         cmsIT8Free(hIT8);
         return NULL;
     }


    strncpy(it8->FileStack[0]->FileName, cFileName, cmsMAX_PATH-1);
    it8->FileStack[0]->FileName[cmsMAX_PATH-1] = 0;

    if (!ParseIT8(it8, type-1)) {

            fclose(it8 ->FileStack[0]->Stream);
            cmsIT8Free(hIT8);
            return NULL;
    }

    CookPointers(it8);
    it8 ->nTable = 0;

    if (fclose(it8 ->FileStack[0]->Stream)!= 0) {
            cmsIT8Free(hIT8);
            return NULL;
    }

    return hIT8;

}

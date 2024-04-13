cmsBool IsMyFile(const char* FileName)
{
   FILE *fp;
   cmsUInt32Number Size;
   cmsUInt8Number Ptr[133];

   fp = fopen(FileName, "rt");
   if (!fp) {
       cmsSignalError(0, cmsERROR_FILE, "File '%s' not found", FileName);
       return FALSE;
   }

   Size = (cmsUInt32Number) fread(Ptr, 1, 132, fp);

   if (fclose(fp) != 0)
       return FALSE;

   Ptr[Size] = '\0';

   return IsMyBlock(Ptr, Size);
}

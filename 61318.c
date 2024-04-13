static void TIFFSetProperties(TIFF *tiff,Image *image)
{
  const char
    *value;

  value=GetImageArtifact(image,"tiff:document");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_DOCUMENTNAME,value);
  value=GetImageArtifact(image,"tiff:hostcomputer");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_HOSTCOMPUTER,value);
  value=GetImageArtifact(image,"tiff:artist");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_ARTIST,value);
  value=GetImageArtifact(image,"tiff:timestamp");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_DATETIME,value);
  value=GetImageArtifact(image,"tiff:make");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_MAKE,value);
  value=GetImageArtifact(image,"tiff:model");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_MODEL,value);
  value=GetImageArtifact(image,"tiff:software");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_SOFTWARE,value);
  value=GetImageArtifact(image,"tiff:copyright");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_COPYRIGHT,value);
  value=GetImageArtifact(image,"kodak-33423");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,33423,value);
  value=GetImageArtifact(image,"kodak-36867");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,36867,value);
  value=GetImageArtifact(image,"label");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_PAGENAME,value);
  value=GetImageArtifact(image,"comment");
  if (value != (const char *) NULL)
    (void) TIFFSetField(tiff,TIFFTAG_IMAGEDESCRIPTION,value);
}

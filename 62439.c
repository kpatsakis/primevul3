MagickExport unsigned int GetMagickSignature(const StringInfo *nonce)
{
  register unsigned char
    *p;

  StringInfo
    *version;

  unsigned int
    signature;

  version=AcquireStringInfo(MagickPathExtent);
  p=GetStringInfoDatum(version);
  signature=MAGICKCORE_QUANTUM_DEPTH;
  (void) memcpy(p,&signature,sizeof(signature));
  p+=sizeof(signature);
  signature=MAGICKCORE_HDRI_ENABLE;
  (void) memcpy(p,&signature,sizeof(signature));
  p+=sizeof(signature);
  signature=MagickLibInterface;
  (void) memcpy(p,&signature,sizeof(signature));
  p+=sizeof(signature);
  signature=1;  /* endianess */
  (void) memcpy(p,&signature,sizeof(signature));
  p+=sizeof(signature);
  SetStringInfoLength(version,p-GetStringInfoDatum(version));
  if (nonce != (const StringInfo *) NULL)
    ConcatenateStringInfo(version,nonce);
  signature=CRC32(GetStringInfoDatum(version),GetStringInfoLength(version));
  version=DestroyStringInfo(version);
  return(signature);
}

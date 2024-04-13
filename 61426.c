static inline const unsigned char *PushFloatPixel(QuantumInfo *quantum_info,
  const unsigned char *magick_restrict pixels,float *pixel)
{
  float
    *p;

  unsigned char
    quantum[4];

  if (quantum_info->endian == LSBEndian)
    {
      quantum[0]=(*pixels++);
      quantum[1]=(*pixels++);
      quantum[2]=(*pixels++);
      quantum[3]=(*pixels++);
      p=(float *) quantum;
      *pixel=(*p);
      *pixel-=quantum_info->minimum;
      *pixel*=quantum_info->scale;
      return(pixels);
    }
  quantum[3]=(*pixels++);
  quantum[2]=(*pixels++);
  quantum[1]=(*pixels++);
  quantum[0]=(*pixels++);
  p=(float *) quantum;
  *pixel=(*p);
  *pixel-=quantum_info->minimum;
  *pixel*=quantum_info->scale;
  return(pixels);
}

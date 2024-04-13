static inline const unsigned char *PushDoublePixel(QuantumInfo *quantum_info,
  const unsigned char *magick_restrict pixels,double *pixel)
{
  double
    *p;

  unsigned char
    quantum[8];

  if (quantum_info->endian == LSBEndian)
    {
      quantum[0]=(*pixels++);
      quantum[1]=(*pixels++);
      quantum[2]=(*pixels++);
      quantum[3]=(*pixels++);
      quantum[4]=(*pixels++);
      quantum[5]=(*pixels++);
      quantum[6]=(*pixels++);
      quantum[7]=(*pixels++);
      p=(double *) quantum;
      *pixel=(*p);
      *pixel-=quantum_info->minimum;
      *pixel*=quantum_info->scale;
      return(pixels);
    }
  quantum[7]=(*pixels++);
  quantum[6]=(*pixels++);
  quantum[5]=(*pixels++);
  quantum[4]=(*pixels++);
  quantum[3]=(*pixels++);
  quantum[2]=(*pixels++);
  quantum[1]=(*pixels++);
  quantum[0]=(*pixels++);
  p=(double *) quantum;
  *pixel=(*p);
  *pixel-=quantum_info->minimum;
  *pixel*=quantum_info->scale;
  return(pixels);
}

MagickExport void SetQuantumScale(QuantumInfo *quantum_info,const double scale)
{
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  quantum_info->scale=scale;
}

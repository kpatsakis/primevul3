MagickExport QuantumFormatType GetQuantumFormat(const QuantumInfo *quantum_info)
{
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  return(quantum_info->format);
}

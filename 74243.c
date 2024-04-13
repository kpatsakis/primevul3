MagickExport void SetQuantumMinIsWhite(QuantumInfo *quantum_info,
  const MagickBooleanType min_is_white)
{
  assert(quantum_info != (QuantumInfo *) NULL);
  assert(quantum_info->signature == MagickSignature);
  quantum_info->min_is_white=min_is_white;
}

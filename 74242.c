MagickExport void SetQuantumImageType(Image *image,
  const QuantumType quantum_type)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  switch (quantum_type)
  {
    case IndexQuantum:
    case IndexAlphaQuantum:
    {
      image->type=PaletteType;
      break;
    }
    case GrayQuantum:
    case GrayAlphaQuantum:
    {
      image->type=GrayscaleType;
      if (image->depth == 1)
        image->type=BilevelType;
      break;
    }
    case CyanQuantum:
    case MagentaQuantum:
    case YellowQuantum:
    case BlackQuantum:
    case CMYKQuantum:
    case CMYKAQuantum:
    {
      image->type=ColorSeparationType;
      break;
    }
    default:
    {
      image->type=TrueColorType;
      break;
    }
  }
}

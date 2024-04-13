static void SetSaturation(double saturation,FPXColorTwistMatrix *color_twist)
{
  FPXColorTwistMatrix
    effect,
    result;

  /*
    Set image saturation in color twist matrix.
  */
  assert(color_twist != (FPXColorTwistMatrix *) NULL);
  effect.byy=1.0;
  effect.byc1=0.0;
  effect.byc2=0.0;
  effect.dummy1_zero=0.0;
  effect.bc1y=0.0;
  effect.bc1c1=saturation;
  effect.bc1c2=0.0;
  effect.dummy2_zero=0.0;
  effect.bc2y=0.0;
  effect.bc2c1=0.0;
  effect.bc2c2=saturation;
  effect.dummy3_zero=0.0;
  effect.dummy4_zero=0.0;
  effect.dummy5_zero=0.0;
  effect.dummy6_zero=0.0;
  effect.dummy7_one=1.0;
  ColorTwistMultiply(*color_twist,effect,&result);
  *color_twist=result;
}

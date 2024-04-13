static void SetColorBalance(double red,double green,double blue,
  FPXColorTwistMatrix *color_twist)
{
  FPXColorTwistMatrix
    blue_effect,
    green_effect,
    result,
    rgb_effect,
    rg_effect,
    red_effect;

  /*
    Set image color balance in color twist matrix.
  */
  assert(color_twist != (FPXColorTwistMatrix *) NULL);
  red=sqrt((double) red)-1.0;
  green=sqrt((double) green)-1.0;
  blue=sqrt((double) blue)-1.0;
  red_effect.byy=1.0;
  red_effect.byc1=0.0;
  red_effect.byc2=0.299*red;
  red_effect.dummy1_zero=0.0;
  red_effect.bc1y=(-0.299)*red;
  red_effect.bc1c1=1.0-0.299*red;
  red_effect.bc1c2=(-0.299)*red;
  red_effect.dummy2_zero=0.0;
  red_effect.bc2y=0.701*red;
  red_effect.bc2c1=0.0;
  red_effect.bc2c2=1.0+0.402*red;
  red_effect.dummy3_zero=0.0;
  red_effect.dummy4_zero=0.0;
  red_effect.dummy5_zero=0.0;
  red_effect.dummy6_zero=0.0;
  red_effect.dummy7_one=1.0;
  green_effect.byy=1.0;
  green_effect.byc1=(-0.114)*green;
  green_effect.byc2=(-0.299)*green;
  green_effect.dummy1_zero=0.0;
  green_effect.bc1y=(-0.587)*green;
  green_effect.bc1c1=1.0-0.473*green;
  green_effect.bc1c2=0.299*green;
  green_effect.dummy2_zero=0.0;
  green_effect.bc2y=(-0.587)*green;
  green_effect.bc2c1=0.114*green;
  green_effect.bc2c2=1.0-0.288*green;
  green_effect.dummy3_zero=0.0;
  green_effect.dummy4_zero=0.0;
  green_effect.dummy5_zero=0.0;
  green_effect.dummy6_zero=0.0;
  green_effect.dummy7_one=1.0;
  blue_effect.byy=1.0;
  blue_effect.byc1=0.114*blue;
  blue_effect.byc2=0.0;
  blue_effect.dummy1_zero=0.0;
  blue_effect.bc1y=0.886*blue;
  blue_effect.bc1c1=1.0+0.772*blue;
  blue_effect.bc1c2=0.0;
  blue_effect.dummy2_zero=0.0;
  blue_effect.bc2y=(-0.114)*blue;
  blue_effect.bc2c1=(-0.114)*blue;
  blue_effect.bc2c2=1.0-0.114*blue;
  blue_effect.dummy3_zero=0.0;
  blue_effect.dummy4_zero=0.0;
  blue_effect.dummy5_zero=0.0;
  blue_effect.dummy6_zero=0.0;
  blue_effect.dummy7_one=1.0;
  ColorTwistMultiply(red_effect,green_effect,&rg_effect);
  ColorTwistMultiply(rg_effect,blue_effect,&rgb_effect);
  ColorTwistMultiply(*color_twist,rgb_effect,&result);
  *color_twist=result;
}

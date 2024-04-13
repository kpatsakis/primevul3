static void ColorTwistMultiply(FPXColorTwistMatrix first,
  FPXColorTwistMatrix second,FPXColorTwistMatrix *color_twist)
{
  /*
    Matrix multiply.
  */
  assert(color_twist != (FPXColorTwistMatrix *) NULL);
  color_twist->byy=(first.byy*second.byy)+(first.byc1*second.bc1y)+
    (first.byc2*second.bc2y)+(first.dummy1_zero*second.dummy4_zero);
  color_twist->byc1=(first.byy*second.byc1)+(first.byc1*second.bc1c1)+
    (first.byc2*second.bc2c1)+(first.dummy1_zero*second.dummy5_zero);
  color_twist->byc2=(first.byy*second.byc2)+(first.byc1*second.bc1c2)+
    (first.byc2*second.bc2c2)+(first.dummy1_zero*second.dummy6_zero);
  color_twist->dummy1_zero=(first.byy*second.dummy1_zero)+
    (first.byc1*second.dummy2_zero)+(first.byc2*second.dummy3_zero)+
    (first.dummy1_zero*second.dummy7_one);
  color_twist->bc1y=(first.bc1y*second.byy)+(first.bc1c1*second.bc1y)+
    (first.bc1c2*second.bc2y)+(first.dummy2_zero*second.dummy4_zero);
  color_twist->bc1c1=(first.bc1y*second.byc1)+(first.bc1c1*second.bc1c1)+
    (first.bc1c2*second.bc2c1)+(first.dummy2_zero*second.dummy5_zero);
  color_twist->bc1c2=(first.bc1y*second.byc2)+(first.bc1c1*second.bc1c2)+
    (first.bc1c2*second.bc2c2)+(first.dummy2_zero*second.dummy6_zero);
  color_twist->dummy2_zero=(first.bc1y*second.dummy1_zero)+
    (first.bc1c1*second.dummy2_zero)+(first.bc1c2*second.dummy3_zero)+
    (first.dummy2_zero*second.dummy7_one);
  color_twist->bc2y=(first.bc2y*second.byy)+(first.bc2c1*second.bc1y)+
    (first.bc2c2*second.bc2y)+(first.dummy3_zero*second.dummy4_zero);
  color_twist->bc2c1=(first.bc2y*second.byc1)+(first.bc2c1*second.bc1c1)+
    (first.bc2c2*second.bc2c1)+(first.dummy3_zero*second.dummy5_zero);
  color_twist->bc2c2=(first.bc2y*second.byc2)+(first.bc2c1*second.bc1c2)+
    (first.bc2c2*second.bc2c2)+(first.dummy3_zero*second.dummy6_zero);
  color_twist->dummy3_zero=(first.bc2y*second.dummy1_zero)+
    (first.bc2c1*second.dummy2_zero)+(first.bc2c2*second.dummy3_zero)+
    (first.dummy3_zero*second.dummy7_one);
  color_twist->dummy4_zero=(first.dummy4_zero*second.byy)+
    (first.dummy5_zero*second.bc1y)+(first.dummy6_zero*second.bc2y)+
    (first.dummy7_one*second.dummy4_zero);
  color_twist->dummy5_zero=(first.dummy4_zero*second.byc1)+
    (first.dummy5_zero*second.bc1c1)+(first.dummy6_zero*second.bc2c1)+
    (first.dummy7_one*second.dummy5_zero);
  color_twist->dummy6_zero=(first.dummy4_zero*second.byc2)+
    (first.dummy5_zero*second.bc1c2)+(first.dummy6_zero*second.bc2c2)+
    (first.dummy7_one*second.dummy6_zero);
  color_twist->dummy7_one=(first.dummy4_zero*second.dummy1_zero)+
    (first.dummy5_zero*second.dummy2_zero)+
    (first.dummy6_zero*second.dummy3_zero)+(first.dummy7_one*second.dummy7_one);
}

static void TraceEllipse(PrimitiveInfo *primitive_info,const PointInfo start,
  const PointInfo stop,const PointInfo degrees)
{
  double
    delta,
    step,
    y;

  PointInfo
    angle,
    point;

  register PrimitiveInfo
    *p;

  register ssize_t
    i;

  /*
    Ellipses are just short segmented polys.
  */
  if ((fabs(stop.x) < DrawEpsilon) && (fabs(stop.y) < DrawEpsilon))
    {
      TracePoint(primitive_info,start);
      return;
    }
  delta=2.0/MagickMax(stop.x,stop.y);
  step=MagickPI/8.0;
  if ((delta >= 0.0) && (delta < (MagickPI/8.0)))
    step=MagickPI/(4*(MagickPI/delta/2+0.5));
  angle.x=DegreesToRadians(degrees.x);
  y=degrees.y;
  while (y < degrees.x)
    y+=360.0;
  angle.y=(double) (DegreesToRadians(y)-DrawEpsilon);
  for (p=primitive_info; angle.x < angle.y; angle.x+=step)
  {
    point.x=cos(fmod(angle.x,DegreesToRadians(360.0)))*stop.x+start.x;
    point.y=sin(fmod(angle.x,DegreesToRadians(360.0)))*stop.y+start.y;
    TracePoint(p,point);
    p+=p->coordinates;
  }
  point.x=cos(fmod(angle.y,DegreesToRadians(360.0)))*stop.x+start.x;
  point.y=sin(fmod(angle.y,DegreesToRadians(360.0)))*stop.y+start.y;
  TracePoint(p,point);
  p+=p->coordinates;
  primitive_info->coordinates=(size_t) (p-primitive_info);
  for (i=0; i < (ssize_t) primitive_info->coordinates; i++)
  {
    p->primitive=primitive_info->primitive;
    p--;
  }
}

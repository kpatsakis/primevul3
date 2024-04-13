static void DrawRoundLinecap(Image *image,const DrawInfo *draw_info,
  const PrimitiveInfo *primitive_info)
{
  PrimitiveInfo
    linecap[5];

  register ssize_t
    i;

  for (i=0; i < 4; i++)
    linecap[i]=(*primitive_info);
  linecap[0].coordinates=4;
  linecap[1].point.x+=2.0*DrawEpsilon;
  linecap[2].point.x+=2.0*DrawEpsilon;
  linecap[2].point.y+=2.0*DrawEpsilon;
  linecap[3].point.y+=2.0*DrawEpsilon;
  linecap[4].primitive=UndefinedPrimitive;
  (void) DrawPolygonPrimitive(image,draw_info,linecap);
}

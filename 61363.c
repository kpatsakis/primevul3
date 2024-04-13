static void draw_fill_color_rgb( wmfAPI* API, const wmfRGB* rgb )
{
  PixelWand
    *fill_color;

  fill_color=NewPixelWand();
  PixelSetRedQuantum(fill_color,ScaleCharToQuantum(rgb->r));
  PixelSetGreenQuantum(fill_color,ScaleCharToQuantum(rgb->g));
  PixelSetBlueQuantum(fill_color,ScaleCharToQuantum(rgb->b));
  PixelSetOpacityQuantum(fill_color,OpaqueOpacity);
  DrawSetFillColor(WmfDrawingWand,fill_color);
  fill_color=DestroyPixelWand(fill_color);
}

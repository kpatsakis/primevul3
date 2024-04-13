void jswrap_graphics_setFontAlign(JsVar *parent, int x, int y, int r) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  if (x<-1) x=-1;
  if (x>1) x=1;
  if (y<-1) y=-1;
  if (y>1) y=1;
  if (r<0) r=0;
  if (r>3) r=3;
  gfx.data.fontAlignX = x;
  gfx.data.fontAlignY = y;
  gfx.data.fontRotate = r;
  graphicsSetVar(&gfx);
}

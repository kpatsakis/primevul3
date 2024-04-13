void  lcdFillRect_ArrayBuffer(struct JsGraphics *gfx, short x1, short y1, short x2, short y2) {
  short y;
  for (y=y1;y<=y2;y++)
    lcdSetPixels_ArrayBuffer(gfx, x1, y, (short)(1+x2-x1), gfx->data.fgColor);
}

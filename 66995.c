unsigned int lcdGetPixelIndex_ArrayBuffer(JsGraphics *gfx, int x, int y, int pixelCount) {
  if (gfx->data.flags & JSGRAPHICSFLAGS_ARRAYBUFFER_ZIGZAG) {
    if (y&1) x = gfx->data.width - (x+pixelCount);
  }
  if (gfx->data.flags & JSGRAPHICSFLAGS_ARRAYBUFFER_VERTICAL_BYTE)
    return (unsigned int)(((x + (y>>3)*gfx->data.width)<<3) | (y&7));
  else
    return (unsigned int)((x + y*gfx->data.width)*gfx->data.bpp);
}

void GraphicsContext3D::paintToCanvas(const unsigned char* imagePixels, int imageWidth, int imageHeight,
                                      int canvasWidth, int canvasHeight, QPainter* context)
{
    QImage image(imagePixels, imageWidth, imageHeight, NativeImageQt::defaultFormatForAlphaEnabledImages());
    context->save();
    context->translate(0, imageHeight);
    context->scale(1, -1);
    context->setCompositionMode(QPainter::CompositionMode_Source);
    context->drawImage(QRect(0, 0, canvasWidth, -canvasHeight), image);
    context->restore();
}

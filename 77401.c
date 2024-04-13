void GraphicsContext::setAlpha(float alpha)
{
    if (paintingDisabled())
        return;

    platformContext()->setAlpha(alpha);
}

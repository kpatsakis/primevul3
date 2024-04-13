QRectF GraphicsContext3DPrivate::boundingRect() const
{
    return QRectF(QPointF(0, 0), QSizeF(m_context->m_currentWidth, m_context->m_currentHeight));
}

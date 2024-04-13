void Element::webkitRequestFullScreen(unsigned short flags)
{
    FullscreenController::from(document())->requestFullScreenForElement(this, (flags | LEGACY_MOZILLA_REQUEST), FullscreenController::EnforceIFrameAllowFullScreenRequirement);
}

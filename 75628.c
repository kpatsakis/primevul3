void ContainerNode::setActive(bool down, bool pause)
{
    if (down == active()) return;

    Node::setActive(down);

    if (renderer()) {
        bool reactsToPress = renderer()->style()->affectedByActiveRules();
        if (reactsToPress)
            setNeedsStyleRecalc();
        if (renderer() && renderer()->style()->hasAppearance()) {
            if (renderer()->theme()->stateChanged(renderer(), PressedState))
                reactsToPress = true;
        }
        if (reactsToPress && pause) {
#ifdef HAVE_FUNC_USLEEP
            double startTime = currentTime();
#endif

            Document::updateStyleForAllDocuments();
            if (renderer())
                renderer()->repaint(true);
            
#ifdef HAVE_FUNC_USLEEP
            double remainingTime = 0.1 - (currentTime() - startTime);
            if (remainingTime > 0)
                usleep(static_cast<useconds_t>(remainingTime * 1000000.0));
#endif
        }
    }
}

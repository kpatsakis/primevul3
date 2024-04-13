ConvolverNode::ConvolverNode(AudioContext* context, float sampleRate)
    : AudioNode(context, sampleRate)
    , m_normalize(true)
{
    ScriptWrappable::init(this);
    addInput(adoptPtr(new AudioNodeInput(this)));
    addOutput(adoptPtr(new AudioNodeOutput(this, 2)));

    m_channelCount = 2;
    m_channelCountMode = ClampedMax;
    m_channelInterpretation = AudioBus::Speakers;

    setNodeType(NodeTypeConvolver);
    initialize();
}

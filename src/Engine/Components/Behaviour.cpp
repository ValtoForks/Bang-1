#include "Bang/Behaviour.h"

#include "Bang/BehaviourManager.h"
#include "Bang/EventListener.tcc"
#include "Bang/FastDynamicCast.h"
#include "Bang/IEventsGameObjectPhysics.h"

USING_NAMESPACE_BANG

Behaviour::Behaviour()
{
    CONSTRUCT_CLASS_ID(Behaviour)
    BehaviourManager::GetActive()->RegisterBehaviour(this);
}

Behaviour::~Behaviour()
{
}

const BPReflectedStruct& Behaviour::GetReflectionInfo() const
{
    return m_reflectionInfo;
}

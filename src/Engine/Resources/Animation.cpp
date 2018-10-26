#include "Bang/Animation.h"

#include <sys/types.h>
#include <istream>
#include <map>
#include <utility>

#include "Bang/Array.tcc"
#include "Bang/Assert.h"
#include "Bang/Map.tcc"
#include "Bang/Math.h"
#include "Bang/Matrix4.h"
#include "Bang/Matrix4.tcc"
#include "Bang/MetaNode.h"
#include "Bang/MetaNode.tcc"
#include "Bang/Set.h"
#include "Bang/Set.tcc"
#include "Bang/Time.h"
#include "Bang/Vector.tcc"

namespace Bang
{
class Path;
}

using namespace Bang;

Animation::Animation()
{
    SetWrapMode(AnimationWrapMode::REPEAT);
}

Animation::~Animation()
{
}

void Animation::AddPositionKeyFrame(
    const String &boneName,
    const Animation::KeyFrame<Vector3> &keyFrame)
{
    if (!GetBoneNameToPositionKeyFrames().ContainsKey(boneName))
    {
        m_boneNameToPositionKeyFrames.Add(boneName, {{}});
    }
    m_boneNameToPositionKeyFrames.Get(boneName).PushBack(keyFrame);
    PropagateResourceChanged();
}

void Animation::AddRotationKeyFrame(
    const String &boneName,
    const Animation::KeyFrame<Quaternion> &keyFrame)
{
    if (!GetBoneNameToRotationKeyFrames().ContainsKey(boneName))
    {
        m_boneNameToRotationKeyFrames.Add(boneName, {{}});
    }
    m_boneNameToRotationKeyFrames.Get(boneName).PushBack(keyFrame);
    PropagateResourceChanged();
}

void Animation::AddScaleKeyFrame(const String &boneName,
                                 const Animation::KeyFrame<Vector3> &keyFrame)
{
    if (!GetBoneNameToScaleKeyFrames().ContainsKey(boneName))
    {
        m_boneNameToScaleKeyFrames.Add(boneName, {{}});
    }
    m_boneNameToScaleKeyFrames.Get(boneName).PushBack(keyFrame);
    PropagateResourceChanged();
}

void Animation::SetDurationInFrames(float durationInFrames)
{
    if (durationInFrames != GetDurationInFrames())
    {
        m_durationInFrames = durationInFrames;
        PropagateResourceChanged();
    }
}

void Animation::SetFramesPerSecond(float framesPerSecond)
{
    if (framesPerSecond != GetFramesPerSecond())
    {
        m_framesPerSecond = framesPerSecond;
        PropagateResourceChanged();
    }
}

void Animation::SetWrapMode(AnimationWrapMode wrapMode)
{
    if (wrapMode != GetWrapMode())
    {
        m_wrapMode = wrapMode;
        PropagateResourceChanged();
    }
}

void Animation::SetSpeed(float speed)
{
    if (speed != GetSpeed())
    {
        m_speed = speed;
        PropagateResourceChanged();
    }
}

float Animation::GetSpeed() const
{
    return m_speed;
}

float Animation::GetDurationInFrames() const
{
    return m_durationInFrames;
}

float Animation::GetDurationInSeconds() const
{
    return GetDurationInFrames() / Math::Max(GetFramesPerSecond(), 0.00001f);
}

float Animation::GetFramesPerSecond() const
{
    return m_framesPerSecond;
}

AnimationWrapMode Animation::GetWrapMode() const
{
    return m_wrapMode;
}

template <class T>
Array<Animation::KeyFrame<T>> GetConsecutiveKeyFrames(
    const Array<Animation::KeyFrame<T>> &keyFrames,
    float timeInFrames)
{
    const uint numKF = keyFrames.Size();
    for (uint i = 0; i < numKF - 1; ++i)
    {
        const Animation::KeyFrame<T> &prevKF = keyFrames[i];
        const Animation::KeyFrame<T> &nextKF = keyFrames[i + 1];
        if (timeInFrames >= prevKF.timeInFrames &&
            timeInFrames <= nextKF.timeInFrames)
        {
            return {{prevKF, nextKF}};
        }
    }
    return {{}};
}

float Animation::WrapTime(float time,
                          float totalDuration,
                          AnimationWrapMode animationWrapMode)
{
    float wrappedTime = -1.0f;
    switch (animationWrapMode)
    {
        case AnimationWrapMode::CLAMP:
            wrappedTime = Math::Clamp(time, 0.0f, totalDuration);
            break;

        case AnimationWrapMode::REPEAT:
            wrappedTime = Math::FModAbs(time, totalDuration);
            break;

        case AnimationWrapMode::PING_PONG:
        {
            wrappedTime = Math::FModAbs(time, totalDuration);
            int parity = SCAST<int>(wrappedTime / totalDuration);
            if ((parity % 2) == 1)
            {
                wrappedTime = totalDuration - wrappedTime;
            }
        }
        break;
    }
    return wrappedTime;
}

const Array<Animation::KeyFrame<Vector3>> &Animation::GetPositionKeyFrames(
    const String &boneName) const
{
    if (!GetBoneNameToPositionKeyFrames().ContainsKey(boneName))
    {
        return Array<Animation::KeyFrame<Vector3>>::Empty();
    }
    return GetBoneNameToPositionKeyFrames().Get(boneName);
}

const Array<Animation::KeyFrame<Quaternion>> &Animation::GetRotationKeyFrames(
    const String &boneName) const
{
    if (!GetBoneNameToRotationKeyFrames().ContainsKey(boneName))
    {
        return Array<Animation::KeyFrame<Quaternion>>::Empty();
    }
    return GetBoneNameToRotationKeyFrames().Get(boneName);
}

const Array<Animation::KeyFrame<Vector3>> &Animation::GetScaleKeyFrames(
    const String &boneName) const
{
    if (!GetBoneNameToScaleKeyFrames().ContainsKey(boneName))
    {
        return Array<Animation::KeyFrame<Vector3>>::Empty();
    }
    return GetBoneNameToScaleKeyFrames().Get(boneName);
}

const Map<String, Array<Animation::KeyFrame<Vector3>>>
    &Animation::GetBoneNameToPositionKeyFrames() const
{
    return m_boneNameToPositionKeyFrames;
}

const Map<String, Array<Animation::KeyFrame<Quaternion>>>
    &Animation::GetBoneNameToRotationKeyFrames() const
{
    return m_boneNameToRotationKeyFrames;
}

const Map<String, Array<Animation::KeyFrame<Vector3>>>
    &Animation::GetBoneNameToScaleKeyFrames() const
{
    return m_boneNameToScaleKeyFrames;
}

void Animation::Import(const Path &animationFilepath)
{
    BANG_UNUSED(animationFilepath);
}

void Animation::ImportMeta(const MetaNode &metaNode)
{
    Asset::ImportMeta(metaNode);

    if (metaNode.Contains("WrapMode"))
    {
        SetWrapMode(SCAST<AnimationWrapMode>(metaNode.Get<int>("WrapMode")));
    }

    if (metaNode.Contains("Speed"))
    {
        SetSpeed(metaNode.Get<float>("Speed"));
    }
}

void Animation::ExportMeta(MetaNode *metaNode) const
{
    Asset::ExportMeta(metaNode);

    metaNode->Set("Speed", SCAST<float>(GetSpeed()));
    metaNode->Set("WrapMode", SCAST<int>(GetWrapMode()));
}

Map<String, Matrix4> Animation::GetBoneMatrices(
    const Map<String, Animation::BoneTransformation> &bonesTransformations)
{
    Map<String, Matrix4> bonesMatrices;
    for (auto &it : bonesTransformations)
    {
        const String &boneName = it.first;
        const BoneTransformation &boneTransformation = it.second;
        const BoneTransformation &bt = boneTransformation;
        Matrix4 transformMatrix = Matrix4::TranslateMatrix(bt.position) *
                                  Matrix4::RotateMatrix(bt.rotation) *
                                  Matrix4::ScaleMatrix(bt.scale);
        bonesMatrices.Add(boneName, transformMatrix);
    }
    return bonesMatrices;
}

Map<String, Animation::BoneTransformation>
Animation::GetBoneAnimationTransformations(const Animation *anim,
                                           Time animationTime)
{
    Map<String, Animation::BoneTransformation> boneTransformations;

    if (anim->GetDurationInFrames() <= 0.0f)
    {
        return boneTransformations;
    }

    double timeInFrames =
        (animationTime.GetSeconds() * anim->GetFramesPerSecond());
    timeInFrames = WrapTime(
        timeInFrames, anim->GetDurationInFrames(), anim->GetWrapMode());
    timeInFrames = Math::Max(timeInFrames, 0.00001);

    for (const auto &it : anim->GetBoneNameToPositionKeyFrames())
    {
        const String &boneName = it.first;
        const auto &posKeyFrames = it.second;

        Vector3 bonePosition = Vector3::Zero;

        Array<KeyFrame<Vector3>> positionInterpKeyFrames =
            GetConsecutiveKeyFrames(posKeyFrames, timeInFrames);

        if (positionInterpKeyFrames.Size() == 2)
        {
            const KeyFrame<Vector3> &prevPosKF = positionInterpKeyFrames[0];
            const KeyFrame<Vector3> &nextPosKF = positionInterpKeyFrames[1];
            float timeBetweenPrevNext =
                (nextPosKF.timeInFrames - prevPosKF.timeInFrames);
            timeBetweenPrevNext = Math::Max(timeBetweenPrevNext, 0.0001f);

            float timePassedSincePrev = (timeInFrames - prevPosKF.timeInFrames);
            float interpFactor = (timePassedSincePrev / timeBetweenPrevNext);
            interpFactor = Math::Clamp(interpFactor, 0.0f, 1.0f);

            bonePosition =
                Vector3::Lerp(prevPosKF.value, nextPosKF.value, interpFactor);
        }
        boneTransformations[boneName].position = bonePosition;
    }

    for (const auto &it : anim->GetBoneNameToRotationKeyFrames())
    {
        const String &boneName = it.first;
        const auto &rotKeyFrames = it.second;

        Quaternion boneRotation = Quaternion::Identity;

        Array<KeyFrame<Quaternion>> rotationInterpKeyFrames =
            GetConsecutiveKeyFrames(rotKeyFrames, timeInFrames);

        if (rotationInterpKeyFrames.Size() == 2)
        {
            const KeyFrame<Quaternion> &prevRotKF = rotationInterpKeyFrames[0];
            const KeyFrame<Quaternion> &nextRotKF = rotationInterpKeyFrames[1];
            float timeBetweenPrevNext =
                (nextRotKF.timeInFrames - prevRotKF.timeInFrames);
            timeBetweenPrevNext = Math::Max(timeBetweenPrevNext, 0.0001f);

            float timePassedSincePrev = (timeInFrames - prevRotKF.timeInFrames);
            float interpFactor = (timePassedSincePrev / timeBetweenPrevNext);
            interpFactor = Math::Clamp(interpFactor, 0.0f, 1.0f);

            boneRotation = Quaternion::SLerp(
                prevRotKF.value, nextRotKF.value, interpFactor);
        }
        boneTransformations[boneName].rotation = boneRotation;
    }

    for (const auto &it : anim->GetBoneNameToScaleKeyFrames())
    {
        const String &boneName = it.first;
        const auto &scaleKeyFrames = it.second;

        Vector3 boneScale = Vector3::One;
        Array<KeyFrame<Vector3>> scaleInterpKeyFrames =
            GetConsecutiveKeyFrames(scaleKeyFrames, timeInFrames);

        if (scaleInterpKeyFrames.Size() == 2)
        {
            const KeyFrame<Vector3> &prevScaleKF = scaleInterpKeyFrames[0];
            const KeyFrame<Vector3> &nextScaleKF = scaleInterpKeyFrames[1];
            float timeBetweenPrevNext =
                (nextScaleKF.timeInFrames - prevScaleKF.timeInFrames);
            timeBetweenPrevNext = Math::Max(timeBetweenPrevNext, 0.0001f);

            float timePassedSincePrev =
                (timeInFrames - prevScaleKF.timeInFrames);
            float interpFactor = (timePassedSincePrev / timeBetweenPrevNext);
            interpFactor = Math::Clamp(interpFactor, 0.0f, 1.0f);

            boneScale = Vector3::Lerp(
                prevScaleKF.value, nextScaleKF.value, interpFactor);
        }
        boneTransformations[boneName].scale = boneScale;
    }

    return boneTransformations;
}

Map<String, Animation::BoneTransformation>
Animation::GetBoneCrossFadeAnimationTransformations(
    const Animation *prevAnimation,
    Time prevAnimationTime,
    const Animation *nextAnimation,
    Time currentCrossFadeTime,
    Time totalCrossFadeTime)
{
    Map<String, Animation::BoneTransformation> boneCrossFadeAnimTransformations;

    double totalCrossFadeSeconds =
        Math::Max(totalCrossFadeTime.GetSeconds(), 0.01);
    float nextPonderation =
        (currentCrossFadeTime.GetSeconds() / totalCrossFadeSeconds);

    // Gather the prev animation bone transformations
    Map<String, BoneTransformation> prevBoneTransformations =
        Animation::GetBoneAnimationTransformations(prevAnimation,
                                                   prevAnimationTime);

    // Gather the next animation bone transformations
    Map<String, BoneTransformation> nextBoneTransformations =
        Animation::GetBoneAnimationTransformations(nextAnimation,
                                                   currentCrossFadeTime);

    // Gather bone names
    Set<String> allBoneNames;
    for (const auto &pair : prevBoneTransformations)
    {
        allBoneNames.Add(pair.first);
    }
    for (const auto &pair : nextBoneTransformations)
    {
        allBoneNames.Add(pair.first);
    }

    // Interpolate transformations
    for (const String &boneName : allBoneNames)
    {
        BoneTransformation prevBoneTransformation;
        auto prevTransformationIt = prevBoneTransformations.Find(boneName);
        if (prevTransformationIt != prevBoneTransformations.End())
        {
            prevBoneTransformation = prevTransformationIt->second;
        }

        BoneTransformation nextBoneTransformation;
        auto nextTransformationIt = nextBoneTransformations.Find(boneName);
        if (nextTransformationIt != nextBoneTransformations.End())
        {
            nextBoneTransformation = nextTransformationIt->second;
        }

        Vector3 interpPos = Vector3::Lerp(prevBoneTransformation.position,
                                          nextBoneTransformation.position,
                                          nextPonderation);
        Quaternion interpRot =
            Quaternion::SLerp(prevBoneTransformation.rotation,
                              nextBoneTransformation.rotation,
                              nextPonderation);
        Vector3 interpScale = Vector3::Lerp(prevBoneTransformation.scale,
                                            nextBoneTransformation.scale,
                                            nextPonderation);

        BoneTransformation crossFadedTransformation;
        crossFadedTransformation.position = interpPos;
        crossFadedTransformation.rotation = interpRot;
        crossFadedTransformation.scale = interpScale;

        boneCrossFadeAnimTransformations.Add(boneName,
                                             crossFadedTransformation);
    }

    return boneCrossFadeAnimTransformations;
}

#ifndef ROPE_H
#define ROPE_H

#include "Bang/AABox.h"
#include "Bang/Array.h"
#include "Bang/Bang.h"
#include "Bang/BangDefines.h"
#include "Bang/Component.h"
#include "Bang/ComponentMacros.h"
#include "Bang/LineRenderer.h"
#include "Bang/MetaNode.h"
#include "Bang/Particle.h"
#include "Bang/ResourceHandle.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

FORWARD class ICloneable;
FORWARD class Mesh;
FORWARD class ShaderProgram;

class Rope : public LineRenderer
{
    COMPONENT(Rope)

public:
	Rope();
	virtual ~Rope();

    // Component
    void OnStart() override;
    void OnUpdate() override;

    // Renderer
    void Bind() override;
    void OnRender() override;
    virtual AABox GetAABBox() const override;
    void SetUniformsOnBind(ShaderProgram *sp) override;

    void Reset();

    void SetDamping(float damping);
    void SetNumPoints(uint numPoints);
    void SetBounciness(float bounciness);
    void SetRopeLength(float ropeLength);
    void SetSpringsForce(float springsForce);

    void SetFixedPoint(uint i, bool fixed);
    void SetFixedPoints(const Array<bool> pointsFixed);
    void SetPoints(const Array<Vector3> &points);
    void SetSeeDebugPoints(bool seeDebugPoints);

    bool IsPointFixed(uint i) const;
    float GetSpringsForce() const;
    float GetRopeLength() const;
    float GetBounciness() const;
    float GetDamping() const;
    uint GetNumPoints() const;
    bool GetSeeDebugPoints() const;

    // ICloneable
    virtual void CloneInto(ICloneable *clone) const override;

    // Serializable
    virtual void ImportMeta(const MetaNode &metaNode) override;
    virtual void ExportMeta(MetaNode *metaNode) const override;

private:
    Array<Vector3> m_points;
    Array<bool> m_fixedPoints;
    Particle::Parameters m_particleParams;

    float m_ropeLength = 1.0f;
    float m_springsForce = 1.0f;
    Array<Particle::Data> m_particlesData;

    bool m_seeDebugPoints = false;
    RH<Mesh> m_ropeDebugPointsMesh;

    bool m_validLineRendererPoints = false;

    void InitParticle(uint i, const Particle::Parameters &params);
    const Particle::Parameters& GetParameters() const;
    float GetPartLength() const;
    void AddSpringForces();
    void ConstrainPointsPositions();

    void UpdateLineRendererPoints();
};

NAMESPACE_BANG_END

#endif // ROPE_H


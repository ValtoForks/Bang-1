#ifndef BPPROPERTY_H
#define BPPROPERTY_H

#include "Bang/Array.h"
#include "Bang/BangDefines.h"
#include "Bang/IToString.h"
#include "Bang/String.h"

NAMESPACE_BANG_BEGIN

class BPReflectedVariable : public IToString
{
public:
    BPReflectedVariable();

    static void FromString(String::Iterator propBegin,
                           String::Iterator propEnd,
                           BPReflectedVariable *outProperty,
                           bool *success);

    String GetInitializationCode(const String &propInitVarName) const;

    bool IsOfType(const Array<String> &varTypeArray) const;

    void SetName(const String &name);
    void SetVariableType(const String &varType);
    void SetVariableCodeName(const String &varCodeName);
    void SetVariableInitValue(const String &initValue);

    const String& GetName() const;
    const String& GetVariableType() const;
    const String& GetVariableCodeName() const;
    const String& GetVariableInitValue() const;

private:
    String m_name = "";
    String m_variableType = "";
    String m_variableCodeName = "";
    String m_variableInitValue = "";

    String ToString() const override;
};

NAMESPACE_BANG_END

#endif // BPPROPERTY_H

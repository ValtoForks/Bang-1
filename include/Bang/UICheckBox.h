#ifndef UICHECKBOX_H
#define UICHECKBOX_H

#include "Bang/Component.h"
#include "Bang/IEventEmitter.h"
#include "Bang/IFocusListener.h"
#include "Bang/IValueChangedListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIFocusable;
FORWARD class UIImageRenderer;
FORWARD class UILayoutElement;

class UICheckBox : public Component,
                   public EventEmitter<IValueChangedListener>,
                   public EventEmitter<IFocusListener>,
                   public EventListener<IFocusListener>
{
    COMPONENT(UICheckBox)

public:
    void OnUpdate() override;

    void SetSize(int size);
    void SetChecked(bool checked);

    bool IsChecked() const;
    int GetSize() const;

    UIImageRenderer *GetTickImage() const;
    UIImageRenderer *GetBackgroundImage() const;
    UILayoutElement *GetLayoutElement() const;
    IFocusable *GetFocusable() const;

private:
    static Color IdleColor;
    static Color OverColor;

    int m_size = -1;
    bool m_isChecked = true;
    UIFocusable *p_focusable = nullptr;
    UIImageRenderer *p_tickImage = nullptr;
    UIImageRenderer *p_checkBgImage = nullptr;
    UILayoutElement *p_layoutElement = nullptr;

    UICheckBox();
    virtual ~UICheckBox();

    static UICheckBox *CreateInto(GameObject *go);

    friend class GameObjectFactory;
};

NAMESPACE_BANG_END

#endif // UICHECKBOX_H


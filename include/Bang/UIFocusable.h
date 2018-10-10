﻿#ifndef UIFOCUSABLE_H
#define UIFOCUSABLE_H

#include "Bang/Key.h"
#include "Bang/Array.h"
#include "Bang/Cursor.h"
#include "Bang/Vector2.h"
#include "Bang/Component.h"
#include "Bang/EventEmitter.h"
#include "Bang/IEventsFocus.h"

NAMESPACE_BANG_BEGIN

class UIFocusable : public Component,
                    public EventEmitter<IEventsFocus>
{
    COMPONENT_WITH_FAST_DYNAMIC_CAST(UIFocusable)

public:
    using EventCallback = std::function<UIEventResult(UIFocusable*,
                                                      const UIEvent&)>;

    UIEventResult ProcessEvent(const UIEvent &event);

    void SetFocusEnabled(bool focusEnabled);
    void SetCursorType(Cursor::Type cursorType);
    void SetConsiderForTabbing(bool considerForTabbing);
    void AddEventCallback(UIFocusable::EventCallback eventCallback);
    void ClearEventCallbacks();

    bool HasFocus() const;
    bool IsFocusEnabled() const;
    bool HasJustFocusChanged() const;
    bool IsBeingPressed() const;
    bool IsMouseOver() const;
    Cursor::Type GetCursorType() const;
    bool GetConsiderForTabbing() const;

private:
    bool m_hasFocus = false;
    bool m_isMouseOver = false;
    bool m_focusEnabled = true;
    bool m_beingPressed = false;
    bool m_considerForTabbing = false;
    bool m_hasJustFocusChanged = false;
    bool m_lastMouseDownWasHere = false;
    Cursor::Type m_cursorType = Cursor::Type::ARROW;

    Array<EventCallback> m_eventCallbacks;

    UIFocusable();
    virtual ~UIFocusable();

    // Component
    bool CanBeRepeatedInGameObject() const override;

    void SetBeingPressed(bool beingPressed);
    void SetIsMouseOver(bool isMouseOver);
    void SetFocus();
    void ClearFocus();

    friend class UICanvas;
};

NAMESPACE_BANG_END

#endif // UIFOCUSABLE_H


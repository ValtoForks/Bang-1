#ifndef UITREE_H
#define UITREE_H

#include "Bang/Map.h"
#include "Bang/List.h"
#include "Bang/Tree.h"
#include "Bang/UIList.h"
#include "Bang/UIButton.h"
#include "Bang/Component.h"
#include "Bang/GameObject.h"
#include "Bang/IFocusListener.h"
#include "Bang/IUITreeListener.h"
#include "Bang/IDragDropListener.h"

NAMESPACE_BANG_BEGIN

FORWARD class UIDragDroppable;
FORWARD class UITreeItemContainer;

class UITree : public Component,
               public IFocusListener,
               public IDestroyListener,
               public IDragDropListener,
               public EventEmitter<IUITreeListener>
{
    COMPONENT(UITree)

public:
    enum class MouseItemRelativePosition { ABOVE, OVER, BELOW, BELOW_ALL };

    void AddItem(GOItem *newItem, GOItem *parentItem, int indexInsideParent = 0);
    void MoveItem(GOItem *item, GOItem *newParentItem, int newIndexInsideParent = 0);
    void RemoveItem(GOItem *itemToRemove);
    GOItem* GetSelectedItem() const;
    void Clear();

    void SetSelection(GOItem *item);
    void SetItemCollapsed(GOItem *item, bool collapsed);
    void SetSelectionCallback(UIList::SelectionCallback callback);

    GOItem* GetParentItem(GOItem *item) const;
    bool IsItemCollapsed(GOItem *item) const;
    List<GOItem*> GetTopChildrenItems() const;
    bool Contains(UITreeItemContainer *item) const;
    List<GOItem*> GetChildrenItems(GOItem *item) const;
    bool ItemIsChildOfRecursive(GOItem *item, GOItem *parent) const;
    UIList* GetUIList() const;

    void GetMousePositionInTree(
                    GOItem **itemOverOut,
                    UITree::MouseItemRelativePosition *itemRelPosOut) const;
    int GetFlatUIListIndex(GOItem *parentItem, int indexInsideParent);

    // Component
    void OnUpdate() override;

    // IFocusListener
    void OnClicked(IFocusable *focusable, ClickType clickType) override;

    // IDragDropListener
    virtual void OnDragStarted(UIDragDroppable *dragDroppable);
    virtual void OnDragUpdate(UIDragDroppable *dragDroppable);
    virtual void OnDrop(UIDragDroppable *dragDroppable);

    // IDestroyListener
    void OnDestroyed(EventEmitter<IDestroyListener> *object) override;

protected:
    UITree();
    virtual ~UITree();

    static UITree* CreateInto(GameObject *go);

private:
    static const int IndentationPx;

    Tree<GOItem*> m_rootTree;
    UIList *p_uiList = nullptr;
    GOItem *p_itemBeingDragged = nullptr;
    Map<GOItem*, Tree<GOItem*>*> m_itemToTree;
    UIList::SelectionCallback m_selectionCallback;

    GameObject *p_dragMarker = nullptr;
    UIImageRenderer *p_dragMarkerImg = nullptr;

    void AddItem_(GOItem* newItemTree, GOItem *parentItem,
                  int indexInsideParent, bool moving);
    void AddItem_(const Tree<GOItem*> &newItemTree, GOItem *parentItem,
                  int indexInsideParent, bool moving);
    void RemoveItem_(GOItem *itemToRemove, bool moving);
    void UnCollapseUpwards(GOItem *item);
    void SetItemCollapsedRecursive(GOItem *item, bool collapse);
    void UpdateCollapsability(GOItem *item);
    void IndentItem(GOItem *item);
    bool IsValidDrag(GOItem *itemBeingDragged, GOItem *itemOver) const;

    Tree<GOItem*>* GetItemTree(GOItem* item) const;
    UITreeItemContainer* GetItemContainer(GOItem *item) const;

    friend class GameObjectFactory;
};


// UITreeItemContainer
class UITreeItemContainer : public GameObject
{
public:
    UITreeItemContainer();
    virtual ~UITreeItemContainer();

    void SetCollapsable(bool collapsable);
    void SetCollapsed(bool collapsed);
    void SetContainedItem(GOItem *item);
    void SetIndentation(int indentationPx);

    bool IsCollapsed() const;
    int GetIndentationPx() const;
    GOItem *GetContainedItem() const;
    UIButton *GetCollapseButton() const;
    UIDragDroppable *GetDragDroppable() const;

private:
    bool m_collapsed = false;

    int m_indentationPx = 0;
    UIButton *p_collapseButton = nullptr;
    GOItem *p_containedGameObject = nullptr;
    GameObject *p_indentSpacer = nullptr;
    UIDragDroppable *p_dragDroppable = nullptr;
};

NAMESPACE_BANG_END

#endif // UITREE_H


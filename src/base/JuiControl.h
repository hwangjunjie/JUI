#ifndef JUI_CONTROL_H_
#define JUI_CONTROL_H_

#include "JuiTypes.h"
#include "JuiEventManager.h"
#include "Object/JObject.h"
#include "math/JRect.h"

class JuiContainer;
class JDrawable;

class JuiControl : public JObject
{
	JDECLARE_ABSTRACT_CLASS(JuiControl)

	enum ControlFlags
	{
		FLAG_VISIBLE			= 0x0001,
		FLAG_ENABLE				= 0x0002,
		FLAG_FOCUSED			= 0x0004,
		FLAG_SELECTED			= 0x0008,
		FLAG_READONLY			= 0x0040,
		FLAG_MOUSE_ENABLE		= 0x0080,
		FLAG_KEYBOARD_ENABLE	= 0x0100,
		FLAG_CONTAINER			= 0x0200,
	};

public:
	JuiControl();
	virtual ~JuiControl();

	virtual JuiControl* Clone();

	JuiControl* GetRoot();
	virtual JuiEventManager *GetInputGenerator() { return NULL; }

	void SetUpdateRegion(const JPoint2I& pos, const JPoint2I& size);
	void SetUpdate();
	virtual void AddUpdateRegion(const JPoint2I& pos, const JPoint2I& size) {}

	void MouseLock();
	void MouseUnlock();
	bool IsMouseLocked();


	void SetName(const char* name);
	const char* GetName();

	void SetParent(JuiContainer* control);
	JuiContainer* GetParent();

	void SetBackground(const std::string& drawable);

public:
	// @ point and size
	// @ {
	//set point and size
	virtual void SetBounds(const JPoint2I& position, const JPoint2I& extent);
	void SetBounds(const JRectI& bounds);
	void SetBounds(int x, int y, int width, int height);

	void SetPosition(const JPoint2I& point);
	void SetPosition(int x, int y);

	void SetExtent(const JPoint2I& size);
	void SetExtent(int width, int height);
	void SetWidth(int width);
	void SetHeight(int height);

	void SetMinSize(int width, int height);
	void SetMinSize(const JPoint2I& size);
	void SetMaxSize(int width, int height);
	void SetMaxSize(const JPoint2I& size);

	//get point and size
	const JRectI& GetBounds() const;
	const JPoint2I& GetPosition() const;
	const JPoint2I& GetExtent() const;

	int GetPosX() const;
	int GetPosY() const;
	int GetWidth() const;
	int GetHeight() const;
	int GetRight() const;
	int GetBottom() const;

	bool IsPointIn(const JPoint2I& pt);
	JPoint2I LocalToRoot(const JPoint2I &pos);
	// @ }


public:
	// @ flags
	// @ {
	bool IsVisible() const;
	bool IsEnable() const;
	bool IsFocused() const;
	bool IsMouseEnable() const;
	bool IsKeyboardEnable() const;
	bool IsContainer() const;

	void SetVisible(bool value);
	void SetEnable(bool value);
	void SetMouseEnable(bool value);

protected:
	bool HasFlag(int flag) const;
	virtual void AddFlag(int flag);
	virtual void RemoveFlag(int flag);
	// @ }
	

public:
	// @ event
	// @ {
	void OnParentResized(const JRectI &oldRect, const JRectI &newRect);
	void OnChildResized(JuiControl *child);

	void NotifySiblings(int message, int param);
	virtual void OnControlEvent(JuiControl* sender, int message, int param) {}

	virtual void OnMouseEnter() {}
	virtual void OnMouseLeave() {}
	virtual void OnMouseDown(const MouseEventInfo& event) {}
	virtual void OnMouseUp(const MouseEventInfo& event) {}
	virtual void OnMouseMove(const MouseEventInfo& event) {}

	virtual void OnKeyDown() {}

	virtual void OnRender(JPoint2I offset, const JRectI& rcPaint);
	// @ }

protected:
	std::string m_Name;
	uint32_t m_nFlags;
	JRectI m_rcBounds;
	JPoint2I m_ptMinSize;
	JPoint2I m_ptMaxSize;

	JuiContainer *m_pParent;
	JDrawable* m_pBackground;
};

#include "JuiControl.inl"

#endif 
#ifndef MENUITEM_H
#define MENUITEM_H

class CMenuItem
{
protected:
   MENUITEMINFO   m_ItemInfo;
	HMENU          m_hMenu;
	unsigned int           m_IDItem;

public:
   CMenuItem();
   CMenuItem(CWnd *Wnd, unsigned int IDItem);
   CMenuItem(CMenu *Menu, unsigned int IDItem);
   CMenuItem(HMENU Menu, unsigned int IDItem);
   ~CMenuItem();

	void SetItem(HMENU hMenu, unsigned int IDItem);
   void SetShortcut(CString Key);
   void SetCaption(CString Caption);
   void SetEnabled(BOOL Enabled);

   HMENU GetHandle() { return m_hMenu;};
   unsigned int  GetID() { return m_IDItem;};

   bool Valid();
};

WORD TextToShortCut(CString SC);


#endif
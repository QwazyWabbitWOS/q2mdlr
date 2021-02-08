//---------------------------------------------------------------------------
#ifndef KeyboardH
#define KeyboardH
//---------------------------------------------------------------------------

class CAction;
class CActionListType;
class CKeystroke;
class CCommandList;

typedef int ActionFunc(void);
typedef int CALLBACK EnumActionFunc(CAction *A, DWORD lParam);



class CAction
{
public:
   CString	      m_Name,m_Cat;
   ActionFunc    *m_Exec;

   CMenuItem      m_Menu;

   CAction        *m_Next, *m_Prev;

   CAction();
   CAction(CString N, ActionFunc *E, CMenuItem M);
   CAction(CString N, CString C, ActionFunc *E, CMenuItem M);
   void Set(CString N, ActionFunc *E, CMenuItem M);
   void Set(CString N, CString C, ActionFunc *E, CMenuItem M);
};

class CActionListType
{
public:
   CAction     *m_First;
   CString     m_Name;

   CActionListType();
   ~CActionListType();

   int Add(CString N, ActionFunc *E, CMenuItem M);
   int Add(CString N, CString C, ActionFunc *E, CMenuItem M = CMenuItem());
   int Add(CString N, CString C, ActionFunc *E, CWnd *Wnd, unsigned int IDItem);
   int Sort();

   int EnumList(EnumActionFunc *Func, int lParam);

   CAction* GetAction(int i);
   CAction* operator[](int i);
};

class CKeystroke
{
public:
   WORD        m_Key;
   bool        m_Ctrl, m_Alt, m_Shift;
   CAction     *m_Act;
   CMenuItem   m_Erase;

   CKeystroke   *m_Prev, *m_Next;

   CKeystroke();
   CKeystroke( CAction *Ac, WORD K, bool C, bool A, bool S);
   ~CKeystroke();

   int ChangeMenuSC();
};

CString VKeyToString(WORD Ch, WORD Old=0);

class CCommandList
{
public:
   CKeystroke         *m_First;
   CActionListType    *m_Actions;

   CCommandList();
   ~CCommandList();

   int Add(CString N, CString Cat, WORD K, bool C, bool A, bool S);
   int AddBefore(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S);
   int AddAfter(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S);
   int Remove();
   int Remove(int n);
   int Remove(CKeystroke *K);
   int Set(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S);
   int Change(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S);
   void UpdateAllMenus();

   int SortAlpha();

   CKeystroke* FindKey(WORD K, CString Cat, bool C, bool A, bool S);
   CKeystroke* FindKey(CString N, CString C);
   int ExecuteKey(WORD K, bool C, bool A, bool S, CString Cat);

   CAction* FindAction(CString N, CString C);
   CAction* FindAction(int n);

   int nItems();

   int SaveToFile(CString FileName);
   int LoadFromFile(CString FileName);

   CKeystroke* operator[](int n);
   CCommandList& operator=(CCommandList &RHS);

};



#endif

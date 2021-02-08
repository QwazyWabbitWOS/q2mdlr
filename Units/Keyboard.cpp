//---------------------------------------------------------------------------
#include "stdafx.h"

#include "MenuItem.h"
#include "Keyboard.h"

#include "ctype.h"
#include "io.h"
#include "fcntl.h"
#include "sys/stat.h"


//---------------------------------------------------------------------------


CKeystroke::CKeystroke()
{
	m_Ctrl = m_Alt = m_Shift = false;
	m_Key = 0;
	m_Act = NULL;
	m_Next = m_Prev = NULL;
	//   Erase = NULL;
}

CKeystroke::CKeystroke(CAction *Ac, WORD K, bool C, bool A, bool S)
{
	m_Key = K;
	m_Ctrl = C;
	m_Alt = A;
	m_Shift = S;
	m_Next = m_Prev = NULL;
	m_Act = Ac;
}

CKeystroke::~CKeystroke()
{
	//   if (Act)
	//     if (Act && Act->m_Menu)
	//       Act->m_Menu.ShortCut = 0;
}

int CKeystroke::ChangeMenuSC()
{
	if (m_Erase.Valid())
	{
		m_Erase.SetShortcut("");
		m_Erase.SetItem(0, 0);
	}

	if (!m_Act)
		return 0;

	if (!m_Act->m_Menu.Valid())
		return 0;

	CString  SC = "";
	WORD K;

	if (m_Key == 190 || m_Act->m_Name == "Zoom In")
		K = MapVirtualKey(m_Key, 2);
	else
		K = MapVirtualKey(m_Key, 2);

	if (m_Alt)
		SC += "Alt+";
	if (m_Shift)
		SC += "Shift+";
	if (m_Ctrl)
		SC += "Ctrl+";

	//   if (K)
	//      SC += (char)K;
	//   else
	SC += VKeyToString(m_Key);

	//   SC+=toupper((char)MapVirtualKey(Key,2));

	//   Menu.ShortCut = SC;
	m_Act->m_Menu.SetShortcut(SC);

	return 1;
}


//---------------------------------------------------------------------------

CCommandList::CCommandList()
{
	m_First = NULL;
	m_Actions = NULL;
}

CCommandList::~CCommandList()
{
	CKeystroke   *K, *N;

	K = m_First;
	while (K)
	{
		N = K->m_Next;
		delete K;
		K = N;
	}
}

int CCommandList::Add(CString N, CString Cat, WORD K, bool C, bool A, bool S)
{
	CKeystroke   *Key, *Last;

	Key = new CKeystroke(FindAction(N, Cat), K, C, A, S);

	Last = m_First;
	if (Last)
	{
		while (Last->m_Next)
		{
			Last = Last->m_Next;
		}

		Key->m_Prev = Last;
		Last->m_Next = Key;
	}
	else
	{
		m_First = Key;
	}

	Key->ChangeMenuSC();

	return 1;
}

int CCommandList::AddBefore(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S)
{
	CKeystroke *Key, *nKey;

	nKey = (*this)[n];

	if (!nKey)
		return 0;

	Key = new CKeystroke(FindAction(N, Cat), K, C, A, S);

	Key->m_Prev = nKey->m_Prev;
	Key->m_Next = nKey;
	nKey->m_Prev = Key;
	Key->m_Prev->m_Next = Key;

	Key->ChangeMenuSC();

	return 1;
}

int CCommandList::AddAfter(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S)
{
	CKeystroke *Key, *nKey;

	nKey = (*this)[n];

	if (!nKey)
		return 0;

	Key = new CKeystroke(FindAction(N, Cat), K, C, A, S);

	Key->m_Prev = nKey;
	Key->m_Next = nKey->m_Next;
	nKey->m_Next = Key;
	Key->m_Next->m_Prev = Key;

	Key->ChangeMenuSC();

	return 1;
}

int CCommandList::Remove()
{
	int         n;

	n = nItems();
	return Remove(n - 1);
}

int CCommandList::Remove(int n)
{
	CKeystroke   *K;
	K = (*this)[n];

	if (!K)
		return 0;

	if (K->m_Prev)
		K->m_Prev->m_Next = K->m_Next;

	if (K->m_Next)
		K->m_Next->m_Prev = K->m_Prev;

	return 1;
}

int CCommandList::Remove(CKeystroke *K)
{
	if (!K)
		return 0;

	if (K->m_Next)
		K->m_Next->m_Prev = K->m_Prev;
	if (K->m_Prev)
		K->m_Prev->m_Next = K->m_Next;

	if (m_First == K)
		m_First = NULL;

	delete K;

	return 1;
}

int CCommandList::Set(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S)
{
	CKeystroke   *Key;

	Key = (*this)[n];

	if (!Key)
		return 0;

	Key->m_Act = FindAction(N, Cat);
	Key->m_Ctrl = C;
	Key->m_Alt = A;
	Key->m_Shift = S;

	Key->ChangeMenuSC();

	return 1;
}

int CCommandList::Change(int n, CString N, CString Cat, WORD K, bool C, bool A, bool S)
{
	CKeystroke   *Key;

	Key = (*this)[n];

	if (!Key)
		return 0;

	Key->m_Act = FindAction(N, Cat);
	Key->m_Ctrl = C;
	Key->m_Alt = A;
	Key->m_Shift = S;

	//   Key->ChangeMenuSC();

	return 1;
}

void CCommandList::UpdateAllMenus()
{
	CKeystroke   *K;

	K = m_First;
	while (K)
	{
		K->ChangeMenuSC();
		K = K->m_Next;
	}
}

int CCommandList::SortAlpha()
{

	return 0;
}

CKeystroke* CCommandList::FindKey(WORD K, CString Cat, bool C, bool A, bool S)
{
	CKeystroke *Key;

	if (!m_First)
		return NULL;

	Key = m_First;
	while (Key)
	{
		if (Key->m_Key == K && Key->m_Ctrl == C && Key->m_Alt == A && Key->m_Shift == S && Key->m_Act)
			if (Key->m_Act->m_Cat.CompareNoCase(Cat) == 0)
				return Key;
		Key = Key->m_Next;
	}
	return NULL;
}

CKeystroke* CCommandList::FindKey(CString N, CString C)
{
	CKeystroke *Key;

	if (!m_First)
		return NULL;

	Key = m_First;
	while (Key)
	{
		if (Key->m_Act)
			if (Key->m_Act->m_Name.CompareNoCase(N) == 0)
				if (Key->m_Act->m_Cat.CompareNoCase(C) == 0)
					return Key;

		Key = Key->m_Next;
	}
	return NULL;
}

int CCommandList::ExecuteKey(WORD K, bool C, bool A, bool S, CString Cat)
{
	CKeystroke *Key;

	Key = FindKey(K, Cat, C, A, S);
	if (!Key)
		return 0;

	return Key->m_Act->m_Exec();
}

int CCommandList::nItems()
{
	CKeystroke   *K;
	int         n = 0;

	if (!m_First)
		return 0;

	K = m_First;
	n++;

	while (K->m_Next)
	{
		K = K->m_Next;
		n++;
	}

	return n;
}

CAction* CCommandList::FindAction(CString N, CString C)
{
	CAction*  A;
	int      i = 0;

	if (!m_Actions)
		return NULL;

	while ((A = m_Actions->GetAction(i++)))
		if ((N.CompareNoCase(A->m_Name) == 0))
			if ((C.CompareNoCase(A->m_Cat) == 0))
			{
				return A;
			}

	return NULL;
}

CAction* CCommandList::FindAction(int n)
{
	return m_Actions->GetAction(n);
}

CKeystroke* CCommandList::operator[](int n)
{
	CKeystroke *F;

	if (!m_First)
		return NULL;

	if (n < 0)
		return m_First;

	F = m_First;

	for (int i = 0; i < n && (F->m_Next); i++)
	{
		F = F->m_Next;

	}

	return F;
}

CCommandList& CCommandList::operator=(CCommandList &RHS)
{
	{
		CKeystroke   *K, *N;

		K = m_First;
		while (K)
		{
			N = K->m_Next;
			delete K;
			K = N;
		}
	}

	m_First = NULL;
	CKeystroke   *K, *Cur, *N;

	K = RHS.m_First;

	if (K == NULL)
	{
		m_First = NULL;
	}
	else
	{
		m_First = new CKeystroke(K->m_Act, K->m_Key, K->m_Ctrl, K->m_Alt, K->m_Shift);
		m_First->m_Erase = K->m_Erase;
		m_First->m_Prev = NULL;
		m_First->m_Next = NULL;

		Cur = m_First;

		while (K)
		{
			if (K->m_Next)
			{
				N = new CKeystroke(K->m_Next->m_Act, K->m_Next->m_Key, K->m_Next->m_Ctrl, K->m_Next->m_Alt, K->m_Next->m_Shift);
				N->m_Erase = K->m_Next->m_Erase;
				N->m_Prev = Cur;
				N->m_Next = NULL;
				Cur->m_Next = N;

			}
			Cur = Cur->m_Next;
			K = K->m_Next;
		}
	}

	m_Actions = RHS.m_Actions;

	return *this;
}


void writeS(int Handle, CString S)
{
	write(Handle, (LPCTSTR)S, S.GetLength());
}

void writeS(int Handle, char *S)
{
	write(Handle, S, strlen(S));
}

void writeC(int Handle, char C)
{
	write(Handle, &C, 1);
}

int CCommandList::SaveToFile(CString FileName)
{
	CKeystroke   *K;
	int         hFile;

	K = m_First;

	hFile = open((LPCTSTR)FileName, O_BINARY | O_CREAT | O_TRUNC | O_WRONLY, S_IREAD | S_IWRITE);

	if (hFile == -1)
	{
		int i = errno;

		return 0;
	}

	while (K)
	{
		if (K->m_Act && K->m_Key)
		{
			writeC(hFile, '\"');
			writeS(hFile, (LPCTSTR)K->m_Act->m_Name);
			writeC(hFile, '\"');
			writeS(hFile, " = ");

			WORD W = K->m_Key;
			write(hFile, &W, 2);

			if (K->m_Shift)
				writeC(hFile, 'S');
			else
				writeC(hFile, 's');

			if (K->m_Ctrl)
				writeC(hFile, 'C');
			else
				writeC(hFile, 'c');

			if (K->m_Alt)
				writeC(hFile, 'A');
			else
				writeC(hFile, 'a');

			writeC(hFile, ' ');
			writeC(hFile, '\"');
			writeS(hFile, (LPCTSTR)K->m_Act->m_Cat);
			writeC(hFile, '\"');

			writeC(hFile, '\x0d');
			writeC(hFile, '\x0a');
		}

		K = K->m_Next;
	}

	close(hFile);

	return 1;
}

int CCommandList::LoadFromFile(CString FileName)
{
	char        *Buffer, *B, *EndBuf;
	int         hFile, End, Pos;

	hFile = open((LPCTSTR)FileName, O_BINARY | O_RDONLY);

	if (hFile == -1)
	{
		int i = errno;

		return 0;
	}

	Buffer = new char[filelength(hFile) + 1];

	End = read(hFile, Buffer, filelength(hFile));
	Buffer[End] = '\x0';

	close(hFile);

	// delete the current list
	{
		CKeystroke   *K, *N;

		K = m_First;
		while (K)
		{
			N = K->m_Next;
			delete K;
			K = N;
		}
	}

	CKeystroke   *Cur, *Prev;
	CString  N, Cat;

	Prev = NULL;
	Cur = NULL;

	Pos = 0;
	B = Buffer;
	EndBuf = Buffer;
	EndBuf += End;

	while (B < EndBuf)
	{
		WORD  Ch;
		bool S, C, A;

		N = "";

		B = strchr(B, '\"');  // find the m_First "
		if (B == NULL)
		{
			break;
		}
		B++;

		try {
			while (*B != '\"')
			{
				N += *B;
				B++;
			}
		}
		catch (...)
		{
		}

		B += 4;  // goto m_First shortcut char


		Ch = *((WORD*)B);
		B += 2;

		if (*B == 'S')
			S = true;
		else
			S = false;

		B++;
		if (*B == 'C')
			C = true;
		else
			C = false;

		B++;
		if (*B == 'A')
			A = true;
		else
			A = false;


		Cat = "";

		B = strchr(B, '\"');  // find the m_First "
		if (B == NULL)
		{
			break;
		}
		B++;

		try {
			while (*B != '\"')
			{
				Cat += *B;
				B++;
			}
		}
		catch (...)
		{
		}
		B++;

		//      Cur = new CKeystroke(FindAction(N), VkKeyScan(Ch), C, A, S);
		Cur = new CKeystroke(FindAction(N, Cat), Ch, C, A, S);

		if (Cur)
		{
			Cur->m_Prev = Prev;

			if (Prev)
				Prev->m_Next = Cur;
			else
				m_First = Cur;

			Prev = Cur;
			Cur = NULL;
		}
	}

	delete[] Buffer;

	return 1;
}



//----------------------------------------------------------------------

CAction::CAction()
{
	m_Name = "";
	m_Cat = "";
	m_Exec = NULL;
	m_Menu.SetItem(0, 0);
}

CAction::CAction(CString N, ActionFunc *E, CMenuItem M)
{
	m_Name = N;
	m_Exec = E;
	m_Menu = M;
	m_Cat = "";
}

CAction::CAction(CString N, CString C, ActionFunc *E, CMenuItem M)
{
	m_Name = N;
	m_Exec = E;
	m_Menu = M;
	m_Cat = C;
}

void CAction::Set(CString N, ActionFunc *E, CMenuItem M)
{
	m_Name = N;
	m_Exec = E;
	m_Menu = M;
	m_Cat = "";
}

void CAction::Set(CString N, CString C, ActionFunc *E, CMenuItem M)
{
	m_Name = N;
	m_Exec = E;
	m_Menu = M;
	m_Cat = C;
}

//-------------------------------------------------------------------------
CActionListType::CActionListType()
{
	m_First = NULL;
}

CActionListType::~CActionListType()
{
	CAction   *F, *N;

	F = m_First;

	if (F == NULL)
		return;

	while (F)
	{
		N = F->m_Next;
		delete F;
		F = N;
	}
}

int CActionListType::Add(CString N, ActionFunc *E, CMenuItem M)
{
	CAction *A;

	A = new CAction(N, E, M);
	A->m_Prev = NULL;
	A->m_Next = m_First;

	if (m_First == NULL)
	{
		m_First = A;
		return 1;
	}

	if (m_First)
		m_First->m_Prev = A;

	m_First = A;

	return 1;
}

int CActionListType::Add(CString N, CString C, ActionFunc *E, CMenuItem M)
{
	CAction *A;

	A = new CAction(N, C, E, M);
	A->m_Prev = NULL;
	A->m_Next = m_First;

	if (m_First == NULL)
	{
		m_First = A;
		return 1;
	}

	if (m_First)
		m_First->m_Prev = A;

	m_First = A;

	return 1;
}

int CActionListType::Add(CString N, CString C, ActionFunc *E, CWnd *Wnd, UINT IDItem)
{
	return Add(N, C, E, CMenuItem(Wnd, IDItem));
}

int CActionListType::Sort()
{
	//
	return 0;
}

CAction* CActionListType::GetAction(int i)
{
	CAction  *A;

	A = m_First;

	while (A && i > 0)
	{
		A = A->m_Next;
		i--;
	}

	return A;
}


CAction* CActionListType::operator[](int i)
{
	CAction*  A, N;

	A = m_First;

	while (A && i > 0)
	{
		A = A->m_Next;
		i--;
	}

	return A;
}

int CActionListType::EnumList(EnumActionFunc *Func, int lParam)
{
	CAction*  A;

	A = m_First;

	while (A)
	{
		if (Func(A, lParam) == 0)
			return 0;

		A = A->m_Next;
	}
	return 1;
}


CString VKeyToString(WORD Ch, WORD Old)
{
	CString A;

	if (Ch >= VK_F1 && Ch <= VK_F24)   // F1 -> F12
	{
		int diff = Ch - VK_F1;
		A += "F";
		if (Ch < VK_F10)
			A += (char)(diff + '1');
		else
			if (Ch < VK_F20)
			{
				diff -= 10;
				A += "1";
				A += (char)(diff + '1');
			}
			else
			{
				diff -= 20;
				A += "2";
				A += (char)(diff + '1');
			}
	}
	else
		A = (char)Ch;


	switch (Ch)
	{
	case VK_DELETE: A = "Delete"; break;
	case VK_BACK: A = "Backspace"; break;
	case VK_SPACE: A = "Space"; break;
	case VK_INSERT: A = "Insert"; break;
	case VK_END: A = "End"; break;
	case VK_HOME: A = "Home"; break;
	case VK_PRIOR: A = "Page Up"; break;
	case VK_NEXT: A = "Page Down"; break;
	case VK_ESCAPE: A = "Escape"; break;
	case VK_LEFT: A = "Left Arrow"; break;
	case VK_UP: A = "Up Arrow"; break;
	case VK_RIGHT: A = "Right Arrow"; break;
	case VK_DOWN: A = "Down Arrow"; break;
	case VK_CAPITAL: A = "CapsLock"; break;
	case VK_RETURN: A = "Return"; break;

	case VK_NUMPAD0: A = "Numpad 0"; break;
	case VK_NUMPAD1: A = "Numpad 1"; break;
	case VK_NUMPAD2: A = "Numpad 2"; break;
	case VK_NUMPAD3: A = "Numpad 3"; break;
	case VK_NUMPAD4: A = "Numpad 4"; break;
	case VK_NUMPAD5: A = "Numpad 5"; break;
	case VK_NUMPAD6: A = "Numpad 6"; break;
	case VK_NUMPAD7: A = "Numpad 7"; break;
	case VK_NUMPAD8: A = "Numpad 8"; break;
	case VK_NUMPAD9: A = "Numpad 9"; break;

	case VK_MULTIPLY: A = "Numpad *"; break;
	case VK_ADD: A = "Numpad +"; break;
	case VK_SUBTRACT: A = "Numpad -"; break;
	case VK_DECIMAL: A = "Numpad ."; break;
	case VK_DIVIDE: A = "Numpad /"; break;
	case VK_SEPARATOR: A = "Separator"; break;

	case VK_NUMLOCK: A = "Numlock"; break;
	case VK_SCROLL: A = "Scroll Lock"; break;
	case VK_TAB: A = "Tab"; break;

	case 0xBA: A = ';'; break;
	case 0xBB: A = '='; break;
	case 0xBC: A = ','; break;
	case 0xBD: A = '-'; break;
	case 0xBE: A = '.'; break;
	case 0xBF: A = '/'; break;
	case 0xC0: A = '`'; break;
	case 0xDB: A = '['; break;
	case 0xDC: A = '\\'; break;
	case 0xDD: A = ']'; break;
	case 0xDE: A = '\''; break;

	}

	return A;
}


#include "PSQuickStart.h"

#include "unsorted.h"
#include "GlobalHelpersThatHaveNoHomeYet.h"
#include "ClientNet/ClientNet.h"
#include "Game.h"

GlobalVar<bool, 0x00EED310> CPSQuickStart::m_ready;
extern char* charname;
extern char* ibuv_text;

int CPSQuickStart::OnPacket_MAYBE_IMPL(CMsgStreamBuffer* pMsg)
{
	printf("> " __FUNCTION__ " ~ Got Msg %04x\n", pMsg->msgid);
	if (pMsg->msgid == 0x1002)
	{
		int unk1, unk2;
		*pMsg >> unk1 >> unk2;

		CClientNet::get()->IBUV_confirm(ibuv_text); // Confirm IBVU
		printf("Send IBUV confirmation with text \"%s\"\n", ibuv_text);

		return 0;
	}

	if (pMsg->msgid == 0x1003) 
	{
		int unk1, unk2;
		*pMsg >> unk1 >> unk2;

		printf("IBUV_Response [%d][%d]\n", unk1, unk2);
		return 0;
	}

	if (pMsg->msgid == 0xB007)
	{
		pMsg->FlushRemaining();

		CMsgStreamBuffer buf(0x7001);

		buf << std::string(charname); // Character Name

		SendMsg(buf);

		return 0;
	}

	return reinterpret_cast<int(__thiscall*)(CPSQuickStart*,CMsgStreamBuffer*)>(0x00863AD0)(this, pMsg);
}

char CPSQuickStart::OnCreate_IMPL(int a1)
{
	printf("> " __FUNCTION__ "(%d)\n", a1);

	if (!StartNetEngine())
	{
		CGFXMainFrame::SetNextProcessSTAT(reinterpret_cast<CGfxRuntimeClass*>(0x0EED894));
		return false;
	}

	g_CGame->LoadTextfiles();
	m_ready = true;

	ShowWindow(g_CGame->GetHWnd(), SW_SHOWNORMAL);
	UpdateWindow(g_CGame->GetHWnd());

	g_CGame->ResizeMainWindow();

	return true;
}

#include "StdAfx.h"
#include "Util.h"
#include <memory/hook.h>
#include <AlramGuideMgrWnd.h>
#include <TextStringManager.h>
#include "GFXVideo3D_Hook.h"
#include "WndProc.h"
#include "hooks.h"
#include "GInterface.h"
#include "PSQuickStart.h"
#include "Game.h"
#include <sys/stat.h>
#include <IFChatViewer.h>
#include "QuickStart.h"
#include "CGame_Hook.h"

std::vector<const CGfxRuntimeClass*> register_objects;
std::vector<overrideFnPtr> override_objects;

QuickStart quickstart;

int APIENTRY _FakeWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow);


void Setup()
{
	// vftableHook(0x00E0963C, 25, addr_from_this(&CGFXVideo3d::BeginSceneIMPL));

	vftableHook(0x00E0963C, 17, addr_from_this(&CGFXVideo3D_Hook::CreateThingsHook));
	vftableHook(0x00E0963C, 26, addr_from_this(&CGFXVideo3D_Hook::EndSceneHook));
	vftableHook(0x00E0963C, 20, addr_from_this(&CGFXVideo3D_Hook::SetSizeHook));

	vftableHook(0x00db95a4, 10, addr_from_this(&CGInterface::OnCreateIMPL));

	replaceAddr(0x00831337+4, (int)&WndProcHook);

#ifdef CONFIG_DEBUG_REDIRECT_PUTDUMP
	replaceAddr(0x00832927+1, (int)&DebugPrintCallback);
#endif

#ifdef CONFIG_TRANSLATIONS_DEBUG
    placeHook(0x008C9C30, addr_from_this(&CTextStringManager::GetString));
#endif

    placeHook(0x0065c6f0, addr_from_this(&CAlramGuideMgrWnd::GetGuide));

    replaceOffset(0x008491d1, addr_from_this(&CGame_Hook::LoadGameOption));
#ifdef CONFIG_CHATVIEWER
    replaceOffset(0x008774f4, (int)&WriteToChatWindow);
    replaceOffset(0x00877b5c, (int)&WriteToChatWindow);

    placeHook(0x007a9bd0, addr_from_this(&CIFChatViewer::ShowHideControls));

#endif // CONFIG_CHATVIEWER

    replaceOffset(0x00B49AE4, (int)&_FakeWinMain);

    quickstart.Setup();
}

bool DoesFileExists(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void RegisterObject(const CGfxRuntimeClass* obj)
{
	register_objects.push_back(obj);
}

void OverrideObject(overrideFnPtr fn)
{
	override_objects.push_back(fn);
}

int APIENTRY _FakeWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
#ifdef CONFIG_DEBUG_CONSOLE
	AllocConsole();
	freopen("CONOUT$", "w", stdout);
	freopen("CONIN$", "r", stdin);
#endif
	printf("florian0's dev-client build on CMake\n");



	// Replace Create & Delete for existing classes
	// Note: We can't just inject existing objects like we would do with new objects.
	//       Joymax uses == on GFX_RUNTIME_CLASS(), so we would end up breaking this comparison
#if 0
	for (std::vector<const CGfxRuntimeClass*>::const_iterator it = override_objects.begin(); it != override_objects.end(); ++it)
	{

	}
#endif

	for (std::vector<const CGfxRuntimeClass*>::const_iterator it = register_objects.begin(); it != register_objects.end(); ++it)
	{
		reinterpret_cast<void(__thiscall*)(const CGfxRuntimeClass*, const char*, void*, void*, const CGfxRuntimeClass*, size_t, int)>(0x00B9C9C0)(*it, (*it)->m_lpszClassName, (*it)->m_pfnCreateObject, (*it)->m_pfnDeleteObject, (*it)->m_pBaseClass, (*it)->m_nObjectSize, 0);
	}


    for (std::vector<overrideFnPtr>::const_iterator it = override_objects.begin(); it != override_objects.end(); ++it)
    {
        (*it)();
    }

	if (quickstart.IsEnabled())
        quickstart.PreWinMain();

	return reinterpret_cast<int (APIENTRY*)(HINSTANCE, HINSTANCE, LPTSTR, int)>(0x008328C0)(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}

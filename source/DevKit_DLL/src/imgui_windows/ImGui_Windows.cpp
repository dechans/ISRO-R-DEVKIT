#include "ImGui_Windows.h"
#include <Windows.h>
#include "imgui/imgui.h"
#include "imgui/examples/imgui_impl_dx9.h"
#include "imgui/examples/imgui_impl_win32.h"

#include <GFX3DFunction/GFXVideo3d.h>
#include <GFX3DFunction/DrawingHelpers.h>

#include <GInterface.h>
#include "unsorted.h"

#include "../hooks/Hooks.h"
#include "../MathUtil.h"
#include "IFSystemMessage.h"
#include "InterfaceDebugger.h"
#include "NavMeshTool.h"
#include "EntityExplorer.h"
#include "SoundTool.h"
#include "NotificationTool.h"
#include "About.h"
#include <BSLib/multibyte.h>


extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

InterfaceDebugger interfaceDebugger;
NavMeshTool navmeshExplorer;
EntityExplorer entityExplorer;
SoundTool soundTool;
NotificationTool notificationTool;
About aboutWnd;

void ImGui_OnCreate(HWND hWindow, void *msghandler, int a3) {
    printf("ImGui_OnCreate\n");
    ImGui::CreateContext();

    ImGui_ImplWin32_Init(hWindow);
    ImGui_ImplDX9_Init(g_CD3DApplication->m_pd3dDevice);

}

void ImGui_OnEndScene() {
    // printf("ImGui_OnEndScene %d\n", g_CD3DApplication->IsLost());

    if (g_CD3DApplication->IsLost()) {
        return;
    }

    ImGui_ImplDX9_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    // Menu
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            ImGui::MenuItem("Debug Mode", NULL, reinterpret_cast<bool *>(0x00EED6A7));

            ImGui::Separator();

            if (ImGui::MenuItem("Restart"))
                SendRestartRequest(2);

            if (ImGui::MenuItem("Exit"))
                SendRestartRequest(1);

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit")) {
            ImGui::MenuItem("Cut", 0, false, false);
            ImGui::MenuItem("Copy", 0, false, false);
            ImGui::MenuItem("Paste", 0, false, false);
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Options")) {
            ImGui::MenuItem("Fullscreen", 0, false, false);

            if (ImGui::BeginMenu("Resolution")) {
                ImGui::MenuItem("640x480 (4:3)", 0, false, false);
                ImGui::MenuItem("800x600 (4:3)", 0, false, false);
                ImGui::Separator();
                ImGui::MenuItem("Enter custom resolution", 0, false, false);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Camera")) {
                ImGui::MenuItem("Fixed behind character", 0, false, false);
                ImGui::MenuItem("Fixed height", 0, false, false);
                ImGui::MenuItem("Free visual point", 0, false, false);
                ImGui::MenuItem("Free (Fly)", 0, false, false);

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools")) {
            interfaceDebugger.MenuItem();
            navmeshExplorer.MenuItem();
            entityExplorer.MenuItem();
            ImGui::MenuItem("Keyframe Editor", 0, false, false);
            ImGui::MenuItem("Script Editor", 0, false, false);
            soundTool.MenuItem();
            notificationTool.MenuItem();

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Windows")) {
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Help")) {
            aboutWnd.MenuItem();
            if (ImGui::MenuItem("Online Help")) {
                ShellExecute(NULL,
                             TEXT("open"),
                             TEXT("https://florian0.gitlab.io/sro_devkit/"),
                             NULL, NULL, SW_SHOWNORMAL);
            }
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }


    ImGui::Begin("Hello World");

    if (ImGui::Button("Hide")) {
        g_pCGInterface->m_IRM.GetResObj(1338, 1)->ShowGWnd(false);
    }

    if (ImGui::Button("Show")) {

    }

    if (ImGui::Button("Init Event")) {
        g_pCGInterface->CreateFlorian0Event();
    }

    if (ImGui::Button("System Log")) {
        wchar_t message[] = L"System Log Message";

        // Color (Picker: https://developer.mozilla.org/en-US/docs/Web/CSS/CSS_Colors/Color_picker_tool)
        int color = D3DCOLOR_ARGB(255, 0, 255, 0);

        // Get the SystemLog Window - Media//resinfo//ginterface.txt - GDR_SYSTEM_MESSAGE_VIEW:IFSystemMessage -> ID Should be 68
        IFSystemMessage *systemmessage = reinterpret_cast<IFSystemMessage *>(g_pCGInterface->m_IRM.GetResObj(68, 1));
        systemmessage->write(0xFF, color, message, 0, 1);
    }

    ImGui::End();

    interfaceDebugger.Render();
    entityExplorer.Render();
    soundTool.Render();
    notificationTool.Render();
    navmeshExplorer.Render();
    aboutWnd.Render();

    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

LRESULT CALLBACK ImGui_WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    //printf("ImGui_WndProc\n");

    // Check if context is already created
    if (!ImGui::GetCurrentContext())
        return RESULT_PASS;

    ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam);

    // Only pass events if imgui wants it
    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
        return RESULT_DISCARD;

    return RESULT_PASS;
}

void ImGui_OnPreSetSize(int width, int height) {
    printf("ImGui_OnPreSetSize\n");
    ImGui_ImplDX9_InvalidateDeviceObjects();
}

void ImGui_OnPostSetSize(int width, int height) {
    printf("ImGui_OnPostSetSize\n");
    //ImGui_ImplDX9_CreateDeviceObjects();
    ImGui_ImplDX9_Init(g_CD3DApplication->m_pd3dDevice);
}

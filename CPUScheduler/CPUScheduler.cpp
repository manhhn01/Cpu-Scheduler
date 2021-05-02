#ifndef UNICODE
#define UNICODE
#endif 
#define WIN32_LEAN_AND_MEAN
#define ID_ADD 1
#define ID_BUILD 2
#define ID_RESET 3
#define ID_OPT_FCFS 4
#define ID_OPT_SJF 5
#define ID_OPT_SRTF 6
#define ID_OPT_RR 7
#define ID_BIGGANTT 8
#define ID_HELP_ABOUT 9

#define GANTT_X 25
#define GANTT_Y 260
#define GANTT_WIDTH 525
#define GANTT_HEIGHT 50

#define BGANTT_X 5
#define BGANTT_Y 5
#define BGANTT_WIDTH 965
#define BGANTT_HEIGHT 70

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <CommCtrl.h>
#include <wchar.h>
#include "resource.h"
#include "CPUScheduler2.h"

using namespace std;

HWND g_hwndField1,
g_hwndField2,
g_hwndField3,
g_hwndField4,
g_hwndField5,
g_hwndField6,
g_hwndLabel1,
g_hwndLabel2,
g_hwndLabel3,
g_hwndAddButton,
g_hwndMain,
g_hwndList,
g_hwndGroupAdd,
g_hwndGroupAlg,
g_hwndBuildButton,
g_hwndGroupGantt,
g_hwndResetButton,
g_hwndOptFCFS,
g_hwndOptSJF,
g_hwndOptSRTF,
g_hwndOptRR,
g_hwndBigGanttButton;

HINSTANCE g_hInst;
Process processes[20];

int g_cProcess = 0, //countProcess
g_algorithm = -1; //0: fcfs 1:sjf 2:srtf 3:rr

vector<Process> g_gantt;

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);
HWND CreateListView(HWND);
void InsertListViewItems(HWND, Process);
void HandleListWM_NOTIFY(LPARAM);
BOOL ProcessSpecialKeys(__in MSG*);
void ProcessAddButton();
void AddMenu(HWND);
void RegDialogClass();
void getLastE();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
	g_hInst = hInstance;
	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&icex);		// Structure for control initialization.

	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Cpu Scheduler";

	WNDCLASS wc = { };

	wc.lpszClassName = CLASS_NAME;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClass(&wc);

	// Create the window.

	g_hwndMain = CreateWindowEx(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Cpu Scheduler",    // Window text
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,            // Window style        
		CW_USEDEFAULT, CW_USEDEFAULT, 610, 530, // Size and position    X,Y,nWidth,nHeight,
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (g_hwndMain == NULL)
	{
		return 0;
	}

	ShowWindow(g_hwndMain, nCmdShow);
	UpdateWindow(g_hwndMain);
	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!ProcessSpecialKeys(&msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HFONT hFont, hButtonFont, hGroupFont;
	//static HDC hdcStatic;
	static HBRUSH hbrBkgnd;
	RECT pos;
	switch (uMsg)
	{
	case WM_CREATE:
	{
		AddMenu(hwnd);
		g_hwndList = CreateListView(hwnd);
		g_hwndGroupAdd = CreateWindowW(
			WC_BUTTON,
			L"Thêm Tiến Trình: ",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			320, 20,
			240, 100,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);
		g_hwndField1 = CreateWindowW(
			WC_EDIT,
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			340, 55,
			40, 25,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndField2 = CreateWindowW(
			WC_EDIT,
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			390, 55,
			40, 25,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndField3 = CreateWindowW(
			WC_EDIT,
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP,
			440, 55,
			40, 25,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndAddButton = CreateWindowW(
			WC_BUTTONW,
			L"Add",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			490, 55,
			50, 25,
			hwnd,
			(HMENU)ID_ADD,
			g_hInst,
			NULL
		);

		g_hwndResetButton = CreateWindowW(
			WC_BUTTONW,
			L"Reset",
			WS_CHILD | WS_VISIBLE,
			410, 85,
			50, 25,
			hwnd,
			(HMENU)ID_RESET,
			g_hInst,
			NULL
		);

		g_hwndGroupAlg = CreateWindowW(
			WC_BUTTON,
			L"Thuật toán: ",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			320, 120,
			240, 110,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndOptFCFS = CreateWindowW(
			WC_BUTTON,
			L"FCFS",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			330, 140,
			60, 20,
			hwnd,
			(HMENU)ID_OPT_FCFS,
			g_hInst,
			NULL
		);

		g_hwndOptSJF = CreateWindowW(
			WC_BUTTON,
			L"SJF",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			330, 160,
			60, 20,
			hwnd,
			(HMENU)ID_OPT_SJF,
			g_hInst,
			NULL
		);

		g_hwndOptSRTF = CreateWindowW(
			WC_BUTTON,
			L"SRTF",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			330, 180,
			60, 20,
			hwnd,
			(HMENU)ID_OPT_SRTF,
			g_hInst,
			NULL
		);

		g_hwndOptRR = CreateWindowW(
			WC_BUTTON,
			L"RR",
			WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
			330, 200,
			60, 20,
			hwnd,
			(HMENU)ID_OPT_RR,
			g_hInst,
			NULL
		);

		g_hwndBuildButton = CreateWindowW(
			WC_BUTTON,
			L"Lập Lịch",
			WS_CHILD | WS_VISIBLE,
			470, 140,
			70, 25,
			hwnd,
			(HMENU)ID_BUILD,
			g_hInst,
			NULL
		);

		g_hwndField4 = CreateWindowW(
			WC_EDIT,
			L"",
			WS_CHILD | WS_BORDER | WS_VISIBLE,
			470, 195,
			70, 25,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndLabel1 = CreateWindowW(
			WC_STATIC,
			L"qq=",
			WS_CHILD | WS_VISIBLE,
			440, 195,
			30, 20,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndGroupGantt = CreateWindowW(
			WC_BUTTON,
			L"Sơ đồ Gantt: ",
			WS_CHILD | WS_VISIBLE | BS_GROUPBOX,
			15, 240,
			545, 100,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndLabel2 = CreateWindowW(
			WC_STATIC,
			L"Thời gian chờ TB: ",
			WS_CHILD | WS_VISIBLE,
			25, 380,
			135, 20,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndField5 = CreateWindowW(
			WC_EDIT,
			L"...",
			WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL,
			170, 380,
			280, 25,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndLabel3 = CreateWindowW(
			WC_STATIC,
			L"Thời gian xử lý TB: ",
			WS_CHILD | WS_VISIBLE,
			25, 410,
			145, 20,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndField6 = CreateWindowW(
			WC_EDIT,
			L"...",
			WS_CHILD | WS_VISIBLE | ES_READONLY | ES_AUTOHSCROLL,
			170, 410,
			280, 25,
			hwnd,
			NULL,
			g_hInst,
			NULL
		);

		g_hwndBigGanttButton = CreateWindowW(
			WC_BUTTON,
			L"Show Larger Chart",
			WS_CHILD | WS_VISIBLE,
			410, 350,
			150, 25,
			hwnd,
			(HMENU)ID_BIGGANTT,
			g_hInst,
			NULL
		);


		hFont = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Tahoma"));
		SendMessage(g_hwndField1, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndField2, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndField3, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndField4, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndField5, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndField6, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndLabel1, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndLabel2, WM_SETFONT, (WPARAM)hFont, TRUE);
		SendMessage(g_hwndLabel3, WM_SETFONT, (WPARAM)hFont, TRUE);

		hButtonFont = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
		SendMessage(g_hwndBuildButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndAddButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndResetButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndOptFCFS, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndOptRR, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndOptSJF, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndOptSRTF, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndBigGanttButton, WM_SETFONT, (WPARAM)hButtonFont, TRUE);

		hGroupFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
		SendMessage(g_hwndGroupAdd, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndGroupGantt, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
		SendMessage(g_hwndGroupAlg, WM_SETFONT, (WPARAM)hButtonFont, TRUE);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case ID_ADD:
			ProcessAddButton();
			break;
		case ID_RESET:
		{
			ListView_DeleteAllItems(g_hwndList);
			g_cProcess = 0;
			SetWindowTextW(g_hwndField5, L"...");
			SetWindowTextW(g_hwndField6, L"...");
			g_gantt.clear();
			InvalidateRect(hwnd, NULL, TRUE);
		}
		break;
		case ID_BIGGANTT:
		{
			GetWindowRect(hwnd, &pos);
			RegDialogClass();
			CreateWindowW(L"DialogClass", L"Gantt Chart",
				WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
				pos.left - 200, pos.top + 200, 1000, 150, NULL, NULL, g_hInst, NULL
			);
		}
		break;
		case ID_OPT_FCFS:
			g_algorithm = 0;
			break;
		case ID_OPT_SJF:
			g_algorithm = 1;
			break;
		case ID_OPT_SRTF:
			g_algorithm = 2;
			break;
		case ID_OPT_RR:
			g_algorithm = 3;
			break;
		case ID_BUILD:
		{
			if (g_cProcess <= 0)
			{
				MessageBoxW(g_hwndMain, L"Nhập ít nhất 1 tiến trình!", L"Lỗi", MB_OK);
				return 0;
			}
			switch (g_algorithm)
			{
			case 0:
				g_gantt = FCFS(processes, g_cProcess);
				break;
			case 1:
				g_gantt = SJF(processes, g_cProcess);
				break;
			case 2:
				g_gantt = SRTF(processes, g_cProcess);
				break;
			case 3:
				TCHAR tempQQ[5];
				GetWindowTextW(g_hwndField4, tempQQ, 5);
				if (_wtoi(tempQQ) <= 0)
				{
					MessageBoxW(g_hwndMain, L"qq không hợp lệ!", L"Lỗi", MB_OK);
					return 0;
				}
				g_gantt = RR(processes, g_cProcess, _wtoi(tempQQ));
				break;
			default:
				MessageBoxW(g_hwndMain, L"Chưa chọn thuật toán!", L"Lỗi", MB_OK);
				return 0;
			}

			static RECT updateArea = { 5, 240, 560, 340 };
			InvalidateRect(hwnd, &updateArea, TRUE);
			TCHAR tempField[50] = L"";
			calcWaitingTime(processes, g_cProcess, tempField);
			SetWindowTextW(g_hwndField5, tempField);
			wcscpy_s(tempField, L"");
			calcTurnAroundTime(processes, g_cProcess, tempField);
			SetWindowTextW(g_hwndField6, tempField);
		}
		break;
		case ID_HELP_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_DIALOG1), hwnd, About);
			break;
		}
	}
	break;

	case WM_CTLCOLORSTATIC:
	{
		//hdcStatic = (HDC)wParam;
		//SetBkColor(hdcStatic, RGB(255, 255, 255));
		if (hbrBkgnd == NULL)
		{
			hbrBkgnd = CreateSolidBrush(RGB(255, 255, 255));
		}
		return (INT_PTR)hbrBkgnd;
	}
	break;

	case WM_PAINT:
	{

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		if (!g_gantt.empty())
		{
			TCHAR temp[10];
			HFONT hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
			int tBurstTime = g_gantt.back().endTime; //Total Burst Time
			int pos_x1, pos_x2;
			RECT textArea;
			Process tempProcess;
			//HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));
			SelectObject(hdc, GetStockObject(DC_PEN));
			SelectObject(hdc, hFont);
			SetDCPenColor(hdc, RGB(150, 150, 150));
			Rectangle(hdc, GANTT_X, GANTT_Y, GANTT_X + GANTT_WIDTH, GANTT_Y + GANTT_HEIGHT);
			for (int i = 0; i < g_gantt.size(); i++)
			{
				tempProcess = g_gantt[i];
				pos_x1 = GANTT_X + (float)tempProcess.startTime / tBurstTime * GANTT_WIDTH;
				pos_x2 = GANTT_X + (float)tempProcess.endTime / tBurstTime * GANTT_WIDTH;
				MoveToEx(hdc, pos_x1, GANTT_Y, NULL);
				LineTo(hdc, pos_x1, GANTT_Y + GANTT_HEIGHT);
				textArea = { pos_x1,GANTT_Y,pos_x2,GANTT_Y + GANTT_HEIGHT };
				DrawText(hdc, tempProcess.id, -1, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				_itow_s(tempProcess.startTime, temp, 10, 10);
				TextOut(hdc, pos_x1, GANTT_Y + GANTT_HEIGHT + 5, temp, wcsnlen_s(temp, 10));
				//_itow_s(pos_x1 - GANTT_X, temp, 10);//
				//MessageBoxW(g_hwndMain, temp, L"Lỗi", MB_OK);//
			}
			_itow_s(tBurstTime, temp, 10, 10);
			TextOut(hdc, GANTT_X + GANTT_WIDTH - 10, GANTT_Y + GANTT_HEIGHT + 5, temp, wcsnlen_s(temp, 10));

			//SelectObject(hdc, original);
			DeleteObject(hFont);
		}
		EndPaint(hwnd, &ps);
	}
	break;

	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		if (!g_gantt.empty())
		{
			TCHAR temp[10];
			HFONT hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Tahoma");
			int tBurstTime = g_gantt.back().endTime;//
			int pos_x1, pos_x2;
			RECT textArea;
			Process tempProcess;
			//HGDIOBJ original = SelectObject(hdc, GetStockObject(DC_PEN));
			SelectObject(hdc, GetStockObject(DC_PEN));
			SelectObject(hdc, hFont);
			SetDCPenColor(hdc, RGB(150, 150, 150));
			Rectangle(hdc, BGANTT_X, BGANTT_Y, BGANTT_X + BGANTT_WIDTH, BGANTT_Y + BGANTT_HEIGHT);
			for (int i = 0; i < g_gantt.size(); i++)
			{
				tempProcess = g_gantt[i];
				pos_x1 = BGANTT_X + (float)tempProcess.startTime / tBurstTime * BGANTT_WIDTH;
				pos_x2 = BGANTT_X + (float)tempProcess.endTime / tBurstTime * BGANTT_WIDTH;
				MoveToEx(hdc, pos_x1, BGANTT_Y, NULL);
				LineTo(hdc, pos_x1, BGANTT_Y + BGANTT_HEIGHT);
				textArea = { pos_x1,BGANTT_Y,pos_x2,BGANTT_Y + BGANTT_HEIGHT };
				DrawText(hdc, tempProcess.id, -1, &textArea, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				_itow_s(tempProcess.startTime, temp, 10, 10);
				TextOut(hdc, pos_x1, BGANTT_Y + BGANTT_HEIGHT + 5, temp, wcsnlen_s(temp, 10));
				//_itow_s(pos_x1 - BGANTT_X, temp, 10);//
				//MessageBoxW(g_hwndMain, temp, L"Lỗi", MB_OK);//
			}
			_itow_s(tBurstTime, temp, 10, 10);
			TextOut(hdc, BGANTT_X + BGANTT_WIDTH - 10, BGANTT_Y + BGANTT_HEIGHT + 5, temp, wcsnlen_s(temp, 10));

			//SelectObject(hdc, original);
			DeleteObject(hFont);
		}
		EndPaint(hwnd, &ps);
		break;
	}
	case WM_DESTROY:
		DestroyWindow(hwnd);
		return 0;
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

INT_PTR CALLBACK About(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			EndDialog(hwnd, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;		
	default:
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

HWND CreateListView(HWND hwndParent) {
	TCHAR szString[3][15] = { L"Process", L"Cpu Burst Time", L"Arrival Time" };

	HWND g_hwndList = CreateWindowW(
		WC_LISTVIEW,
		L"",
		WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
		15, 20,
		280, 210,
		hwndParent,
		0,
		g_hInst,
		NULL
	);
	ListView_SetExtendedListViewStyle(g_hwndList, LVS_EX_FULLROWSELECT);

	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_CENTER;
	lvc.cx = 60; //col width

	lvc.pszText = szString[0];
	ListView_InsertColumn(g_hwndList, 0, &lvc);

	lvc.cx = 100;
	lvc.pszText = szString[1];
	ListView_InsertColumn(g_hwndList, 1, &lvc);

	lvc.pszText = szString[2];
	ListView_InsertColumn(g_hwndList, 2, &lvc);
	return g_hwndList;
}

void InsertListViewItems(HWND g_hwndListView, Process process)
{
	TCHAR temp[10];
	LVITEM lvI;
	if (g_cProcess >= 20) {
		MessageBoxW(g_hwndMain, L"Số lượng tiến trình không quá 20!", L"Lỗi", MB_OK);
	}

	lvI.mask = LVIF_TEXT | LVIF_STATE;
	lvI.stateMask = 0;
	lvI.iSubItem = 0;
	lvI.state = 0;

	lvI.iItem = g_cProcess;
	lvI.pszText = process.id;
	ListView_InsertItem(g_hwndListView, &lvI);

	lvI.iItem = g_cProcess;
	lvI.iSubItem = 1;
	_itow_s(process.burstTime, temp, 10, 10);
	lvI.pszText = temp;
	ListView_SetItem(g_hwndListView, &lvI);

	lvI.iItem = g_cProcess;
	lvI.iSubItem = 2;
	_itow_s(process.arrivalTime, temp, 10, 10);
	lvI.pszText = temp;
	ListView_SetItem(g_hwndListView, &lvI);
}

BOOL ProcessSpecialKeys(__in MSG* pMsg) //TAB & RETURN
{
	switch (pMsg->message)
	{
	case WM_CHAR:
		switch (pMsg->wParam)
		{
		case VK_TAB:
		{
			HWND hwnd = GetFocus();
			if (hwnd == g_hwndField1)
			{
				hwnd = g_hwndField2;
			}
			else if (hwnd == g_hwndField2)
			{
				hwnd = g_hwndField3;
			}
			else if (hwnd == g_hwndField3)
			{
				hwnd = g_hwndField1;
			}
			SetFocus(hwnd);
			return TRUE;
		}
		case VK_RETURN:
			ProcessAddButton();
			return TRUE;

		default:
			break;
		}
	default:
		break;
	}

	return FALSE;
}

void ProcessAddButton()
{
	TCHAR temp[10];
	GetWindowText(g_hwndField1, processes[g_cProcess].id, 15);
	GetWindowText(g_hwndField2, temp, 10);
	processes[g_cProcess].burstTime = _wtoi(temp);
	GetWindowText(g_hwndField3, temp, 10);
	processes[g_cProcess].arrivalTime = _wtoi(temp);
	//validate data
	if (wcslen(processes[g_cProcess].id) == 0 || processes[g_cProcess].burstTime <= 0 || processes[g_cProcess].arrivalTime < 0) {
		MessageBoxW(g_hwndMain, L"Dữ liệu nhập vào không hợp lệ!", L"Lỗi", MB_OK);
		SetFocus(g_hwndField1);
		return;
	}
	for (int i = 0; i < g_cProcess; i++)
	{
		if (wcscmp(processes[g_cProcess].id, processes[i].id) == 0) {
			MessageBoxW(g_hwndMain, L"Tiến trình đã tồn tại", L"Lỗi", MB_OK);
			SetFocus(g_hwndField1);
			return;
		}
	}
	//end validate data
	InsertListViewItems(g_hwndList, processes[g_cProcess]);
	g_cProcess++;

	SetWindowTextW(g_hwndField1, L"");
	SetWindowTextW(g_hwndField2, L"");
	SetWindowTextW(g_hwndField3, L"");
	SetFocus(g_hwndField1);
}

void RegDialogClass() {
	WNDCLASS wc = { };

	wc.lpszClassName = L"DialogClass";
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DialogProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = g_hInst;
	wc.hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(IDI_ICON1));
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	RegisterClass(&wc);
}


//get last error message
void getLastE()
{
	wchar_t buf[256];
	FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buf, (sizeof(buf) / sizeof(wchar_t)), NULL);
	MessageBoxW(g_hwndMain, buf, L"err", MB_OK);
}

void AddMenu(HWND hwnd) {
	HMENU hMenu = CreateMenu();
	HMENU hMenuBar = CreateMenu();
	AppendMenu(hMenu, MF_STRING, ID_HELP_ABOUT, L"About...");
	AppendMenu(hMenuBar, MF_POPUP, (UINT_PTR)hMenu, L"Help");
	SetMenu(hwnd, hMenuBar);
}
#include "stdafx.h"
#include "GalsPanic.h"
#include <vector>
using namespace std;
#define MAX_LOADSTRING 100
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
// >> :

// 플레이어
class Circle										
{
public:
	POINT cirPoint;
	double radius = 5;
};
void DrawCircle(HDC hdc, POINT point, double r);				// 플레이어를 그려주는 함수
// 보조 함수
bool operator==(const POINT &a, const POINT &b);				// POINT 같음을 쉽게 판정하기 위한 연산자설정
bool DotInFigure(int x, int y, const vector<POINT> &p);			// 점이 다각형 내부에 있는지 판단 함수
// 점의 상태
#define MIN(x,y) (x < y ? x : y)								// 작은 값 판정 매크로			0 : 꼭지점
#define MAX(x,y) (x > y ? x : y)								// 큰 값 판정 매크로			1 : 모서리
int DotState(POINT p, vector<POINT> polygon);					// 점의 상태 판정 함수			2 : 외부
int DotState(int x, int y, vector<POINT> polygon);				//                             -1 : 내부
// 이동 판단
bool MOVE(int start, int end);

vector<POINT> BasicPoint;
vector<POINT>::iterator Bit;
vector<POINT> PaintPoint;
vector<POINT>::iterator Pit;



int OnEdge(POINT p, vector<POINT> polygon);
int OnEdge(int x, int y, vector<POINT> polygon);
int AtNode(POINT p, vector<POINT> polygon);
int AtNode(int x, int y, vector<POINT> polygon);

int StPEnP[2] = { 0,0 };
// >> :
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GALSPANIC, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GALSPANIC));
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GALSPANIC));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GALSPANIC);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPED, 100, 100, 800, 800, nullptr, nullptr, hInstance, nullptr);
   if (!hWnd)
   {	return FALSE;   }
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   return TRUE;
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC			hdc;
	PAINTSTRUCT ps;
	static Circle Circle;
	static int Node;
    switch (message)
    {
	case WM_CREATE:
		BasicPoint.push_back({ 350,350 });
		BasicPoint.push_back({ 450,350 });
		BasicPoint.push_back({ 450,250 });
		BasicPoint.push_back({ 550,250 });
		BasicPoint.push_back({ 550,450 });
		BasicPoint.push_back({ 350,450 });
		Circle.cirPoint.x = BasicPoint[0].x;
		Circle.cirPoint.y = BasicPoint[0].y;
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_KEYDOWN:
		//if (wParam == VK_RIGHT)
		if (wParam == VK_RIGHT)
		{
			Node = AtNode(Circle.cirPoint, BasicPoint);
			if (Node != -1)
			{
				if (AtNode(Circle.cirPoint.x + 2, Circle.cirPoint.y, BasicPoint) != -1)
				{
					Circle.cirPoint.x += 2;
					Node = AtNode(Circle.cirPoint, BasicPoint);
				}
				else if (OnEdge(Circle.cirPoint.x + 2, Circle.cirPoint.y, BasicPoint) != -1)
				{
					Circle.cirPoint.x += 2;
					Node = OnEdge(Circle.cirPoint, BasicPoint);
				}
				else if (!(DotInFigure(Circle.cirPoint.x + 2, Circle.cirPoint.y, BasicPoint)))
				{
					PaintPoint.push_back(Circle.cirPoint);
					Circle.cirPoint.x += 2;
					StPEnP[0] = Node;
				}
			}
			else
			{
				Node = OnEdge(Circle.cirPoint, BasicPoint);
				if (Node != -1)
				{
					if (AtNode(Circle.cirPoint.x + 2, Circle.cirPoint.y, BasicPoint) != -1)
					{
						Circle.cirPoint.x += 2;
						Node = AtNode(Circle.cirPoint, BasicPoint);
					}
					else if (OnEdge(Circle.cirPoint.x + 2, Circle.cirPoint.y, BasicPoint) != -1)
					{
						Circle.cirPoint.x += 2;
					}
					else if (!(DotInFigure(Circle.cirPoint.x + 2, Circle.cirPoint.y, BasicPoint)))
					{
						PaintPoint.push_back(Circle.cirPoint);
						Circle.cirPoint.x += 2;
						StPEnP[0] = Node;
					}
				}
			}
		}
		else if (wParam == VK_DOWN)
		{
			Node = AtNode(Circle.cirPoint, BasicPoint);
			if (Node != -1)
			{
				if (AtNode(Circle.cirPoint.x, Circle.cirPoint.y+2, BasicPoint) != -1)
				{
					Circle.cirPoint.y += 2;
					Node = AtNode(Circle.cirPoint, BasicPoint);
				}
				else if (OnEdge(Circle.cirPoint.x, Circle.cirPoint.y+2, BasicPoint) != -1)
				{
					Circle.cirPoint.y += 2;
					Node = OnEdge(Circle.cirPoint, BasicPoint);
				}
				else if (!(DotInFigure(Circle.cirPoint.x, Circle.cirPoint.y+2, BasicPoint)))
				{
					PaintPoint.push_back(Circle.cirPoint);
					Circle.cirPoint.y += 2;
					StPEnP[0] = Node;
				}
			}
			else
			{
				Node = OnEdge(Circle.cirPoint, BasicPoint);
				if (Node != -1)
				{
					if (AtNode(Circle.cirPoint.x, Circle.cirPoint.y+2, BasicPoint) != -1)
					{
						Circle.cirPoint.y += 2;
						Node = AtNode(Circle.cirPoint, BasicPoint);
					}
					else if (OnEdge(Circle.cirPoint.x, Circle.cirPoint.y+2, BasicPoint) != -1)
					{
						Circle.cirPoint.y += 2;
					}
					else if (!(DotInFigure(Circle.cirPoint.x, Circle.cirPoint.y+2, BasicPoint)))
					{
						PaintPoint.push_back(Circle.cirPoint);
						Circle.cirPoint.y += 2;
						StPEnP[0] = Node;
					}
				}
			}
		}
		else if (wParam == VK_UP)
		{
			Node = AtNode(Circle.cirPoint, BasicPoint);
			if (Node != -1)
			{
				if (AtNode(Circle.cirPoint.x, Circle.cirPoint.y - 2, BasicPoint) != -1)
				{
					Circle.cirPoint.y -= 2;
					Node = AtNode(Circle.cirPoint, BasicPoint);
				}
				else if (OnEdge(Circle.cirPoint.x, Circle.cirPoint.y - 2, BasicPoint) != -1)
				{
					Circle.cirPoint.y -= 2;
					Node = OnEdge(Circle.cirPoint, BasicPoint);
				}
				else if (!(DotInFigure(Circle.cirPoint.x, Circle.cirPoint.y - 2, BasicPoint)))
				{
					PaintPoint.push_back(Circle.cirPoint);
					Circle.cirPoint.y -= 2;
					StPEnP[0] = Node;
				}
			}
			else  
			{
				Node = OnEdge(Circle.cirPoint, BasicPoint);
				if (Node != -1)
				{
					if (AtNode(Circle.cirPoint.x, Circle.cirPoint.y - 2, BasicPoint) != -1)
					{
						Circle.cirPoint.y -= 2;
						Node = AtNode(Circle.cirPoint, BasicPoint);
					}
					else if (OnEdge(Circle.cirPoint.x, Circle.cirPoint.y - 2, BasicPoint) != -1)
					{
						Circle.cirPoint.y -= 2;
					}
					else if (!(DotInFigure(Circle.cirPoint.x, Circle.cirPoint.y - 2, BasicPoint)))
					{
						PaintPoint.push_back(Circle.cirPoint);
						Circle.cirPoint.y -= 2;
						StPEnP[0] = Node;
					}
				}
			}
		}
		else if (wParam == VK_LEFT)
		{
			Node = AtNode(Circle.cirPoint, BasicPoint);
			if (Node != -1)
			{
				if (AtNode(Circle.cirPoint.x - 2, Circle.cirPoint.y, BasicPoint) != -1)
				{
					Circle.cirPoint.x -= 2;
					Node = AtNode(Circle.cirPoint, BasicPoint);
				}
				else if (OnEdge(Circle.cirPoint.x - 2, Circle.cirPoint.y, BasicPoint) != -1)
				{
					Circle.cirPoint.x -= 2;
					Node = OnEdge(Circle.cirPoint, BasicPoint);
				}
				else if (!(DotInFigure(Circle.cirPoint.x - 2, Circle.cirPoint.y, BasicPoint)))
				{
					PaintPoint.push_back(Circle.cirPoint);
					Circle.cirPoint.x -= 2;
					StPEnP[0] = Node;
				}
			}
			else  
			{
				Node = OnEdge(Circle.cirPoint, BasicPoint);
				if (Node != -1)
				{
					if (AtNode(Circle.cirPoint.x - 2, Circle.cirPoint.y, BasicPoint) != -1)
					{
						Circle.cirPoint.x -= 2;
						Node = AtNode(Circle.cirPoint, BasicPoint);
					}
					else if (OnEdge(Circle.cirPoint.x - 2, Circle.cirPoint.y, BasicPoint) != -1)
					{
						Circle.cirPoint.x -= 2;
					}
					else if (!(DotInFigure(Circle.cirPoint.x - 2, Circle.cirPoint.y, BasicPoint)))
					{
						PaintPoint.push_back(Circle.cirPoint);
						Circle.cirPoint.x -= 2;
						StPEnP[0] = Node;
					}
				}
			}
		}
		InvalidateRgn(hWnd, NULL, TRUE);
		break;
    case WM_PAINT:
        {
            hdc = BeginPaint(hWnd, &ps);
			Polygon(hdc, &BasicPoint[0], BasicPoint.size());
			DrawCircle(hdc, Circle.cirPoint, Circle.radius);
			TextOut(hdc, 720, 10, to_wstring(Circle.cirPoint.x).c_str(), _tcslen(to_wstring(Circle.cirPoint.x).c_str()));
			TextOut(hdc, 720, 30, to_wstring(Circle.cirPoint.y).c_str(), _tcslen(to_wstring(Circle.cirPoint.y).c_str()));
			TextOut(hdc, 720, 50, to_wstring(Node).c_str(), _tcslen(to_wstring(Node).c_str()));
			TextOut(hdc, 650, 70, _T("출발점 : "), 5);
			TextOut(hdc, 720, 70, to_wstring(StPEnP[0]).c_str(), _tcslen(to_wstring(StPEnP[0]).c_str()));
			TextOut(hdc, 650, 90, _T("도착점 : "), 5);
			TextOut(hdc, 720, 90, to_wstring(StPEnP[1]).c_str(), _tcslen(to_wstring(StPEnP[1]).c_str()));
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
void DrawCircle(HDC hdc, POINT point, double r)
{
	Ellipse(hdc, point.x - r, point.y - r, point.x + r, point.y + r);
}
bool operator==(const POINT &a, const POINT &b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}
bool DotInFigure(int x, int y, const vector<POINT> &p) {
	int crosses = 0; // (x, y)부터 오른쪽 반직선을 출발해 다각형 p와의 교점의 개수
	for (int i = 0; i < p.size(); i++)
	{
		int j = (i + 1) % p.size();
		//점 (x, y)가 선분 (p[i], p[j])의 y좌표 사이에 있음
		if ((p[i].y > y) != (p[j].y > y)) {
			//atX는 점 B를 지나는 수평선과 선분 (p[i], p[j])의 교점
			double atX = (p[j].x - p[i].x)*(y - p[i].y) / (p[j].y - p[i].y) + p[i].x;
			//atX가 오른쪽 반직선과의 교점이 맞으면 교점의 개수를 증가시킨다.
			if (x <= atX)
				crosses++;
		}
	}
	return crosses % 2 > 0;
}
int DotState(POINT p, vector<POINT> polygon)
{
	POINT p1, p2;
	int i;

	for (i = 1; i <= polygon.size(); i++)
	{
		p1.x = polygon[i - 1].x;					p1.y = polygon[i - 1].y;
		p2.x = polygon[i % polygon.size()].x;		p2.y = polygon[i % polygon.size()].y;
		if ((MIN(p1.x, p2.x) <= p.x) && (p.x <= MAX(p1.x, p2.x)))
		{
			if ((p2.x - p1.x == 0) && (p2.y - p1.y == 0))
				return 0;												// 꼭지점
			else if ((p2.x - p1.x == 0) && (p2.y - p1.y != 0))
				return 1;												// 가로 모서리
			else if ((p2.x - p1.x != 0) && (p2.y - p1.y == 0))
				return 1;												// 세로 모서리
			else if ((p2.x - p1.x != 0) && (p2.y - p1.y != 0))
			{
				if (DotInFigure(p.x, p.y, polygon))
					return -1;											// 다각형 내부
				else return 2;											// 다각형 외부
			}
		}
	}
}
int DotState(int x, int y, vector<POINT> polygon)
{
	POINT p1, p2;
	int i;

	for (i = 1; i <= polygon.size(); i++)
	{
		p1.x = polygon[i - 1].x;					p1.y = polygon[i - 1].y;
		p2.x = polygon[i % polygon.size()].x;		p2.y = polygon[i % polygon.size()].y;
		if ((MIN(p1.x, p2.x) <= x) && (x <= MAX(p1.x, p2.x)))
		{
			if ((p2.x - p1.x == 0) && (p2.y - p1.y == 0))
				return 0;												// 꼭지점
			else if ((p2.x - p1.x == 0) && (p2.y - p1.y != 0))
				return 1;												// 가로 모서리
			else if ((p2.x - p1.x != 0) && (p2.y - p1.y == 0))
				return 1;												// 세로 모서리
			else if ((p2.x - p1.x != 0) && (p2.y - p1.y != 0))
			{
				if (DotInFigure(x, y, polygon))
					return -1;											// 다각형 내부
				else return 2;											// 다각형 외부
			}
		}
	}
}
bool MOVE(int start, int end)
{
	switch (start)
	{
	case 0:	// 꼭지점 출발			
		switch (end)
		{
		case 0:					// 꼭지점 -> 꼭지점
			return true;
		case 1:					// 꼭지점 -> 모서리
			return true;
		case 2:					// 꼭지점 -> 외부
		{

			return true;
		}
		case -1:				// 꼭지점 -> 내부
			return false;
		}
	case 1:	// 모서리 출발
		switch (end)
		{
		case 0:					// 모서리 -> 꼭지점
			return true;
		case 1:					// 모서리 -> 모서리
			return true;
		case 2:					// 모서리 -> 외부
		{

			return true;
		}
		case -1:				// 모서리 -> 내부
			return false;
		}
	case 2:	// 외부 출발
		switch (end)
		{
		case 0:					// 외부 -> 꼭지점
		{

			return true;
		}
		case 1:					// 외부 -> 모서리
		{

			return true;
		}
		case 2:					// 외부 -> 외부
			return true;
		}
	}
}
int Edge(POINT p, vector<POINT> polygon)
{
	POINT p1, p2;
	int edge, i;

	for (i = 1; i <= polygon.size(); i++)
	{
		p1.x = polygon[i - 1].x;					p1.y = polygon[i - 1].y;
		p2.x = polygon[i % polygon.size()].x;		p2.y = polygon[i % polygon.size()].y;
		if ((MIN(p1.x, p2.x) <= p.x) && (p.x <= MAX(p1.x, p2.x)))
		{
			if ((p2.x - p1.x == 0) && (p2.y - p1.y == 0))
				edge = i - 1;												// 꼭지점
			else if ((p2.x - p1.x == 0) && (p2.y - p1.y != 0))
				edge = i - 1;												// 가로 모서리
			else if ((p2.x - p1.x != 0) && (p2.y - p1.y == 0))
				edge = i - 1;												// 세로 모서리
			else if ((p2.x - p1.x != 0) && (p2.y - p1.y != 0))
			{
				if (!DotInFigure(p.x, p.y, polygon))
					return 2;											// 다각형 외부
			}
		}
	}
}

int OnEdge(POINT p, vector<POINT> polygon)
{
	int pedge = -1;
	double fxy = 0;
	double a = 0;
	POINT p1, p2;
	int i;

	for (i = 1; i <= polygon.size(); i++)
	{
		p1.x = polygon[i - 1].x;					p1.y = polygon[i - 1].y;
		p2.x = polygon[i % polygon.size()].x;		p2.y = polygon[i % polygon.size()].y;
		if ((MIN(p1.x, p2.x) <= p.x) && (p.x <= MAX(p1.x, p2.x)))
		{
			if (p2.x - p1.x == 0)
			{
				pedge = i - 1;
				break;
			}
			a = (p2.y - p1.y) / (p2.x - p1.x);
			fxy = a*(p.x - p1.x) + p1.y - p.y;
			if (fxy == 0)
			{
				pedge = i - 1;
				break;
			}
		}
	}

	return pedge;
}
int OnEdge(int x, int y, vector<POINT> polygon)
{
	int pedge = -1;
	double fxy = 0;
	double a = 0;
	POINT p1, p2;
	int i;

	for (i = 1; i <= polygon.size(); i++)
	{
		p1.x = polygon[i - 1].x;					p1.y = polygon[i - 1].y;
		p2.x = polygon[i % polygon.size()].x;		p2.y = polygon[i % polygon.size()].y;
		if ((MIN(p1.x, p2.x) <= x) && (x <= MAX(p1.x, p2.x)))
		{
			if (p2.x - p1.x == 0)
			{
				pedge = i - 1;
				break;
			}
			a = (p2.y - p1.y) / (p2.x - p1.x);
			fxy = a*(x - p1.x) + p1.y - y;
			if (fxy == 0)
			{
				pedge = i - 1;
				break;
			}
		}
	}
	return pedge;
}
int AtNode(POINT p, vector<POINT> polygon)
{
	int i;
	int pnode = -1;

	for (i = 1; i <= polygon.size(); i++)
	{
		if ((polygon[i - 1].x == p.x) && (polygon[i - 1].y == p.y))
		{
			pnode = i - 1;
			break;
		}
	} 
	return pnode;
}
int AtNode(int x, int y, vector<POINT> polygon)
{
	int i;
	int pnode = -1;

	for (i = 1; i <= polygon.size(); i++)
	{
		if ((polygon[i - 1].x == x) && (polygon[i - 1].y == y))
		{
			pnode = i - 1;
			break;
		}
	}
	return pnode;
}
// vn_framework3D_2025.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "framework.h"
#include "vn_framework3D_2025.h"

#include "framework/vn_environment.h"


#define MAX_LOADSTRING 100




// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

HWND hWnd = NULL;								// ウィンドウのハンドル

//関数の前方宣言
bool FullScreenOn(HWND hWnd);
void FullScreenOff(HWND hWnd);

//クライアント領域のサイズ
int screen_width = SCREEN_WIDTH;
int screen_height = SCREEN_HEIGHT;
static bool isFullScreen = false;

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。


    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_VNFRAMEWORK3D2025, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VNFRAMEWORK3D2025));


    MSG msg;

    //ロケールの設定(日本)
    setlocale(LC_CTYPE, "JPN");

    //メインフレームの初期化
    vnMainFrame::initialize(hWnd, hInst, screen_width, screen_height);

    // メイン メッセージ ループ:
    while (true)
    {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                goto END;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        //メインフレームの実行
        vnMainFrame::execute();
    }

END:
    
    //メインフレームの終了
    vnMainFrame::terminate();
    ClipCursor(NULL);
    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスの登録だけをします。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VNFRAMEWORK3D2025));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = NULL;   //MAKEINTRESOURCEW(IDC_VNFRAMEWORK3D2025);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   StringCchCopyW(szTitle, MAX_LOADSTRING, L".CUBE");

   //ウィンドウ生成
   hWnd = CreateWindowW(
       szWindowClass,       //ウィンドウクラス
       szTitle,             //タイトル
       WS_OVERLAPPEDWINDOW,
       CW_USEDEFAULT,
       0,
       CW_USEDEFAULT,
       0,
       nullptr,
       nullptr,
       hInstance,
       nullptr);

   
   if (!hWnd)
   {
      return FALSE;
   }

   //クライアント領域が希望のサイズになるようにウィンドウ全体のサイズを計算して再設定
   RECT rw, rc;
   ::GetWindowRect(hWnd, &rw); // ウィンドウ全体のサイズ
   ::GetClientRect(hWnd, &rc); // クライアント領域のサイズ

   

   int new_width = (rw.right - rw.left) - (rc.right - rc.left) + screen_width;
   int new_height = (rw.bottom - rw.top) - (rc.bottom - rc.top) + screen_height;

   ::SetWindowPos(hWnd, nullptr, 0, 0, new_width, new_height, SWP_NOMOVE | SWP_NOZORDER);

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   //WM_PAINTを発生させないようにする
   ValidateRect(hWnd, 0);



   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
#if 0
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
#endif
    case WM_MOUSEMOVE:              //マウス座標の設定
        vnDirectInput::setMousePosition(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_KEYDOWN:				//キーが押された
        
        switch (wParam)
        {
        case VK_ESCAPE:
            PostQuitMessage(0);
            break;
        case VK_F11:
            if (!isFullScreen) {
                isFullScreen = FullScreenOn(hWnd);
            }
            else
            {
                FullScreenOff(hWnd);
                isFullScreen = false;
            }
            break;
        }


        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_SETFOCUS:
    {
        RECT rc;
        GetClientRect(hWnd, &rc);

        POINT ul = { rc.left, rc.top };
        POINT lr = { rc.right, rc.bottom };

        ClientToScreen(hWnd, &ul);
        ClientToScreen(hWnd, &lr);

        rc.left = ul.x;
        rc.top = ul.y;
        rc.right = lr.x;
        rc.bottom = lr.y;

        ClipCursor(&rc);
    }
        break;

    case WM_KILLFOCUS:
        ClipCursor(NULL);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

#if 0
// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
#endif

//フルスクリーンの切り替え
bool FullScreenOn(HWND hWnd)
{
    SetWindowLong(hWnd, GWL_STYLE, WS_POPUP);	//ｳｨﾝﾄﾞｳ種別を枠無しに変更

    //デバイスモードを変更
    DEVMODE devMode;
    devMode.dmSize = sizeof(DEVMODE); //構造体のｻｲｽﾞ
    devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;//幅、高さの設定
    devMode.dmPelsWidth = vnMainFrame::screenWidth;	//ｳｨﾝﾄﾞｳの幅
    devMode.dmPelsHeight = vnMainFrame::screenHeight;	//ｳｨﾝﾄﾞｳの高さ

    //解像度の変更
    LONG lResult = ChangeDisplaySettings(&devMode, CDS_FULLSCREEN);
    if (lResult != DISP_CHANGE_SUCCESSFUL)
    {

        MessageBox(NULL, L"ディスプレイの設定変更エラー", L"ERROR", MB_OK);
        return false;
    }
    ShowWindow(hWnd, SW_SHOW);	//画面更新

    //ｳｨﾝﾄﾞｳの大きさを変更
    SetWindowPos(hWnd,
        HWND_TOPMOST,		//最前面のウィンドウにする
        0, 0,				//始点
        vnMainFrame::screenWidth,
        vnMainFrame::screenHeight,		//始点
        SWP_NOSIZE);		//ｵﾌﾟｼｮﾝ

    return true;
}

//フルスクリーン解除
void FullScreenOff(HWND hWnd)
{
    SetWindowLong(hWnd, GWL_STYLE,
        WS_OVERLAPPEDWINDOW);	//ｳｨﾝﾄﾞｳ種別を戻す

    LONG lResult = ChangeDisplaySettings(NULL, 0);//解像度を戻す

    ShowWindow(hWnd, SW_SHOW);	//画面更新
    SetWindowPos(hWnd, HWND_TOPMOST, 100, 50, 0, 0, SWP_NOSIZE);
}

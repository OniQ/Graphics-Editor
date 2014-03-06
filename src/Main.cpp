// Main.cpp : Defines the entry point for the application.

#include "stdafx.h"
#include "Main.h"
  
int  WINAPI _tWinMain(  HINSTANCE hInstance,
                      HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MSG msg;
    HACCEL hAccelTable;

    myRegisterClass(hInstance);

    // Perform application initialization:
    if (!initInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MAIN));

    // Main message loop:
    while (GetMessage(&msg, NULL, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}

int InitGL(GLvoid)                                        // All Setup For OpenGL Goes Here
{
    glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);                // Black Background
    glClearDepth(1.0f);                                    // Depth Buffer Setup
    glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations
    primitive = GL_POLYGON;
    objCamera.Position_Camera(0, 2.5f, 5, 0, 2.5f, 0, 0, 1, 0); 
    glEnable(GL_TEXTURE_2D); 
    return TRUE; 
}

GLvoid reSizeGLScene(GLsizei width, GLsizei height)        // Resize And Initialize The GL Window
{
    if (height == 0)                                        // Prevent A Divide By Zero By
    {
        height = 1;                                        // Making Height Equal One
    }

    glViewport(0, 0, width, height);                        // Reset The Current Viewport

    glMatrixMode(GL_PROJECTION);                        // Select The Projection Matrix
    glLoadIdentity();                                    // Reset The Projection Matrix

    // Calculate The Aspect Ratio Of The Window
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

    glMatrixMode(GL_MODELVIEW);                            // Select The Modelview Matrix
    glLoadIdentity();                                    // Reset The Modelview Matrix
    SwapBuffers(hdc);
    UpdateWindow(mainWindow);
}

int DrawGLScene(GLvoid)                                    // Here's Where We Do All The Drawing
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear Screen And Depth Buffer
    glLoadIdentity();                                    // Reset The Current Modelview Matrix 

    // use this function for opengl target camera
    gluLookAt(objCamera.mPos.x, objCamera.mPos.y, objCamera.mPos.z,
        objCamera.mView.x, objCamera.mView.y, objCamera.mView.z,
        objCamera.mUp.x, objCamera.mUp.y, objCamera.mUp.z);

    glTranslatef(0, 1.0f, 0);
    for (std::vector<Shape*>::iterator it = objects.begin(); it != objects.end(); ++it){
        if (!(*it)->hidden){
                glPushMatrix();
                glTranslatef((*it)->t_x, (*it)->t_y, (*it)->t_z);
                glRotatef((*it)->rotation, 0.0f, 0.0f, 1.0f);
                (*it)->primitive = primitive;
                (*it)->draw();
                glPopMatrix();
        }

    }
    SwapBuffers(hdc);
    UpdateWindow(hwndView);
    return TRUE;                                         
}
 
ATOM myRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style            = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnwndProc      = wndProc;
    wcex.cbClsExtra       = 0;
    wcex.cbWndExtra       = 0;
    wcex.hInstance        = hInstance;
    wcex.hIcon            = NULL;
    wcex.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground    = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName     = NULL;
    wcex.lpszClassName    = szWindowClass;
    wcex.hIconSm          = NULL;

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL, TEXT("Main window class registration failed!"),
            szWindowClass, MB_ICONERROR);
        return 0;
    } 

    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnwndProc = ViewwndProc;
    wcex.cbWndExtra = sizeof (long);
    wcex.hIcon = NULL;
    wcex.hIconSm = NULL;
    wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
    wcex.lpszClassName = szViewWindowClass;

    return RegisterClassEx(&wcex);
}

BOOL initInstance(HINSTANCE hInstance, int nCmdShow)
{ 

   hInst = hInstance; // Store instance handle in our global variable

   mainWindow = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      50, 50, 1024, 600, NULL, NULL, hInstance, NULL);

   if (!mainWindow)
   {
      return FALSE;
   }

   HGLRC        hRC;
   hwndView = CreateWindow(szViewWindowClass, TEXT("View"),
       WS_TILED | WS_CAPTION | WS_CHILDWINDOW | WS_VISIBLE,
       0, 0, 200, 100,
       mainWindow, (HMENU)ID_VIEW,
       hInst,
       NULL);
   hwndEdit = CreateWindow(TEXT("Edit"), 
       NULL, 
       WS_BORDER | WS_CHILD | WS_VISIBLE | WS_VSCROLL |
       ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
       10, 10, 243, 230, 
       mainWindow, (HMENU)ID_EDITOR,
       hInst, NULL);


   static    PIXELFORMATDESCRIPTOR pfd =                // pfd Tells Windows How We Want Things To Be
   {
       sizeof(PIXELFORMATDESCRIPTOR),                // Size Of This Pixel Format Descriptor
       1,                                            // Version Number
       PFD_DRAW_TO_WINDOW |                        // Format Must Support Window
       PFD_SUPPORT_OPENGL |                        // Format Must Support OpenGL
       PFD_DOUBLEBUFFER,                            // Must Support Double Buffering
       PFD_TYPE_RGBA,                                // Request An RGBA Format
       16,                                            // Select Our Color Depth
       0, 0, 0, 0, 0, 0,                            // Color Bits Ignored
       0,                                            // No Alpha Buffer
       0,                                            // Shift Bit Ignored
       0,                                            // No Accumulation Buffer
       0, 0, 0, 0,                                    // Accumulation Bits Ignored
       16,                                            // 16Bit Z-Buffer (Depth Buffer)  
       0,                                            // No Stencil Buffer
       0,                                            // No Auxiliary Buffer
       PFD_MAIN_PLANE,                                // Main Drawing Layer
       0,                                            // Reserved
       0, 0, 0                                        // Layer Masks Ignored
   };
   hdc = GetDC(hwndView);
   if (!(PixelFormat = ChoosePixelFormat(hdc, &pfd)))    // Did Windows Find A Matching Pixel Format?
   {                             // Reset The Display
       MessageBox(NULL, TEXT("Can't Find A Suitable PixelFormat."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
       return FALSE;                                // Return FALSE
   }
   if (!(PixelFormat = ChoosePixelFormat(hdc, &pfd)))    // Did Windows Find A Matching Pixel Format?
   {
       MessageBox(NULL, TEXT("Can't Find A Suitable PixelFormat."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
       return FALSE;                                // Return FALSE
   }

   if (!SetPixelFormat(hdc, PixelFormat, &pfd))        // Are We Able To Set The Pixel Format?
   {
       MessageBox(NULL, TEXT("Can't Set The PixelFormat."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
       return FALSE;                                // Return FALSE
   }

   if (!(hRC = wglCreateContext(hdc)))                // Are We Able To Get A Rendering Context?
   {
       MessageBox(NULL, TEXT("Can't Create A GL Rendering Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
       return FALSE;                                // Return FALSE
   }

   if (!wglMakeCurrent(hdc, hRC))                    // Try To Activate The Rendering Context
   {
       MessageBox(NULL, TEXT("Can't Activate The GL Rendering Context."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
       return FALSE;                                // Return FALSE
   }
   if (!InitGL())                                    // Initialize Our Newly Created GL Window
   {                             // Reset The Display
       MessageBox(NULL, TEXT("Initialization Failed."), TEXT("ERROR"), MB_OK | MB_ICONEXCLAMATION);
       return FALSE;                                // Return FALSE
   }
   ShowWindow(mainWindow, nCmdShow);
   UpdateWindow(mainWindow);
   UpdateWindow(hEdit);
   UpdateWindow(hwndView);

   return TRUE;
}
 
LRESULT CALLBACK wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    int wmId, wmEvent;   
    static int dwPos;
    switch (message) 
    { 

    case WM_CREATE:{ 
        toolBar = createToolbar(hWnd); 
        objectSelector = createComboEx(hWnd); 
        initButtons(hWnd);
        break;
    }  
    case WM_SIZE:{ 
        width = LOWORD(lParam);
        height = HIWORD(lParam);
        MoveWindow(toolBar, 0, 20, width, 5, TRUE);
        MoveWindow(hwndEdit, 0, 40, width / 2 - 5, height - 40, TRUE); 
        MoveWindow(hwndView, width / 2, 40, width / 2, height / 2, TRUE);
        MoveWindow(objectSelector, width/2+10, height/2+100, 100, 100, TRUE);
        MoveWindow(checkBox, width / 2 + 120, height / 2 + 100, 120, 20, TRUE);
        MoveWindow(deleteButton, width / 2 + 245, height / 2 + 100, 40, 40, TRUE);

        MoveWindow(controlButtons[0], width / 2 + 450, height / 2 + 180, 40, 40, TRUE);
        MoveWindow(controlButtons[1], width / 2 + 350, height / 2 + 180, 40, 40, TRUE);
        MoveWindow(controlButtons[2], width / 2 + 400, height / 2 + 120, 40, 40, TRUE);
        MoveWindow(controlButtons[3], width / 2 + 400, height / 2 + 240, 40, 40, TRUE);

        MoveWindow(controlButtons[4], width / 2 + 450, height / 2 + 300, 40, 40, TRUE);
        MoveWindow(controlButtons[5], width / 2 + 350, height / 2 + 300, 40, 40, TRUE);
        reSizeGLScene(width / 2, height / 2);
        DrawGLScene();
        break; 
    }
    case WM_LBUTTONDOWN:{
        SetFocus(mainWindow);
        break;
    }
    case WM_MOUSEWHEEL:
        if ((short)HIWORD(wParam) > 0)
        {
            objCamera.Move_Camera(CAMERASPEED);
            DrawGLScene();
        }
        if ((short)HIWORD(wParam) < 0)
        {
            objCamera.Move_Camera(-CAMERASPEED);
            DrawGLScene();
        }
        break;
    case WM_COMMAND:
        wmId    = LOWORD(wParam);
        wmEvent = HIWORD(wParam);   
        if (HIWORD(wParam) == CBN_SELCHANGE)
            // If the user makes a selection from the list:
            //   Send CB_GETCURSEL message to get the index of the selected list item.
            //   Send CB_GETLBTEXT message to get the item.
        { 
            int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
                (WPARAM)0, (LPARAM)0);
            if (ItemIndex != CB_ERR){
                EnableWindow(deleteButton, TRUE);  
                LPCSTR nameStr = objNames[ItemIndex].c_str();
                fileToEdit(nameStr); 
                int hid = BST_CHECKED; 
                selected = objects[ItemIndex];
                currentObjIndex = ItemIndex;
                if (objects[ItemIndex]->hidden == true){
                    hid = BST_UNCHECKED;
                }
                else{
                    hid = BST_CHECKED;
                }
                CheckDlgButton(hWnd, ID_CHECK_DISP_OBJ, hid);
            }
            else{
                CheckDlgButton(hWnd, ID_CHECK_DISP_OBJ, BST_INDETERMINATE);
            }
        }
        switch (wmId)
        {
        case ID_CHECK_DISP_OBJ:
            if (wmEvent == BN_CLICKED)
            {
                dwPos = SendMessage(checkBox, BM_GETCHECK, 0, 0);
                if (dwPos == BST_CHECKED)
                {
                    CheckDlgButton(hWnd, ID_CHECK_DISP_OBJ, BST_UNCHECKED);
                    selected->hidden = true;
                    DrawGLScene();

                }
                else if (dwPos == BST_UNCHECKED) {
                    CheckDlgButton(hWnd, ID_CHECK_DISP_OBJ, BST_CHECKED);
                    selected->hidden = false;
                    DrawGLScene();
                }
                dwPos = SendMessage(checkBox, BM_GETCHECK, 0, 0);
            }
            break;
        case ID_BUTTON_DEL_OBJ:
            if (wmEvent == BN_CLICKED)
            {
                int ItemIndex = SendMessage(objectSelector, (UINT)CB_GETCURSEL,
                    (WPARAM)0, (LPARAM)0);
                if (ItemIndex != CB_ERR){
                    objects.erase(objects.begin() + ItemIndex);
                    objNames.erase(objNames.begin() + ItemIndex);
                    SendMessage(objectSelector, (UINT)CB_DELETESTRING,
                        (WPARAM)ItemIndex, (LPARAM)0);
                }
                int top = SendMessage(objectSelector, (UINT)CB_GETCOUNT, (WPARAM)0, (LPARAM)0);
                if (top != 0){
                    SendMessage(objectSelector, (UINT)CB_SETCURSEL, (WPARAM)top - 1, (LPARAM)0);
                    selected = objects[top - 1];
                    LPCSTR nameStr = objNames[top - 1].c_str();
                    fileToEdit(nameStr);
                }
                else{
                    SendMessage(objectSelector, (UINT)CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
                    selected = NULL;
                    SetDlgItemText(hWnd, ID_EDITOR, TEXT(""));
                    SendMessage(toolBar, TB_ENABLEBUTTON, IDM_SAVE, FALSE);
                }

                ItemIndex = SendMessage(objectSelector, (UINT)CB_GETCURSEL,
                    (WPARAM)0, (LPARAM)0);
                if (ItemIndex == CB_ERR){
                    EnableWindow(deleteButton, FALSE);
                    CheckDlgButton(hWnd, ID_CHECK_DISP_OBJ, BST_INDETERMINATE);
                }
                else{
                    int hid = BST_CHECKED;
                    if (objects[ItemIndex]->hidden == true){
                        hid = BST_UNCHECKED;
                    }
                    else{
                        hid = BST_CHECKED;
                    }
                    CheckDlgButton(hWnd, ID_CHECK_DISP_OBJ, hid);
                }
                DrawGLScene();
            }
            break; 
        case ID_BUTTON_RIGHT:
            if (selected){
                selected->t_x += 0.1f;
                DrawGLScene();
            }
            break;
        case ID_BUTTON_LEFT:
            if (selected){
                selected->t_x -= 0.1f;
                DrawGLScene();
            }
            break;
        case ID_BUTTON_DOWN:
            if (selected){
                selected->t_y -= 0.1f;
                DrawGLScene();
            }
            break;
        case ID_BUTTON_UP:
            if (selected){
                selected->t_y += 0.1f;
                DrawGLScene();
            }
            break;
        case ID_BUTTON_ROTATE_RIGHT:
            if (selected){
                selected->rotation -= 1.0f;
                if (selected->rotation > 359.0f)
                    selected->rotation = 0.0f;
                if (selected->rotation < -1.0f)
                    selected->rotation = 359.0f;
                DrawGLScene();
            }
            break;
        case ID_BUTTON_ROTATE_LEFT:
            if (selected){
                selected->rotation += 1.0f;
                if (selected->rotation > 359.0f)
                    selected->rotation = 0.0f;
                if (selected->rotation < -1.0f)
                    selected->rotation = 359.0f;
                DrawGLScene();
            }
            break;
        case ID_EDITOR:
            switch (wmEvent)
            {
            case EN_MAXTEXT:
                MessageBeep(0);
            default:
                break;
            }
            break;
        case IDM_SAVE:{
            BOOL res = saveObjFile(hWnd, ID_EDITOR);
            if (!res){
                MessageBox(NULL, TEXT("Writting error"),
                    TEXT("Error"), MB_ICONERROR);
            }
            break;
        }
        case IDM_RUN:{
            objects.clear();
            for (std::vector<std::string>::iterator it = objNames.begin(); it != objNames.end(); ++it){
                objects.push_back(new MifObject(*it));
            }
            int ItemIndex = SendMessage(objectSelector, (UINT)CB_GETCOUNT,
                (WPARAM)0, (LPARAM)0);
            if (ItemIndex != 0){
                selected = objects[currentObjIndex];
            }
            else{
                selected = NULL;
            }
            DrawGLScene();
            break;
        }
        case IDM_OPEN:{
            BOOL loaded = openFileIntoEditor(hwndEdit, ID_EDIT);
            SendMessage(toolBar, TB_ENABLEBUTTON, IDM_SAVE, loaded); 
            DrawGLScene();
            break;
        }
        case IDM_about: 
            DialogBox(hInst, MAKEINTRESOURCE(IDD_aboutBOX), mainWindow, about);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_UP:
            objCamera.Move_Camera(CAMERASPEED);
            break;
        case VK_DOWN:
            objCamera.Move_Camera(-CAMERASPEED);
            break;
        case VK_LEFT:
            objCamera.Strafe_Camera(-CAMERASPEED);
            break;
        case VK_RIGHT:
            objCamera.Strafe_Camera(CAMERASPEED);
            break;
        case VK_HOME:
            openFileIntoEditor(hwndEdit, ID_EDIT);
            break; 
        case VK_F1:
            {
                primitive = GL_LINE_LOOP;
                DrawGLScene();
            }
            break;
        case VK_F2:
            {
                primitive = GL_POLYGON;
                DrawGLScene();
                  }
            break;
        case VK_ESCAPE:{
            SetWindowTextA(hwndEdit, "");
            PostQuitMessage(0);
            break;
        }
        default:
            return 0;
        }
        DrawGLScene();
        break;
    case WM_PAINT:
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK viewwndProc(HWND hwnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{  
    switch (message)
    {
    case WM_CREATE: 
        break; 
    case WM_MOUSEMOVE:
        if (wParam & MK_LBUTTON)
        {
            int xPos = LOWORD(lParam);
            int yPos = HIWORD(lParam);
            objCamera.Mouse_Move(width, height);
            DrawGLScene();
        }
        break;

    case WM_LBUTTONDOWN: {
        ShowCursor(FALSE);
        POINT p;
        p.x = LOWORD(lParam);
        p.y = HIWORD(lParam);
        ClientToScreen(hwnd, &p);
        objCamera.centerX = p.x;
        objCamera.centerY = p.y;
        break;
    } 
    case WM_LBUTTONUP:
    case WM_KILLFOCUS:
        ShowCursor(TRUE);
    case WM_ACTIVATE:                              // Watch For Window Activate Message 
    case WM_NCLBUTTONDOWN:
        SetFocus(mainWindow); 
        break;

    case WM_SYSCOMMAND:                            // Intercept System Commands
        {
            switch (wParam)                        // Check System Calls
            {
            case SC_SCREENSAVE:                    // Screensaver Trying To Start?
            case SC_MONITORPOWER:                  //  Monitor Trying To Enter Powersave?
                return 0;                          // Prevent From Happening
            }
            break;                                 // Exit
        } 
    }
    return DefMDIChildProc(hwnd, message, wParam, lParam);
}

// Message handler for about box.
INT_PTR CALLBACK about(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL fileToEdit(LPCSTR fileName){
    HANDLE hFile;
    BOOL bLoaded = false; 
    currentFile = fileName;
    hFile = CreateFile(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    {
        DWORD dwFileSize = GetFileSize(hFile, NULL);
        if (dwFileSize != 0xFFFFFFFF)
        {
            char* pszFileText = (char*)malloc(dwFileSize + 1);

            DWORD dwRead;
            if (ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
            {
                // Add null character to end since ReadFile doesn't do it
                pszFileText[dwFileSize] = '\0';

                // Set editbox text
                SetWindowTextA(hwndEdit, pszFileText);

                bLoaded = TRUE;
            }

            free(pszFileText);
        }
        CloseHandle(hFile);
    }
    else{
        MessageBeep(0);
    }
    return bLoaded;
}

BOOL openFileIntoEditor(HWND hWnd, UINT editID)
{
    // Query user to open file 
    BOOL bLoaded = FALSE;

    TCHAR szFileName[MAX_PATH] = {0}; 
    TCHAR Filestring[MAX_PATH] = { 0 };
    TCHAR FileTitlestring[MAX_PATH] = { 0 };
    OPENFILENAME opf = { 0 };
    opf.lStructSize = sizeof(OPENFILENAME);
    opf.lpstrFile = Filestring;
    opf.lpstrFileTitle = FileTitlestring;
    opf.nMaxFile = MAX_PATH;
    opf.nMaxFileTitle = MAX_PATH;
    opf.lpstrFilter = TEXT("Obj Files (*.obj)\0*.obj\0MifObj Files (*.mifobj)\0*.mifobj\0");
    opf.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
 
    if (GetOpenFileName(&opf))
    {
        std::string fileStr = opf.lpstrFile;
        objects.push_back(new MifObject(fileStr)); 
        std::string fileNameString = opf.lpstrFile; 
        objNames.push_back(fileNameString);
        addItems(opf.lpstrFileTitle);  
        bLoaded = fileToEdit(objNames.back().c_str());
    }
 
    return bLoaded;
}

BOOL saveObjFile(HWND hwnd, UINT editID){
    LPCSTR filePath = currentFile.c_str();
    BOOL bSaved = FALSE;
    HANDLE hFile;

    hFile = CreateFile(filePath, GENERIC_WRITE, CREATE_ALWAYS, NULL, OPEN_EXISTING, 0, NULL);
    if (hFile != INVALID_HANDLE_VALUE)
    { 
        HWND heditor = GetDlgItem(hwnd, editID);
        int textLength = GetWindowTextLength(heditor);
        TCHAR* pstrText = new TCHAR[textLength + 1];
        GetDlgItemText(hwnd, editID, pstrText, textLength+1); 

        DWORD written;
        std::string str = pstrText;
        BOOL result = WriteFile(hFile, str.c_str(), str.length(), &written, NULL);
        int e = GetLastError();
        bSaved = result;
        CloseHandle(hFile);
        delete pstrText;
    }
    else{
        MessageBeep(0);
    } 
    return bSaved;
}

HIMAGELIST g_hImageList = NULL;

HWND createToolbar(HWND hWndParent)
{
    // Declare and initialize local constants.
    const int ImageListID = 0;
    const int numButtons = 3;
    const int bitmapSize = 16;

    const DWORD buttonStyles = 0;

    // Create the toolbar.
    HWND hWndToolbar = CreateWindowEx(0, TOOLBARCLASSNAME, NULL,
        WS_CHILD | TBSTYLE_WRAPABLE, 0, 0, 0, 0,
        hWndParent, NULL, hInst, NULL);

    if (hWndToolbar == NULL)
        return NULL;

    // Create the image list.
    g_hImageList = ImageList_Create(bitmapSize, bitmapSize,   // Dimensions of individual bitmaps.
        ILC_COLOR16 | ILC_MASK,   // Ensures transparent background.
        numButtons, 0);

    // Set the image list.
    SendMessage(hWndToolbar, TB_SETIMAGELIST,
        (WPARAM)ImageListID,
        (LPARAM)g_hImageList);

    // Load the button images.
    SendMessage(hWndToolbar, TB_LOADIMAGES,
        (WPARAM)IDB_STD_SMALL_COLOR,
        (LPARAM)HINST_COMMCTRL);

    // Initialize button info.
    // IDM_NEW, IDM_OPEN, and IDM_SAVE are application-defined command constants.

    TBBUTTON tbButtons[numButtons] =
    {
        { MAKELONG(STD_FILEOPEN, ImageListID), IDM_OPEN, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)"Open" },
        { MAKELONG(STD_FILESAVE, ImageListID), IDM_SAVE, 0, buttonStyles, { 0 }, 0, (INT_PTR)"Save" },
        { MAKELONG(STD_REPLACE, ImageListID), IDM_RUN, TBSTATE_ENABLED, buttonStyles, { 0 }, 0, (INT_PTR)"Run" }
    };

    // Add buttons.
    SendMessage(hWndToolbar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
    SendMessage(hWndToolbar, TB_ADDBUTTONS, (WPARAM)numButtons, (LPARAM)&tbButtons);

    // Resize the toolbar, and then show it.
    SendMessage(hWndToolbar, TB_AUTOSIZE, 0, 0);
    ShowWindow(hWndToolbar, TRUE);

    return hWndToolbar;
}

HWND WINAPI createComboEx(HWND hwndp)
{
    HWND hwndParent = hwndp; // Handle to the parent window

    HWND hWndComboBox = CreateWindow(WC_COMBOBOX, TEXT(""),
        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_VISIBLE | WS_VSCROLL,
        0, 0, 0, 0, hwndParent, NULL, hInst,
        NULL);
     
    return hWndComboBox;
}

BOOL WINAPI addItems(PTCHAR text)
{  
    TCHAR A[16]; 
    memset(&A, 0, sizeof(A)); 
    strcpy(A, (PTCHAR)text);
    SendMessage(objectSelector, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)A); 
    return TRUE;
}

void initButtons(HWND hwnd){
    checkBox = CreateWindow(TEXT("button"), TEXT("Display object"),
        WS_VISIBLE | WS_CHILD | BS_3STATE,
        20, 20, 185, 35,
        hwnd, (HMENU)ID_CHECK_DISP_OBJ, hInst, 0);
    CheckDlgButton(hwnd, ID_CHECK_DISP_OBJ, BST_INDETERMINATE);
    deleteButton = CreateWindow(TEXT("button"), TEXT("Delete object"),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_ICON,
        20, 50, 185, 50,
        hwnd, (HMENU)ID_BUTTON_DEL_OBJ, hInst, 0);
    controlButtons[0] = CreateWindow(TEXT("button"), TEXT(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_BUTTON_RIGHT, hInst, 0);
    controlButtons[1] = CreateWindow(TEXT("button"), TEXT(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_BUTTON_LEFT, hInst, 0);
    controlButtons[2] = CreateWindow(TEXT("button"), TEXT(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_BUTTON_UP, hInst, 0);
    controlButtons[3] = CreateWindow(TEXT("button"), TEXT(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_BUTTON_DOWN, hInst, 0);
    controlButtons[4] = CreateWindow(TEXT("button"), TEXT(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_BUTTON_ROTATE_RIGHT, hInst, 0);
    controlButtons[5] = CreateWindow(TEXT("button"), TEXT(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_BITMAP,
        0, 0, 0, 0,
        hwnd, (HMENU)ID_BUTTON_ROTATE_LEFT, hInst, 0);

    HICON hIcon1 = LoadIcon(NULL, IDI_ERROR);
    EnableWindow(deleteButton, FALSE);
    SendMessage(deleteButton, BM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon1);

    HBITMAP image = (HBITMAP)LoadImage(hInst, TEXT("objects/icons/arrow-right-icon.bmp"), IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendMessage(controlButtons[0], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);

    image = (HBITMAP)LoadImage(hInst, TEXT("objects/icons/arrow-left-icon.bmp"), IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendMessage(controlButtons[1], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);

    image = (HBITMAP)LoadImage(hInst, TEXT("objects/icons/arrow-up-icon.bmp"), IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendMessage(controlButtons[2], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);

    image = (HBITMAP)LoadImage(hInst, TEXT("objects/icons/arrow-down-icon.bmp"), IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendMessage(controlButtons[3], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);

    image = (HBITMAP)LoadImage(hInst, TEXT("objects/icons/arrow-rotate-clockwise-icon.bmp"), IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendMessage(controlButtons[4], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);

    image = (HBITMAP)LoadImage(hInst, TEXT("objects/icons/arrow-rotate-anticlockwise-icon.bmp"), IMAGE_BITMAP, 32, 32, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
    SendMessage(controlButtons[5], BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)image);
}

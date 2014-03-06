#pragma once

#include "resource.h"
#include "camera.h"
#include "MifObject.h" 
using namespace oglut;

#define MAX_LOADSTRING 100
#define ID_EDIT 120
#define ID_VIEW 121
#define ID_EDITOR 122
#define ID_CHECK_DISP_OBJ 123
#define ID_BUTTON_DEL_OBJ 124

#define ID_BUTTON_RIGHT 126
#define ID_BUTTON_LEFT 127
#define ID_BUTTON_UP 128
#define ID_BUTTON_DOWN 129
#define ID_BUTTON_ROTATE_RIGHT 130
#define ID_BUTTON_ROTATE_LEFT 131
HINSTANCE hInst;                                      // current instance
TCHAR szTitle[] = TEXT("obj editor");                 // The title bar text
TCHAR szWindowClass[] = TEXT("mainClass");            // the main window class name
TCHAR szViewWindowClass[] = TEXT("View");
HWND  mainWindow;
HWND hwndView, hwndEdit;
HWND toolBar, objectSelector;
HWND hEdit;
HDC hdc = NULL;
GLuint PixelFormat;            // Holds The Results After Searching For A Match
CCamera objCamera; 
int currentObjIndex;
UINT TextureArray[1];
HWND checkBox, deleteButton;
HWND controlButtons[6];
std::string currentFile;

ATOM                myRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    wndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    about(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    viewwndProc(HWND, UINT, WPARAM, LPARAM);
HWND                createToolbar(HWND hWndParent);
HWND                WINAPI createComboEx(HWND hwndp);
BOOL                WINAPI addItems(PTCHAR text);
GLvoid              reSizeGLScene(GLsizei width, GLsizei height);
BOOL                openFileIntoEditor(HWND hWnd, UINT editID);
BOOL                fileToEdit(LPCSTR fileName);
BOOL                saveObjFile(HWND hwnd, UINT editID);
int width;
int height;

std::vector<Shape*> objects;
Shape* selected; 
int primitive;
std::vector<std::string> objNames;
void initButtons(HWND hwnd); 

#include "utils.cpp"
#include <windows.h>

global_variable bool isRunning = true;

struct Render_State {
    int height, width;
    void* memory;
    BITMAPINFO bitmap_info;
};

global_variable Render_State render_state;
#include "renderer.cpp"

LRESULT CALLBACK window_callback(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam) {
    LRESULT result = 0;
    switch (uMsg) {
        case WM_CLOSE: 
        case WM_DESTROY: {
            isRunning = false;
        } break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);

            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;
            int buffer_size = render_state.width * render_state.height * sizeof(unsigned int);

            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, buffer_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
        } break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }
    return result;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpfnWndProc = window_callback;
    window_class.lpszClassName = CLASS_NAME;

    RegisterClass(&window_class);

    HWND window = CreateWindow(
        window_class.lpszClassName,
        L"First Game",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720,
        nullptr, nullptr, hInstance, nullptr
    );
    HDC hdc = GetDC(window);

    while (isRunning) {
        // Input
        MSG message;
        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
        // Simulate

        clear_screen(0x4D8934);
        draw_rect(0, 0, 1, 1, 0xF6B26B);
        draw_rect(-20, 20, 5, 5, 0xF6B26B);

        //Render
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
    }
    
}

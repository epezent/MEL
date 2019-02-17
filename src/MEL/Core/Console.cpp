#include <MEL/Core/Console.hpp>
#include <MEL/Utility/Mutex.hpp>
#include <csignal>
#include <iostream>
#include <cstdio>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define _NO_OLDNAMES  // for MinGW compatibility
#include <conio.h>    // for getch() and kbhit()
#define getch _getch
#define kbhit _kbhit
#else
#include <termios.h>   // for getch() and kbhit()
#include <unistd.h>    // for getch(), kbhit() and (u)sleep()
#include <sys/ioctl.h> // for getkey()
#include <sys/types.h> // for kbhit()
#include <sys/time.h>  // for kbhit()
#endif

namespace mel {

//==============================================================================
// CONSTANTS
//==============================================================================

namespace {

#ifdef _WIN32
HANDLE stdout_handle = GetStdHandle(static_cast<DWORD>(-11));
CONSOLE_SCREEN_BUFFER_INFO g_csbiInfo;
struct Initializer {
    Initializer() {
        GetConsoleScreenBufferInfo(stdout_handle, &g_csbiInfo);
    }
    ~Initializer() { SetConsoleTextAttribute(stdout_handle, g_csbiInfo.wAttributes); }
};
Initializer g_initializer;
#endif

// mutex to make console output thread safe
static Mutex g_console_mutex;

} // namespace

//==============================================================================
// SIGNAL HANDLING
//==============================================================================

bool (*ctrl_handler)(CtrlEvent);

#ifdef _WIN32
BOOL win_handler(DWORD var) {
    if (var == CTRL_C_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::CtrlC));
    else if (var == CTRL_BREAK_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::CtrlQuit));
    else if (var == CTRL_CLOSE_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::Close));
    else if (var == CTRL_LOGOFF_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::Logoff));
    else if (var == CTRL_SHUTDOWN_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::Shutdown));
    else
        return 0;
}

bool register_ctrl_handler(bool (*handler)(CtrlEvent)) {
    ctrl_handler = handler;
    return !!SetConsoleCtrlHandler((PHANDLER_ROUTINE)win_handler, TRUE);
}
#else

void posix_handler(int sig) {
    if (sig == SIGINT)
        ctrl_handler(CtrlEvent::CtrlC);
    else if (sig == SIGQUIT)
        ctrl_handler(CtrlEvent::CtrlQuit);
}

bool register_ctrl_handler(bool (*handler)(CtrlEvent)) {
    ctrl_handler = handler;
    signal(SIGINT, posix_handler);
    signal(SIGQUIT, posix_handler);
    return true;
}

#endif

//==============================================================================
// CONSOLE FORMAT (THREAD SAFE)
//==============================================================================

#ifdef _WIN32

WORD get_color(Color color, bool background) {
    DWORD val = 0;
    switch(color) {

        case Color::None:
            if (background)
                return (g_csbiInfo.wAttributes & ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
            val = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;

        case Color::Black:
            break;

        case Color::White:
            val = FOREGROUND_INTENSITY;
        case Color::Gray:
            val = val | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case Color::Red:
            val = FOREGROUND_INTENSITY;
        case Color::DarkRed:
            val = val | FOREGROUND_RED;
            break;

        case Color::Green:
            val = FOREGROUND_INTENSITY;
        case Color::DarkGreen:
            val = val | FOREGROUND_GREEN;
            break;

        case Color::Blue:
            val = FOREGROUND_INTENSITY;
        case Color::DarkBlue:
            val = val | FOREGROUND_BLUE;
            break;

        case Color::Cyan:
            val = FOREGROUND_INTENSITY;
        case Color::Aqua:
            val = val | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case Color::Magenta:
            val = FOREGROUND_INTENSITY;
        case Color::Purple:
            val = val | FOREGROUND_RED | FOREGROUND_BLUE;
            break;

        case Color::Yellow:
            val = FOREGROUND_INTENSITY;
        case Color::Gold:
            val = val | FOREGROUND_RED | FOREGROUND_GREEN;
            break;

    }
    if (background)
        return static_cast<WORD>(val * 16);
    else
        return static_cast<WORD>(val);
}

void set_text_color(Color foreground, Color background) {
    WORD attributes = get_color(foreground, false) | get_color(background, true);
    Lock lock(g_console_mutex);
    SetConsoleTextAttribute(stdout_handle, attributes);
}

void reset_text_color() {
    Lock lock(g_console_mutex);
    SetConsoleTextAttribute(stdout_handle, g_csbiInfo.wAttributes);
}

#else

void set_text_color(Color foreground, Color background) {
    Lock lock(g_console_mutex);
    // background
    if (background == Color::None)
        std::cout << "\x1B[0m";
    else if (background == Color::Black)
        std::cout << "\x1B[40m";
    else if (background == Color::Gray)
        std::cout << "\x1B[100m";
    else if (background == Color::White)
        std::cout << "\x1B[107m";
    else if (background == Color::Red)
        std::cout << "\x1B[101m";
    else if (background == Color::DarkRed)
        std::cout << "\x1B[41m";
    else if (background == Color::Green)
        std::cout << "\x1B[102m";
    else if (background == Color::DarkGreen)
        std::cout << "\x1B[42m";
    else if (background == Color::Blue)
        std::cout << "\x1B[104m";
    else if (background == Color::DarkBlue)
        std::cout << "\x1B[44m";
    else if (background == Color::Cyan)
        std::cout << "\x1B[106m";
    else if (background == Color::Aqua)
        std::cout << "\x1B[46m";
    else if (background == Color::Magenta)
        std::cout << "\x1B[105m";
    else if (background == Color::Purple)
        std::cout << "\x1B[45m";
    else if (background == Color::Yellow)
        std::cout << "\x1B[103m";
    else if (background == Color::Gold)
        std::cout << "\x1B[43m";
    // set foreground
    if (foreground == Color::None)
        std::cout << "\x1B[97m";
    else if (foreground == Color::Black)
        std::cout << "\x1B[30m";
    else if (foreground == Color::Gray)
        std::cout << "\x1B[90m";
    else if (foreground == Color::White)
        std::cout << "\x1B[97m";
    else if (foreground == Color::Red)
        std::cout << "\x1B[91m";
    else if (foreground == Color::DarkRed)
        std::cout << "\x1B[31m";
    else if (foreground == Color::Green)
        std::cout << "\x1B[92m";
    else if (foreground == Color::DarkGreen)
        std::cout << "\x1B[32m";
    else if (foreground == Color::Blue)
        std::cout << "\x1B[94m";
    else if (foreground == Color::DarkBlue)
        std::cout << "\x1B[34m";
    else if (foreground == Color::Cyan)
        std::cout << "\x1B[96m";
    else if (foreground == Color::Aqua)
        std::cout << "\x1B[36m";
    else if (foreground == Color::Magenta)
        std::cout << "\x1B[95m";
    else if (foreground == Color::Purple)
        std::cout << "\x1B[35m";
    else if (foreground == Color::Yellow)
        std::cout << "\x1B[93m";
    else if (foreground == Color::Gold)
        std::cout << "\x1B[33m";
}

void reset_text_color() {
    Lock lock(g_console_mutex);
    std::cout << "\x1B[0m\x1B[0K";
}

#endif

//==============================================================================
// CONSOLE INPUT (THREAD SAFE)
//==============================================================================

#ifndef _WIN32
/// Get character without waiting for Return to be pressed.
/// Windows has this in conio.h
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/// Determines if keyboard has been hit.
/// Windows has this in conio.h
int kbhit(void) {
    static struct termios oldt, newt;
    int cnt = 0;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag    &= ~(ICANON | ECHO);
    newt.c_iflag     = 0; // input mode
    newt.c_oflag     = 0; // output mode
    newt.c_cc[VMIN]  = 1; // minimum time to wait
    newt.c_cc[VTIME] = 1; // minimum characters to wait for
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ioctl(0, FIONREAD, &cnt); // Read count
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 100;
    select(STDIN_FILENO+1, NULL, NULL, NULL, &tv); // A small time delay
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return cnt; // Return number of characters
}
#endif

int kb_hit() {
    Lock lock(g_console_mutex);
    return kbhit();
}

int get_ch() {
    Lock lock(g_console_mutex);
    return getch();
}

int get_ch_nb() {
    Lock lock(g_console_mutex);
    if (kbhit())
        return getch();
    else
        return 0;
}

int get_key(void) {
    Lock lock(g_console_mutex);
    #ifndef _WIN32
    int cnt = kbhit(); // for ANSI escapes processing
    #endif
    int k = getch();
    switch(k) {
        case 0: {
            int kk;
            switch (kk = getch()) {
                case 71: return KEY_NUMPAD7;
                case 72: return KEY_NUMPAD8;
                case 73: return KEY_NUMPAD9;
                case 75: return KEY_NUMPAD4;
                case 77: return KEY_NUMPAD6;
                case 79: return KEY_NUMPAD1;
                case 80: return KEY_NUMPAD2;
                case 81: return KEY_NUMPAD3;
                case 82: return KEY_NUMPAD0;
                case 83: return KEY_NUMDEL;
                default: return kk-59+KEY_F1; // Function keys
            }}
        case 224: {
            int kk;
            switch (kk = getch()) {
                case 71: return KEY_HOME;
                case 72: return KEY_UP;
                case 73: return KEY_PGUP;
                case 75: return KEY_LEFT;
                case 77: return KEY_RIGHT;
                case 79: return KEY_END;
                case 80: return KEY_DOWN;
                case 81: return KEY_PGDOWN;
                case 82: return KEY_INSERT;
                case 83: return KEY_DELETE;
                default: return kk-123+KEY_F1; // Function keys
            }}
#ifdef __APPLE__
        case 10:  return KEY_ENTER;
        case 127: return KEY_BACKSPACE;
#else
        case 10: return KEY_ENTER;
        case 8:  return KEY_BACKSPACE;
#endif

#ifdef _WIN32
        case 27: return KEY_ESCAPE;
#else // _WIN32
        case 155: // single-character CSI
        case 27: {
            // Process ANSI escape sequences
            if (cnt >= 3 && getch() == '[') {
                switch (k = getch()) {
                    case 'A': return KEY_UP;
                    case 'B': return KEY_DOWN;
                    case 'C': return KEY_RIGHT;
                    case 'D': return KEY_LEFT;
                }
            } else return KEY_ESCAPE;
        }
#endif // _WIN32
        default: return k;
    }
}

int get_key_nb() {
    if (kbhit())
        return get_key();
    else
        return 0;
}

void prompt(const std::string& message) {
    print_string(message);
    Lock lock(g_console_mutex);
    getchar();
}

//==============================================================================
// CONSOLE OUTPUT
//==============================================================================



#ifdef _WIN32
void print_string(const std::string& str) {
    Lock lock(g_console_mutex);
    WriteConsoleA(stdout_handle, str.c_str(), static_cast<DWORD>(str.size()), NULL, NULL);
}
#else
void print_string(const std::string& str) {
    Lock lock(g_console_mutex);
    std::cout << str;
}
#endif

//==============================================================================
// MISC
//==============================================================================

#ifdef _WIN32
const bool STDOUT_IS_A_TTY = !!_isatty(_fileno(stdout));
#else
const bool STDOUT_IS_A_TTY = !!isatty(fileno(stdout));
#endif

bool is_tty() {
    return STDOUT_IS_A_TTY;
}

void beep() {
#ifdef _WIN32
    Lock lock(g_console_mutex);
    Beep(750, 250);
#endif
}

#define UTIL_PRINT(st) do { std::cout << st; } while(false)
const std::string ANSI_CLS                = "\033[2J\033[3J";
const std::string ANSI_CURSOR_HOME        = "\033[H";

void cls(void) {
    Lock lock(g_console_mutex);
#if defined(_WIN32)
    // Based on https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022%28v=vs.85%29.aspx
    const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    const DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    UTIL_PRINT(ANSI_CLS);
    UTIL_PRINT(ANSI_CURSOR_HOME);
#endif
}

}  // namespace mel

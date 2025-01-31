#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

#define LOG_ERROR(...) Logger::Error(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...) Logger::Info(__FILE__, __FUNCTION__, __LINE__, __VA_ARGS__)


class Logger 
{
private:
   inline static HANDLE hConsole = nullptr;

    enum TextColor {
        RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
        GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
        WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
    };

public:
    static void Init() {
        AllocConsole();
        FILE* fpstdin = stdin;
        FILE* fpstdout = stdout;
        FILE* fpstderr = stderr;
        freopen_s(&fpstdin, "CONIN$", "r", stdin);
        freopen_s(&fpstdout, "CONOUT$", "w", stdout);
        freopen_s(&fpstderr, "CONOUT$", "w", stderr);

        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    static void SetTextColor(TextColor color) {
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
    }

    template<typename... Args>
    static void Error(const char* file, const char* function, int line, const char* format, Args... args) {
        SetTextColor(RED);
        PrintLogMessage("ERROR", file, function, line, format, args...);
        SetTextColor(WHITE);
    }

    template<typename... Args>
    static void Info(const char* file, const char* function, int line, const char* format, Args... args) {
        SetTextColor(GREEN);
        PrintLogMessage("INFO ", file, function, line, format, args...);
        SetTextColor(WHITE);
    }

private:
    template<typename... Args>
    static void PrintLogMessage(const char* level, const char* file, const char* function, int line, const char* format, Args... args) {
        // Get current time
        time_t now = time(nullptr);
        char timeStr[26];
        ctime_s(timeStr, sizeof(timeStr), &now);
        timeStr[24] = '\0'; // Remove newline

        // Format the message
        char message[1024];
        sprintf_s(message, format, args...);
        const char* filename = strrchr(file, '\\');
        filename = filename ? filename + 1 : file; // if no backslash found, use the whole string
      
        printf(" %s : [%s:%s:%d] %s\n",
            level,
            filename,
            function,
            line,
            message);
    }
};


#endif
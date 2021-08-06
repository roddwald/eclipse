#pragma once
#include "sdk.hpp"

#define CHAT_COLOR_DEFAULT "\x01"
#define CHAT_COLOR_RED "\x02"
#define CHAT_COLOR_LIGHTPURPLE "\x03"
#define CHAT_COLOR_GREEN "\x04"
#define CHAT_COLOR_LIME "\x05"
#define CHAT_COLOR_LIGHTGREEN "\x06"
#define CHAT_COLOR_LIGHTRED "\x07"
#define CHAT_COLOR_GRAY "\x08"
#define CHAT_COLOR_LIGHTOLIVE "\x09"
#define CHAT_COLOR_LIGHTSTEELBLUE "\x0A"
#define CHAT_COLOR_LIGHTBLUE "\x0B"
#define CHAT_COLOR_BLUE "\x0C"
#define CHAT_COLOR_PURPLE "\x0D"
#define CHAT_COLOR_PINK "\x0E"
#define CHAT_COLOR_LIGHTRED2 "\x0F"
#define CHAT_COLOR_OLIVE "\x10"

class c_hud_chat {
public:
    enum chat_filters
    {
        CHAT_FILTER_NONE = 0,
        CHAT_FILTER_JOINLEAVE = 0x000001,
        CHAT_FILTER_NAMECHANGE = 0x000002,
        CHAT_FILTER_PUBLICCHAT = 0x000004,
        CHAT_FILTER_SERVERMSG = 0x000008,
        CHAT_FILTER_TEAMCHANGE = 0x000010,
        CHAT_FILTER_ACHIEVEMENT = 0x000020,
    };

    void chat_print(const char* fmt, ...) {
        char msg[1024];
        va_list args;
        va_start(args, fmt);
        vsprintf_s(msg, fmt, args);
        call_virtual<void(__cdecl*)(void*, int, int, const char*, ...)>(this, 27)(this, 0, 0, fmt);
        va_end(args);
    }
};
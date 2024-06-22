#include <stdio.h>

#define SUPPORT_FILEFORMAT_MP3
#define SUPPORT_FILEFORMAT_OGG
#define SUPPORT_FILEFORMAT_WAV
#define SUPPORT_FILEFORMAT_FLAC
#include <raylib.h>

static Music music;

typedef struct {
     short mills;
     short secs;
     short mins;
     int hours;
} Time;

Time convertSecsToTime(double total_seconds) {
    Time time;
    
    int seconds = (int)total_seconds;
    int milliseconds = (int)((total_seconds - seconds) * 1000);

    time.hours = seconds / 3600;
    seconds = seconds % 3600;
    time.mins = seconds / 60;
    time.secs = seconds % 60;
    time.mills = milliseconds;

    return time;
}

const char * getTimeFormat(const Time * time, bool with_mills) {
    if (!with_mills) return TextFormat("%d:%d:%d", time->hours, time->mins, time->secs);
    else             return TextFormat("%d:%d:%d.%d", time->hours, time->mins, time->secs, time->mills);

}

int main(int argc, char * argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage:%s [Audiofile]\n", argv[0]);
        return 1;
    }

    InitAudioDevice();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(1000, 700, "Cauri - Audio Player");
    music = LoadMusicStream(argv[1]);
    if (!IsMusicReady(music)) {
        fprintf(stderr, "Music could not be loaded :(");
        return 1;
    }

    SetMusicVolume(music, 0.2f);
    PlayMusicStream(music);
    
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        BeginDrawing();
            ClearBackground(BLACK);

            Time played = convertSecsToTime(GetMusicTimePlayed(music));
            Time length = convertSecsToTime(GetMusicTimeLength(music));
            DrawText(TextFormat("%s / %s", getTimeFormat(&played, false), getTimeFormat(&length, false)), 25, 25, 23, WHITE);

        EndDrawing();
    }
    CloseWindow();

    return 0;
}

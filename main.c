#include <stdio.h>

#define SUPPORT_FILEFORMAT_MP3
#define SUPPORT_FILEFORMAT_OGG
#define SUPPORT_FILEFORMAT_WAV
#define SUPPORT_FILEFORMAT_FLAC
#include <raylib.h>

#define MUSIC_SHIFT_AMOUNT  5.0f
#define BACKGROUND_COLOR    (Color){ 25, 25, 25, 255 }

static Music music;
static Rectangle bar;

typedef struct {
     short mills;
     short secs;
     short mins;
     int hours;
} Time;

Time convertSecsToTime(int total_seconds) {
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
    if (!with_mills) return TextFormat("%02d:%02d:%02d", time->hours, time->mins, time->secs);
    else             return TextFormat("%02d:%02d:%02d.%03d", time->hours, time->mins, time->secs, time->mills);
    return "";
}

void progress() {
    if (GetMusicTimeLength(music) == 0) return;

    bar.x = 20;
    bar.y = GetScreenHeight() - 20;
    bar.width = GetScreenWidth() - 40;
    bar.height = 5;

    const int prog_x = GetMusicTimePlayed(music) / GetMusicTimeLength(music) * bar.width;
    Rectangle prog_bar = bar;
    prog_bar.width = prog_x;

    DrawRectangleRoundedLines(bar, 0.5, 4, 1, WHITE );
    DrawRectangleRounded(prog_bar, 0.5, 4, WHITE );

    Vector2 circle = {
        bar.x + prog_x,
        bar.y + bar.height / 2
    };

    static bool dragging = false;
    if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) dragging = false;
    if ((CheckCollisionPointRec(GetMousePosition(), bar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || dragging) {
        if (GetMouseX() >= bar.x && GetMouseX() <= bar.x + bar.width) {
            dragging = true;
            circle.x = GetMouseX();
            const float time = (GetMouseX() - bar.x) / bar.width;
            SeekMusicStream(music, time * GetMusicTimeLength(music));
        }
    }
    DrawCircleV(circle, 7, WHITE );

    Time played = convertSecsToTime(GetMusicTimePlayed(music));
    Time length = convertSecsToTime(GetMusicTimeLength(music));
    DrawText(TextFormat("%s / %s", getTimeFormat(&played, false), getTimeFormat(&length, false)), 25, 25, 23, WHITE);

    if (IsKeyPressed(KEY_SPACE)) {
        IsMusicStreamPlaying(music) ? PauseMusicStream(music) : ResumeMusicStream(music);
    }

    if (IsKeyPressed(KEY_LEFT)) {
        float t = (GetMusicTimePlayed(music) - MUSIC_SHIFT_AMOUNT);
        if (t < 0) t = 0.1f;
        SeekMusicStream(music, t);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        float t = (GetMusicTimePlayed(music) + MUSIC_SHIFT_AMOUNT);
        if (t > GetMusicTimeLength(music)) t = GetMusicTimeLength(music);
        SeekMusicStream(music, t);
    }
}

int main(int argc, char * argv[]) {

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_ALWAYS_RUN);
    SetTargetFPS(60);
    InitAudioDevice();
    InitWindow(1000, 700, "Cauri - Audio Player");

    if (argc > 1) {
        music = LoadMusicStream(argv[1]);
    }

    SetMusicVolume(music, 0.25f);
    PlayMusicStream(music);
    SetExitKey(0);
    
    while (!WindowShouldClose()) {
        UpdateMusicStream(music);
        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);
            if (IsMusicReady(music)) progress();

        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseWindow();

    return 0;
}

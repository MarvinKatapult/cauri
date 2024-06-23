
project="cauri"
win_flags="-lwinmm -lopengl32 -lgdi32"
inc_raylib="-Lwin_raylib/ -lraylib -Iwin_raylib"

red_echo() {
    local text=$1
    echo -e "\e[31m$text\e[0m"
}

green_echo() {
    local text=$1
    echo -e "\e[32m$text\e[0m"
}

yellow_echo() {
    local text=$1
    echo -e "\e[33m$text\e[0m"
}

######### Compiling Raylib #########
    cd raylib/src/ 
    make PLATFORM=PLATFORM_DESKTOP -j5
    error_code=$?
    echo -n "Compiling Raylib "
    if [ $error_code -eq 0 ]; then
        green_echo "finished"
    else
        red_echo "failed"
    fi
    cd ../..
####################################

####################################
    echo -n "Compiling all files for project "
    yellow_echo "$project"
######### Add Source Files #########
    x86_64-w64-mingw32-gcc -o $project -Wextra -Wall -pedantic -g -Iinclude \
        main.c \
        $inc_raylib $win_flags -lm -static \
###################################
    error_code=$?
    echo -n "Compiling "
    if [ $error_code -eq 0 ]; then
        green_echo "finished" 
    else
        red_echo "failed" 
    fi

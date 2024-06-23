
project="cauri"
inc_raylib="-Lraylib/src/ -lraylib -Iraylib/src"

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
    cc -o $project -Wextra -Wall -pedantic -g -Iinclude \
        main.c \
        $inc_raylib -lm \
###################################
    error_code=$?
    echo -n "Compiling "
    if [ $error_code -eq 0 ]; then
        green_echo "finished" 
    else
        red_echo "failed" 
    fi

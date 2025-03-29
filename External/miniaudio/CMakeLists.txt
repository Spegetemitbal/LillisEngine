add_library(miniaudio
        "include/miniaudio.h"
        "src/miniaudio.c"
)

target_include_directories(miniaudio PUBLIC "include/")
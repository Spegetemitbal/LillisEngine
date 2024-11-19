CPMAddPackage(
        NAME stb
        GITHUB_REPOSITORY nothings/stb
        DOWNLOAD_ONLY TRUE
        # branch: master
        GIT_TAG master
)

include_directories(${stb_SOURCE_DIR})
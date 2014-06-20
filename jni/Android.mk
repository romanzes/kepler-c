LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := keplerc

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL

LOCAL_SRC_FILES := \
    app.cpp \
    app-android.cpp \
    textures.cpp \
    game.cpp \
    gameover.cpp \
    importgl.cpp \
    input.cpp \
    mainmenu.cpp \
    polygon.cpp \
    util.cpp \
    vector2.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := keplerc

LOCAL_CFLAGS := -DANDROID_NDK \
                -DDISABLE_IMPORTGL

LOCAL_SRC_FILES := \
	app.cpp \
    app-android.cpp \
    common.cpp \
    gameover.cpp \
    gamescreen.cpp \
    importgl.cpp \
    input.cpp \
    mainmenu.cpp \
    polygon.cpp \
    textures.cpp \
    util.cpp \
    vector2.cpp \
    game/asteroid.cpp \
    game/bullet.cpp \
    game/game.cpp \
    game/logic.cpp \
    game/render.cpp \
    game/ship.cpp \

LOCAL_LDLIBS := -lGLESv1_CM -ldl -llog

include $(BUILD_SHARED_LIBRARY)

########################
#
# Main Application Entry
#
########################
LOCAL_PATH := $(call my-dir)
CUGL_PATH  := $(LOCAL_PATH)/../../../../../../../cugl
PROJ_PATH  := $(LOCAL_PATH)/../../../../../..
include $(CLEAR_VARS)

LOCAL_MODULE := main
LOCAL_C_INCLUDES := $(CUGL_PATH)/include
LOCAL_C_INCLUDES += $(CUGL_PATH)/sdl2/include
LOCAL_C_INCLUDES += $(CUGL_PATH)/thirdparty/box2d/include
LOCAL_C_INCLUDES += $(CUGL_PATH)/thirdparty/geomtools/include
LOCAL_C_INCLUDES += $(PROJ_PATH)/source
LOCAL_C_INCLUDES += $(PROJ_PATH)/source
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Input
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/MVC/Tilemap
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/MVC/Tile
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Game
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/CMV/Tile
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Generator
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/CMV/Tilemap


# Add your application source files here.
LOCAL_PATH = $(PROJ_PATH)
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, \
	$(LOCAL_PATH)/source/TMApp.cpp \
	$(LOCAL_PATH)/source/main.cpp \
	$(LOCAL_PATH)/source/CMV/Tilemap/CMVTilemapController.cpp \
	$(LOCAL_PATH)/source/MVC/Tilemap/MVCTilemapController.cpp \
	$(LOCAL_PATH)/source/Game/GameController.cpp \
	$(LOCAL_PATH)/source/Input/InputController.cpp \
	$(LOCAL_PATH)/source/Generator/GeneratorController.cpp)

# Link in SDL2
LOCAL_SHARED_LIBRARIES := SDL2
LOCAL_SHARED_LIBRARIES += SDL2_image
LOCAL_SHARED_LIBRARIES += SDL2_ttf
LOCAL_SHARED_LIBRARIES += SDL2_codec

LOCAL_STATIC_LIBRARIES := CUGL
LOCAL_STATIC_LIBRARIES += box2d
LOCAL_STATIC_LIBRARIES += poly2tri
LOCAL_STATIC_LIBRARIES += polyclipping

LOCAL_LDLIBS := -lGLESv1_CM -lGLESv2 -lGLESv3 -lOpenSLES -llog -landroid
include $(BUILD_SHARED_LIBRARY)

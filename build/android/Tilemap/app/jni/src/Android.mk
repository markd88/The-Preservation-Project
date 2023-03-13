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
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/GuardSet
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Camera
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/GuardSet/Guard
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Input
<<<<<<< HEAD
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Scene
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Path
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Tile
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/ArtifactSet
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/ArtifactSet/Artifact
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Tilemap
LOCAL_C_INCLUDES += $(PROJ_PATH)/source
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Character
=======
LOCAL_C_INCLUDES += $(PROJ_PATH)/source
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/GuardSet/Guard
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Character
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Tilemap
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Camera
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Tile
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/ArtifactSet
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Path
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/Scene
LOCAL_C_INCLUDES += $(PROJ_PATH)/source/ArtifactSet/Artifact
>>>>>>> c6c8a38e6884f17031f96d0f527132e81ebad643


# Add your application source files here.
LOCAL_PATH = $(PROJ_PATH)
LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, \
	$(LOCAL_PATH)/source/App.cpp \
	$(LOCAL_PATH)/source/main.cpp \
	$(LOCAL_PATH)/source/Tilemap/TilemapController.cpp \
	$(LOCAL_PATH)/source/Scene/GamePlayController.cpp \
	$(LOCAL_PATH)/source/Scene/LoadingController.cpp \
	$(LOCAL_PATH)/source/Input/InputController.cpp \
	$(LOCAL_PATH)/source/Path/PathController.cpp \
	$(LOCAL_PATH)/source/Camera/CameraAction.cpp \
	$(LOCAL_PATH)/source/Camera/CameraManager.cpp \
	$(LOCAL_PATH)/source/Camera/CameraMove.cpp)

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

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../cocos2d)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/external)
$(call import-add-path,$(LOCAL_PATH)/../../cocos2d/cocos)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AddBarrier.cpp \
				   ../../Classes/AdventureScene.cpp \
				   ../../Classes/AppDelegate.cpp \
				   ../../Classes/Ball.cpp \
				   ../../Classes/Bar.cpp \
				   ../../Classes/Barrier.cpp \
				   ../../Classes/HelloScene.cpp \
				   ../../Classes/PassDifficultyScene.cpp \
				   ../../Classes/Star.cpp \
				   ../../Classes/Vibrator.cpp \
				   ../../Classes/ChuangGuan.cpp \
				   ../../Classes/GateMapLayer.cpp \
				   ../../Classes/SelectGate.cpp	\
				   ../../Classes/MainMenuScene.cpp	\
				   ../../Classes/ListScene.cpp	\
				   ../../Classes/Final1Scene.cpp \
				   ../../Classes/Final2Scene.cpp \
				   ../../Classes/Final3Scene.cpp \
				   ../../Classes/FinalThrough1Scene.cpp \
				   ../../Classes/FinalThrough2Scene.cpp \
				   ../../Classes/FinalThrough3Scene.cpp \
				   ../../Classes/FinalThrough4Scene.cpp \
				   ../../Classes/FinalThrough5Scene.cpp \
				   ../../Classes/GameOver1Scene.cpp \
				   ../../Classes/GameOver2Scene.cpp \
				   ../../Classes/GameOver3Scene.cpp \
				   ../../Classes/GameOver4Scene.cpp \
				   ../../Classes/GameOver5Scene.cpp \
				   ../../Classes/PassAllGateScene.cpp
				   

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

# _COCOS_HEADER_ANDROID_BEGIN
# _COCOS_HEADER_ANDROID_END


LOCAL_STATIC_LIBRARIES := cocos2dx_static

# _COCOS_LIB_ANDROID_BEGIN
# _COCOS_LIB_ANDROID_END

include $(BUILD_SHARED_LIBRARY)

$(call import-module,.)

# _COCOS_LIB_IMPORT_ANDROID_BEGIN
# _COCOS_LIB_IMPORT_ANDROID_END

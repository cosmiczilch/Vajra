/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// OpenGL ES 2.0 code

#include <jni.h>
#include <android/log.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include "Libraries/glm/glm.hpp"
#include "Libraries/glm/gtx/transform.hpp"
#include "Libraries/glm/gtc/type_ptr.hpp"

#include "Vajra/Engine/Core/Engine.h"
#include "Vajra/Engine/GameObject/GameObject.h"
#include "Vajra/Engine/SceneGraph/SceneGraph.h"
#include "Vajra/Framework/Core/Framework.h"
#include "Vajra/Framework/Logging/Logger.h"
#include "Vajra/Placeholder/Renderer/Renderer.h"
#include "Vajra/Placeholder/Tesserakonteres.h"
#include "Vajra/Utilities/VajraToAndroidJavaInterface/VajraToAndroidJavaInterface.h"
#include "local_test.h"

// #include "Libraries/libpng-android-master/jni/png.h"
#include "png.h"

jobject gInterfaceObject;


void testCallIntoJava() {
    jclass clazz = VajraToAndroidJavaInterface::GetJNIEnv()->FindClass("com/vajra/androidwrapper/VajraAndroidWrapper");
    jmethodID methodID = VajraToAndroidJavaInterface::GetJNIEnv()->GetStaticMethodID(clazz, "TestMethodInJava", "()V");
    VajraToAndroidJavaInterface::GetJNIEnv()->CallStaticVoidMethod(clazz, methodID);
}

void copyAssetsToSDCard() {
    jclass clazz = VajraToAndroidJavaInterface::GetJNIEnv()->FindClass("com/vajra/androidwrapper/VajraAndroidWrapper");
    jmethodID methodID = VajraToAndroidJavaInterface::GetJNIEnv()->GetStaticMethodID(clazz, "CopyAssetsToSDCard", "()V");
    VajraToAndroidJavaInterface::GetJNIEnv()->CallStaticVoidMethod(clazz, methodID);
}

char* loadAsset(const char* path)
{
    jstring _path = VajraToAndroidJavaInterface::GetJNIEnv()->NewStringUTF(path);
    jclass clazz = VajraToAndroidJavaInterface::GetJNIEnv()->GetObjectClass(gInterfaceObject);
    jmethodID staticMethodId = VajraToAndroidJavaInterface::GetJNIEnv()->GetStaticMethodID(clazz, "getAssetContents", "(Ljava/lang/String;)[B");
    if (staticMethodId == 0) {
        return 0;
    }
    jbyteArray result = (jbyteArray) VajraToAndroidJavaInterface::GetJNIEnv()->CallStaticObjectMethod(clazz, staticMethodId, _path);
    jsize size = VajraToAndroidJavaInterface::GetJNIEnv()->GetArrayLength(result);
    char* data = (char*)calloc((size_t)size, sizeof(char));
    VajraToAndroidJavaInterface::GetJNIEnv()->GetByteArrayRegion(result, (jsize)0, size, (jbyte*)data);
    return data; //data is returned as shader source
}


extern "C" {
    JNIEXPORT void JNICALL Java_com_vajra_androidwrapper_VajraAndroidWrapper_init(JNIEnv * env, jobject obj,  jint width, jint height);
    JNIEXPORT void JNICALL Java_com_vajra_androidwrapper_VajraAndroidWrapper_step(JNIEnv * env, jobject obj, float dt);
};

JNIEXPORT void JNICALL Java_com_vajra_androidwrapper_VajraAndroidWrapper_init(JNIEnv * env, jobject obj,  jint width, jint height)
{
    png_structp png_ptr;
    png_infop info_ptr;
    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);


    gInterfaceObject = reinterpret_cast<jobject>(env->NewGlobalRef(obj));

    VajraToAndroidJavaInterface::SetJNIEnv(env);

    // Copy all assets from the apk to the sdcard:
    copyAssetsToSDCard();

    int number = testEngineFunction();
    FRAMEWORK->GetLogger()->dbglog("test number from test: %d", number);
    number = localTestFunction();
    FRAMEWORK->GetLogger()->dbglog("test number from test: %d", number);

    // loadAsset("simple_vshader");
    testCallIntoJava();

    // TODO [Implement] send the correct DPI
    setupGraphics(width, height, 0);


    Tesserakonteres::initGameObjectsForScene();
}

JNIEXPORT void JNICALL Java_com_vajra_androidwrapper_VajraAndroidWrapper_step(JNIEnv * env, jobject obj, float dt)
{
    renderFrame();
}


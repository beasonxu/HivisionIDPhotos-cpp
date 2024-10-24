#include <jni.h>
#include <string>
#include "HIvision_SDK.h"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
extern "C" JNIEXPORT jstring JNICALL
Java_com_hivisionidphotos_app_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    return env->NewStringUTF(hello.c_str());
}




extern "C"
JNIEXPORT jstring JNICALL
Java_com_hivisionidphotos_app_MainActivity_humanMatch(JNIEnv *env, jobject jobject , jstring imagePath, jstring modelPath, jstring outPath) {
    const char* str1;
    str1 = env->GetStringUTFChars(imagePath, 0);
    std::string ImagePathStr(str1);
    const char* ImagePath = ImagePathStr.c_str();

    const char* str2;
    str2 = env->GetStringUTFChars(modelPath, 0);
    std::string ModelPathStr(str2);
    const char* ModelPath = ModelPathStr.c_str();

    const char* str3;
    str3 = env->GetStringUTFChars(outPath, 0);
    std::string OutPathStr(str3);


    cv::Mat image =cv::imread(ImagePath, cv::IMREAD_COLOR);
    cv::Mat bgra_img=human_matting(ModelPath,image,4);
    std::string outStr = OutPathStr+"human_matting.png";
    cv::imwrite(outStr, bgra_img);
    env->ReleaseStringUTFChars(imagePath, str1);
    env->ReleaseStringUTFChars(modelPath, str2);
    env->ReleaseStringUTFChars(outPath, str3);

    return env->NewStringUTF(outStr.c_str());
}
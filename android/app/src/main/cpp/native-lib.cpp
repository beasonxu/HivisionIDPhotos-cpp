#include <jni.h>
#include <string>
#include "opencv2/core/mat.hpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_hivisionidphotos_app_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";


    cv::Mat bgra_img=human_matting(modelPathCStr, image,p.get<int>("thread_num"));
    return env->NewStringUTF(hello.c_str());
}
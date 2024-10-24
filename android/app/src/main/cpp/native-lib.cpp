#include <jni.h>
#include <string>
#include "HIvision_SDK.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_hivisionidphotos_app_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";

    Hivision_config  hivision;
    std::string ImagePathStr(hivision.image_path);
    std::string ModelPathStr(hivision.model_path);
    std::string OutPathStr(hivision.out_path);

    const char* ImagePath = ImagePathStr.c_str();
    const char* ModelPath = ModelPathStr.c_str();


    cv::Mat image =cv::imread(ImagePath, cv::IMREAD_COLOR);
    cv::Mat bgra_img=human_matting(ModelPath,image,4);
    cv::imwrite(OutPathStr+"human_matting.png", bgra_img);



    return env->NewStringUTF(hello.c_str());
}


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

extern "C"
JNIEXPORT jint JNICALL
Java_com_hivisionidphotos_app_MainActivity_generationPic(JNIEnv *env, jobject jobject,jstring j_input_image,
    jstring j_output_image,jstring j_model_path,jstring j_segment_model,jint j_out_size_kb,int j_thread_num,
    jint j_background_color_r,jint j_background_color_g,jint j_background_color_b,
    jint j_out_images_width,jint j_out_images_height,jfloat j_head_measure_ratio,
    jint j_face_model,jboolean j_layout_photos){
    const char* char_input_image;
    char_input_image = env->GetStringUTFChars(j_input_image, 0);
    std::string input_image(char_input_image);

    const char* char_output_image;
    char_output_image = env->GetStringUTFChars(j_output_image, 0);
    std::string output_image(char_output_image);

    const char* char_model_path;
    char_model_path = env->GetStringUTFChars(j_output_image, 0);
    std::string model_path(char_model_path);

    const char* char_segment_model;
    char_segment_model = env->GetStringUTFChars(j_segment_model, 0);
    std::string segment_model(char_segment_model);
    int out_size_kb = j_out_size_kb;
    int thread_num =j_thread_num;
    int background_color_r = j_background_color_r;
    int background_color_g = j_background_color_g;
    int background_color_b = j_background_color_b;
    int out_images_width = j_out_images_width;
    int out_images_height = j_out_images_height;
    float head_measure_ratio = j_head_measure_ratio;
    int face_model = j_face_model;
    bool layout_photos = j_layout_photos;



    env->ReleaseStringUTFChars(j_input_image, char_input_image);
    env->ReleaseStringUTFChars(j_output_image, char_output_image);
    env->ReleaseStringUTFChars(j_model_path, char_model_path);
    env->ReleaseStringUTFChars(j_segment_model, char_segment_model);



    matting_params human_matting_params;
    if(head_measure_ratio>1||head_measure_ratio<0){
        printf("option value is invalid: --head_measure_ratio %f",head_measure_ratio);
        return -1;
    }

#include <chrono>

    const char* modelPathCStr = segment_model.c_str();
    cv::Vec3b newBackgroundColor(background_color_b, background_color_g, background_color_r);
    auto start_face_1 = std::chrono::high_resolution_clock::now();
    LFFD* face_detector = new LFFD(model_path, face_model, thread_num);
    auto end_face1 = std::chrono::high_resolution_clock::now();
    auto duration_face1= std::chrono::duration_cast<std::chrono::milliseconds>(end_face1 - start_face_1);

    cv::Mat image = cv::imread(input_image, cv::IMREAD_COLOR);

    auto start = std::chrono::high_resolution_clock::now();
    cv::Mat bgra_img=human_matting(modelPathCStr, image,thread_num);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);


    cv::Mat add_background_img = addBackground(bgra_img, newBackgroundColor);
    cv::cvtColor(add_background_img, add_background_img, cv::COLOR_BGRA2BGR);

    std::vector<FaceInfo > finalBox;
    auto start_face = std::chrono::high_resolution_clock::now();
    face_detector->detect(add_background_img, finalBox, add_background_img.rows, add_background_img.cols);
    auto end_face = std::chrono::high_resolution_clock::now();
    auto duration_face = std::chrono::duration_cast<std::chrono::milliseconds>(end_face - start_face);
    std::cout << " c++人脸检测耗时 " << duration_face.count() + duration_face1.count()-50 << "ms." << std::endl;
    if (finalBox.size() > 1) {
        printf("输入人脸不为 1");
        return -2;
    }else if (finalBox.empty()){
        printf("检测不到人脸");
        return -3;
    }else {
        human_matting_params.face_info = finalBox[0];
    }
    free(face_detector);
    auto start_photo = std::chrono::high_resolution_clock::now();
    cv::Mat hd_result = photo_adjust(human_matting_params, add_background_img, out_images_height,out_images_width,background_color_r,background_color_g,background_color_b, head_measure_ratio);
    if(out_size_kb>0){
        resizeImageToKB(hd_result,output_image+"result_kb.png",out_size_kb);
    }
    cv::Mat standard_result;
    cv::Size standard_size(out_images_width, out_images_height);
    cv::resize(hd_result, standard_result, standard_size);
    cv::imwrite(output_image +"result_hd.png", hd_result);
    cv::imwrite(output_image+"result_standard.png", standard_result);
    if(layout_photos){
        auto result_typography_arr =generate_layout_photo(out_images_height,out_images_width);
        cv::Mat result_layout_image = generate_layout_image(
                standard_result,
                std::get<0>(result_typography_arr),
                std::get<1>(result_typography_arr),
                out_images_width,
                out_images_height
        );
        cv::imwrite( output_image+"layout_photo.png", result_layout_image);
    }

    return 0;
}
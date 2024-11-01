package com.hivisionidphotos.app

import android.content.res.AssetManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.os.Environment.DIRECTORY_DOWNLOADS
import android.util.Log
import com.hivisionidphotos.app.FileUtils.copyAllAssetsToExternalStorage
import com.hivisionidphotos.app.databinding.ActivityMainBinding
import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.GlobalScope
import kotlinx.coroutines.launch

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    @OptIn(DelicateCoroutinesApi::class)
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = stringFromJNI()

        binding.btn1.setOnClickListener {
            GlobalScope.launch(Dispatchers.IO) {
//                copyAllAssetsToExternalStorage(this@MainActivity)
                val dir = this@MainActivity.getExternalFilesDir(DIRECTORY_DOWNLOADS)?.absolutePath+"/cache"
                val imagePath = "$dir/test.jpg"
                val modelPath = "$dir/model.onnx"
                val outPath = dir

                val result = humanMatch(imagePath,modelPath,dir)
                Log.d("result", result.toString())
            }

        }
    }




    /**
     * A native method that is implemented by the 'app' native library,
     * which is packaged with this application.
     */
    private external fun stringFromJNI(): String
    external fun humanMatch(imagePath:String,modelPath:String,outPath:String):String
    external fun generationPic(input_image:String,
                               output_image:String,model_path:String,segment_model:String,out_size_kb:Int,thread_num:Int,
                               background_color_r:Int,background_color_g:Int,background_color_b:Int,
                               out_images_width:Int,out_images_height:Int,head_measure_ratio:Float,
                               face_model:Int,layout_photos:Boolean):Int

    companion object {
        // Used to load the 'app' library on application startup.
        init {
            System.loadLibrary("app")
        }
    }
}
package com.hivisionidphotos.app

import android.content.Context
import android.content.res.AssetManager
import android.os.Environment.DIRECTORY_DOWNLOADS
import android.util.Log
import java.io.File
import java.io.FileOutputStream
import java.io.IOException
import java.io.InputStream
import java.io.OutputStream

object FileUtils {
    fun copyAllAssetsToExternalStorage(context: Context) {
        val assetManager = context.assets
        val assetList = assetManager.list("") ?: return

        val externalStorageDir = File(context.getExternalFilesDir(DIRECTORY_DOWNLOADS)?.absolutePath+"/cache")

        if (!externalStorageDir.exists()) {
            externalStorageDir.mkdirs()
        }

        assetList.forEach { asset ->
            copyAsset(assetManager, asset, File(externalStorageDir, asset))
        }
    }

    private fun copyAsset(assetManager: AssetManager, assetName: String, destinationFile: File) {
        try {
            assetManager.open(assetName).use { inputStream ->
                FileOutputStream(destinationFile).use { outputStream ->
                    copyStream(inputStream, outputStream)
                }
            }
        } catch (e: IOException) {
            Log.e("FileUtils", "Error copying asset: $assetName", e)
        }
    }

    private fun copyStream(inputStream: InputStream, outputStream: OutputStream) {
        val buffer = ByteArray(1024)
        var length: Int
        while (inputStream.read(buffer).also { length = it } > 0) {
            outputStream.write(buffer, 0, length)
        }
    }
}
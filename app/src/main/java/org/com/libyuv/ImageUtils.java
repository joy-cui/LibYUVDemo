package org.com.libyuv;

import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Created by cui.li on 2016/12/30.
 */

public class ImageUtils {
    public static final int MEDIA_TYPE_IMAGE = 1;
    public static final int MEDIA_TYPE_VIDEO = 2;

    // save image to sdcard path: Pictures/MyTestImage/
    public static void saveImageData(byte[] imageData,boolean isSrc) {
        File imageFile = getOutputMediaFile(MEDIA_TYPE_VIDEO, isSrc);

        if (imageFile == null) return;
        Log.e("", "onPreviewFrame......imageFileDir: " + imageFile.getAbsolutePath());
        try {

            FileOutputStream fos = new FileOutputStream(imageFile);
            fos.write(imageData);
            fos.close();
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static File getOutputMediaFile(int type,boolean isSrc) {
//        Log.e("","onPreviewFrame.................getOutputMediaFile");
//        File imageFileDir =
//                new File(Environment.getExternalStoragePublicDirectory(Environment.DIRECTORY_PICTURES), "MyTestImage");

//        File file = new File("mnt/sdcard/test.jpg");

        File imageFileDir =
                new File("/mnt/sdcard/test/", "yuv");

        if (!imageFileDir.exists()) {
            if (!imageFileDir.mkdirs()) {
                return null;
            }
        }
//        // Create a media file name
        String timeStamp = new SimpleDateFormat("yyyyMMdd_HHmmss").format(new Date());
        File imageFile;
        if (type == MEDIA_TYPE_IMAGE) {
            imageFile = new File(imageFileDir.getPath() + File.separator +
                    "IMG_" + timeStamp + ".jpg");
        } else if (type == MEDIA_TYPE_VIDEO) {
            if(isSrc){
                imageFile = new File(imageFileDir.getPath() + File.separator +
                        "VID_src" +  ".yuv");
            }else {
                imageFile = new File(imageFileDir.getPath() + File.separator +
                        "VID_dest" +  ".yuv");
            }
        } else return null;
        return imageFile;
    }

}

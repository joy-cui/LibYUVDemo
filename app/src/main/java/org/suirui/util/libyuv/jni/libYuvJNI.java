package org.suirui.util.libyuv.jni;

import android.util.Log;

import org.suirui.util.libyuv.jni.entry.YUV;

/**
 * Created by cui.li on 2017/1/3.
 */

public class libYuvJNI {
    private static libYuvJNI intance;

    public libYuvJNI() {
        loadlibrary();
    }

    public synchronized static libYuvJNI getInstance() {

        if (intance == null) {
            intance = new libYuvJNI();
        }
        return intance;

    }

    private void loadlibrary() {
        Log.e("", "signal....loadlibrary");
        String[] LIBS = new String[]{"yuv", "yuvUtil"};
        for (int i = 0; i < LIBS.length; i++) {
            try {
                System.loadLibrary(LIBS[i]);
            } catch (Exception e) {
                e.printStackTrace();

            }
        }
    }


    /**
     * @param buf
     * @param w
     * @param h
     * @param rotate 旋转度数
     * @return
     */
    public static native byte[] nv21To420(byte[] buf, int w, int h, int rotate, boolean isMirror, boolean isRotate);

    /**
     * yuv转yuv分量
     *
     * @param buf
     * @param w
     * @param h
     * @return
     */
    public static native YUV yuv420ToYUV(byte[] buf, int w, int h);


    //NV21转I420并旋转
    public static native byte[] NV21ToI420Rotate(byte[] buf, int w, int h, int rotate);

    //NV21转I420并旋转+镜像
    public static native byte[] NV21ToI420RotateMirror(byte[] buf, int w, int h, int rotate);
}

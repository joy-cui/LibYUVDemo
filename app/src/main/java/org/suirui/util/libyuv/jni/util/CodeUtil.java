package org.suirui.util.libyuv.jni.util;

import org.suirui.util.libyuv.jni.entry.YUV;
import org.suirui.util.libyuv.jni.libYuvJNI;

/**
 * Created by cui.li on 2017/1/5.
 */

public class CodeUtil {
    private static CodeUtil intance;

    public CodeUtil() {
        libYuvJNI.getInstance();
    }

    public synchronized static CodeUtil getInstance() {

        if (intance == null) {
            intance = new CodeUtil();
        }
        return intance;

    }

    /**
     * n21转420 镜像处理 旋转角度
     *
     * @param buf
     * @param w
     * @param h
     * @param rotate
     * @param isMirror
     * @param isRotate
     * @return
     */
    public byte[] nv21To420(byte[] buf, int w, int h, int rotate, boolean isMirror, boolean isRotate) {
        return libYuvJNI.nv21To420(buf, w, h, rotate, isMirror, isRotate);
    }

    public YUV yuv420ToYUV(byte[] buf, int w, int h) {
        return libYuvJNI.yuv420ToYUV(buf, w, h);
    }


}

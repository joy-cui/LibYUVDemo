package org.suirui.util.libyuv.jni;

import android.util.Log;

/**
 * Created by cui.li on 2017/1/3.
 */

public class JniCallBack {
    private static JniCallBack callBack = null;

    public JniCallBack() {
    }

    public synchronized static JniCallBack getInstance() {

        if (callBack == null) {
            callBack = new JniCallBack();
        }
        return callBack;

    }

    void onLibYUVCallBack(byte[] des, int width, int height) {
        Log.e("", "VideoStreamProcess......JniCallBack...width:" + width + " height:" + height);

    }
}

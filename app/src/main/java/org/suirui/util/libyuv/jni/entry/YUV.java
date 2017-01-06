package org.suirui.util.libyuv.jni.entry;

/**
 * Created by cui.li on 2017/1/5.
 *
 */

public class YUV {
    private byte[] y;
    private byte[] u;
    private byte[] v;

    public YUV() {
    }

    public YUV(byte[] y, byte[] u, byte[] v) {
        this.y = y;
        this.u = u;
        this.v = v;
    }

    public byte[] getY() {
        return y;
    }

    public void setY(byte[] y) {
        this.y = y;
    }

    public byte[] getU() {
        return u;
    }

    public void setU(byte[] u) {
        this.u = u;
    }

    public byte[] getV() {
        return v;
    }

    public void setV(byte[] v) {
        this.v = v;
    }
}

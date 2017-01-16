# LIBYUV
---------------------------------
1.library 模块主要是打包libyuv.so文件

2.app模块通过camera采集的数据libyuv中的转码，旋转方法
  1>将libyuv.so文件拷贝到app模块jni/prebuilt文件夹下重新打包
  2>编写JNI方法，调用libyuv.so中的方法
#include <jni.h>

Java_com_example_millawang_myapplication_MainActivity_StringFromJNI(JNIEnv *env, jstring str)
{
    return (*env)->NewStringUTF(env, "Hello From JNI");
}
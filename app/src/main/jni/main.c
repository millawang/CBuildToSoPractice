#include <jni.h>

Java_com_example_millawang_myapplication_MainActivity_StringFromJNI(JNIEnv *env, jstring javaString)
{
    ///const char *nativeString = (*env)->GetStringUTFChars(env, javaString, 0);
    //(*env)->ReleaseStringUTFChars(env, javaString, nativeString);

    char *str= "fffff";

    return (*env)->NewStringUTF(env, str);
}
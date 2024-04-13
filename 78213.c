base::FilePath GetTestFilesDirectory(JNIEnv* env) {
  ScopedJavaLocalRef<jstring> directory =
      content::Java_ShellLayoutTestUtils_getApplicationFilesDirectory(
          env, base::android::GetApplicationContext());
  return base::FilePath(ConvertJavaStringToUTF8(directory));
}

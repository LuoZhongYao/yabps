all :
	ndk-build
	adb push ./obj/local/armeabi/yabps /data/user

clean :
	ndk-build clean

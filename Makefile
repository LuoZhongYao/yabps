all :
	ndk-build
	adb push ./obj/local/armeabi/yabps /data/user

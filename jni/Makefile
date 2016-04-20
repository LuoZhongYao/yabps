all :
	ndk-build
	adb push ../obj/local/armeabi/yabps /system/bin/
	adb shell killall yabps
	adb shell "yabps /dev/ttyS3&"

clean :
	ndk-build clean

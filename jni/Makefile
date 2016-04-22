all :
	ndk-build
	adb -s 20080411 push ../obj/local/armeabi/yabps /system/bin/
	adb -s 20080411 shell killall yabps
	adb -s 20080411 shell "yabps /dev/ttyS3&"

clean :
	ndk-build clean

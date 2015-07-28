all :
	ndk-build
	adb push ./obj/local/armeabi/yabps /system/bin/
	adb shell busybox killall yabps
	adb shell "yabps /dev/ttyS3&"

clean :
	ndk-build clean

wire :
	adb pull /data/user/hci.hex

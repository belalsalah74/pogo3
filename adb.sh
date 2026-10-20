unzip -l ./Pokemon_GO.apk | grep -E "libmain|libpgo"
unzip -p ./Pokemon_GO.apk lib/arm64-v8a/libmain.so | strings | grep -E "PGOHOOK|libmain_orig|libpgo_hook|entered"
unzip -p ./Pokemon_GO.apk lib/arm64-v8a/libpgo_hook.so | strings | grep -E "constructor running|curve setter"
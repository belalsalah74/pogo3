# 🧩 Pokémon GO Patches for Morphe

Morphe patches for Pokémon GO.

## ❓ About

Patches for Pokémon GO (arm64-v8a, builds 0.433.1 and 0.431.0):

- **Google web sign-in:** replaces Google Play Services sign-in with a browser-based Google
  login, so Google accounts work in re-signed builds.
- **Enhanced throw:** experimental native hook, installed by a small `libmain.so` wrapper.
- **Spoof signature (ROM support):** only useful on ROMs or microG setups that support
  signature spoofing.

Patch the `.apkm` (Morphe merges the splits) or a merged APK, then uninstall the Play Store
version before installing, since the signature differs.

### How to use these patches

Click here to add these patches to Morphe: https://morphe.software/add-source?github=belalsalah74/pogo1

## 🩹 Patches list

<!-- PATCHES_START EXPANDED -->
> **[v1.0.0-dev.1](https://github.com/belalsalah74/pogo1/releases/tag/v1.0.0-dev.1)**&nbsp;&nbsp;•&nbsp;&nbsp;`dev`&nbsp;&nbsp;•&nbsp;&nbsp;1 patches total
<details open>
<summary>📦 Pokémon GO&nbsp;&nbsp;•&nbsp;&nbsp;1 patch</summary>
<br>

**🎯 Supported versions:**

| 0.431.0 |
| :---: |

| 💊&nbsp;Patch | 📜&nbsp;Description | ⚙️&nbsp;Options |
|----------|----------------|-----------|
| [Enhanced throw](#enhanced-throw) | Installs the experimental native research loader for Pokémon GO 0.431.0 arm64-v8a. |  |

</details>

<!-- PATCHES_END -->

### 🛠️ Building locally

- Build the native libraries first (needs the Android NDK): `ANDROID_NDK_HOME=/path/to/ndk bash native/build.sh`
- Run `./gradlew :patches:buildAndroid`
- The built patches .mpp file is found in `patches/build/libs/patches-*.mpp`
- Patch the mpp file using [Morphe-Desktop](https://github.com/MorpheApp/morphe-desktop)
  like any other patch bundle.

### 🔎 Troubleshooting the native hook

The hook writes its progress to logcat (tag `PGOHOOK431`) and to a file that survives logcat
eviction. After launching the patched game, read it with:

```
adb shell cat /sdcard/Android/data/com.nianticlabs.pokemongo/files/pgo_hook.log
```

See the [Morphe documentation](https://github.com/MorpheApp/morphe-documentation) for more information.

## 📜 License

Pokémon GO Patches are licensed under the [GNU General Public License v3.0](LICENSE)

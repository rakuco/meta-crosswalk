This instruction explains step by step how to build a platform image including Chromium ozone-gbm using Yocto.

## Build a platform image with Yocto.
### Clone Yocto recipe repositories that need to build Chromium.
```
$ mkdir yocto
$ cd yocto/
$ git clone http://git.yoctoproject.org/git/poky
$ cd poky
$ git checkout pyro  (latest stable branch)
$ cd ..
$ git clone https://github.com/joone/meta-crosswalk
$ git clone https://github.com/imyller/meta-nodejs.git
$ git clone git://git.yoctoproject.org/meta-intel.git
$ git clone git://git.openembedded.org/meta-openembedded
```
### Checkout chromium58 branch.
```
$ cd meta-crosswalk
$ git checkout chromium58_gbm
$ cd ../yocto/poky
$ source oe-init-build-env
$ cd build
```
### Edit the Yocto build configuration
You can clone my build configuration. For this, remove the generated conf folder.
```
$ rm -rf conf
$ git clone https://github.com/joone/yocto-build-conf.git
```
Instead, you need to update the path of recipes in bblayer.conf.

You can also edit bblayer.conf and local.conf in the generated conf folder as follows:
```
$ vi conf/bblayer.conf
```
Add meta-crosswalk and meata-nodejs
```
BBLAYERS ?= " \
/home/joone/git/yocto/poky/meta \
/home/joone/git/yocto/poky/meta-yocto \
/home/joone/git/yocto/meta-crosswalk \
/home/joone/git/yocto/meta-nodejs \
/home/joone/otc/yocto/meta-intel \
/home/joone/otc/yocto/meta-openembedded/meta-oe \
/home/joone/otc/yocto/meta-openembedded/meta-filesystems \
"
```
```
$ vi conf/local.conf
```
Add following line to to include Chromium and its dependencies in the image.
```
IMAGE_INSTALL_append = " chromium"
CORE_IMAGE_EXTRA_INSTALL += "libgles2-mesa libegl"

```
Also, you can select a specific target machine as follows: (default: qemux86)
```
MACHINE ?= "intel-corei7-64"
```

### Update Linux kernel and Mesa recipes

```
diff --git a/meta-yocto-bsp/recipes-kernel/linux/linux-yocto_4.10.bbappend b/meta-yocto-bsp/recipes-kernel/linux/linux-yocto_4.10.bbappend
index 6430548..5768bbc 100644
--- a/meta-yocto-bsp/recipes-kernel/linux/linux-yocto_4.10.bbappend
+++ b/meta-yocto-bsp/recipes-kernel/linux/linux-yocto_4.10.bbappend
@@ -24,3 +24,5 @@ LINUX_VERSION_genericx86-64 = "4.10.9"
 LINUX_VERSION_edgerouter = "4.10.9"
 LINUX_VERSION_beaglebone = "4.10.9"
 LINUX_VERSION_mpc8315e-rdb = "4.10.9"
+
+SRC_URI += "file:///home/joone/otc/yocto/poky/build/conf/kernel_dma_buf.patch" 
diff --git a/meta/recipes-graphics/mesa/mesa.inc b/meta/recipes-graphics/mesa/mesa.inc
index 0348bb2..7d81e1e 100644
--- a/meta/recipes-graphics/mesa/mesa.inc
+++ b/meta/recipes-graphics/mesa/mesa.inc
@@ -46,7 +46,7 @@ PACKAGECONFIG[vulkan] = "--with-vulkan-drivers=intel, --without-vulkan-drivers"
 
 PACKAGECONFIG[gles] = "--enable-gles1 --enable-gles2, --disable-gles1 --disable-gles2"
 
-EGL_PLATFORMS  = "drm"
+EGL_PLATFORMS  = "surfaceless"
 EGL_PLATFORMS .="${@bb.utils.contains('PACKAGECONFIG', 'x11', ',x11', '', d)}"
 EGL_PLATFORMS .="${@bb.utils.contains('PACKAGECONFIG', 'wayland', ',wayland', '', d)}"
 PACKAGECONFIG[egl] = "--enable-egl --with-egl-platforms=${EGL_PLATFORMS}, --disable-egl"
```
Build Chromium and create an image using bitbak command.
```
$ bitbake core-image-base
```
In case of building only Chromium,
```
$ bitbake chromium
```

## Run your target image on Intel compute stick or MinnowBoard Max
First, you need to create a USB image as follows:
```
$ cd tmp/deploy/images/<build_arch_dir>
$ sudo dd if=<image_name>.hddimg of=/dev/sdc
```

If the board does not boot automactically, you can boot it manually from the EFI shell as follows:
```
Shell> fs0:
Shell> bootx64
```
You can test ozone-gbm by running ozone-demo or mus_demo:
```
$ /usr/lib/chromium/ozone-demo
$ /usr/lib/chromium/mash --service=mus_demo
```
Run chromium
```
$ /usr/lib/chromium/chrome https://webglsamples.org/aquarium/aquarium.html --mash --no-sandbox --window-manager=simple_wm --ash-host-window-bounds=2160x1440

```
Run mash session
```
$ /usr/lib/chromium/mash https://webglsamples.org/aquarium/aquarium.html --service=mash_session --no-sandbox --window-manager=simple_wm --ash-host-window-bounds=2160x1440
```
If your display resolution is 1920x1080, you don't need to pass --ash-host-window-bounds parameter.

## Build Chromium external source
The current Yocto recipe only works with the stable release of Chromium so there might be many build issues with ToT, but you can try it out.
```
INHERIT += "externalsrc"
EXTERNALSRC_pn-chromium = "/home/joone/otc/chromium/src"
```

## Tips

## Proxy suooirt
When mash or chrome launches, we can pass a proxy auto-config (PAC) file as runtime parameter as follow:
```
--proxy-pac-url=URL
```

### Add truetype fonts
This is a temporary solution.
```
$ cd  ~yocto/poky/build/tmp/deploy/rpm 
$ rpm -ifvh corei7_64/fontconfig-utils-2.12.1-r0.corei7_64.rpm
$ rpm -ifvh noarch/ttf*
```
If there are not ttf rpm packages, build them as follows:
```
$ bitbake ttf-dejavu
$ bitbake ttf-hunkyfonts
```
### Find the build files
```
$ cd ~yocto/poky/build/tmp/work/core2-64-poky-linux/chromium/58.0.3029.110-r0/chromium-58.0.3029.110/out/Release
```
### To recompile your source code if you change a line in it.
```
$ bitbake -f -c compile chromium
```
## References
* https://layers.openembedded.org/layerindex/branch/master/recipes/
* http://www.yoctoproject.org/docs/latest/yocto-project-qs/yocto-project-qs.html
* https://github.com/ds-hwang/wiki/wiki/build-chromium-on-yocto
* https://github.com/rakuco/meta-crosswalk
* https://wiki.yoctoproject.org/wiki/Tracing_and_Profiling


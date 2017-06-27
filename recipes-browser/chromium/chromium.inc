DESCRIPTION = "Chromium is an open-source browser project that aims to build a safer, faster, and more stable way for all users to experience the web."
HOMEPAGE = "https://www.chromium.org/Home"

SRC_URI = "https://commondatastorage.googleapis.com/chromium-browser-official/chromium-${PV}.tar.xz"
SRC_URI[md5sum] = "aca4aeff806e1230912aa9c9df6b7be7"
SRC_URI[sha256sum] = "f24cef3dd2acf9dd5ccdeeca47fea42d1c1ddff32b7375dc9e0cd35a4e8d78ff"

# Chromium itself is licensed under the 3-clause BSD license. However, it
# depends upon several other projects whose copyright files are listed in
# LIC_FILES_CHKSUM. The licenses in those files vary, but none of them should
# cause the main Chromium license to change. The list of copyright files was
# generated with the aid of Chromium's own tools/licenses.py module. As such,
# it contains more items than we actually use in our build (e.g. there are
# components specific to Android, OS X, iOS etc, or bundled dependencies that
# we replace with system-wide versions). For more information about each
# project, you can consult README.chromium in each directory listed below.
# LICENSE does not cover all the licenses in LIC_FILES_CHKSUM; instead, it
# currently contains Chromium's license (BSD-3-Clause) as well as Blink's
# (LGPL2.0, LGPL2.1 and BSD-2-Clause).

LICENSE = "CLOSED"

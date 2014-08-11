git clone -b v210-stable git://anongit.freedesktop.org/systemd/systemd-stable
cd systemd-stable/ && git pull

./autogen.sh --prefix=/usr --with-udevrulesdir=/lib/udev/rules.d --with-distro=debian --with-sysvinit-path= --with-sysvrcd-path= --disable-selinux --enable-libcryptsetup --disable-audit --disable-tcpwrap --disable-plymouth

## Later try to use it with kdbus : --enable-kdbus

# Audit slows down the kernel, only good for corporate use
# IMA goes hand to hand with TPM - checksums can do the same job
# AppArmor/SElinux/SECCOMP/SMACK are different ways to do MAC, but require configuration on debian
#
# localstatedir is where the journal is kept, /var/log
# libdir is where the libs links and pkgconfig are installed, /usr/lib
# rootlibdir is where libs links target and the pamsecurity is, /lib
# firmware-path is where firmware files are found, /lib/firmware
# rootprefixdir and prefix is where /lib/udev/ and /lib/systemd/ go, with some bins in bin/
# bindir is where the rest of the binaries go, /bin
# sysconfdir is where etc/systemd, etc/udev and the etc/.d directories (tmpfiles, binfmt, modules-load, sysctl, pam) along with xdg, kernel
# sysroot is where the required includes and libs are found
# datarootdir is where README and some code sample will be installed, with doc/systemd automatically being added as a suffix
# datadir is where the locales and dbus-1 services go
# sharedata is where polkit-1, kbd-model-map and some pkgconfig files go
# sysvinit compat and zsh completion are also disabled.

./configure CFLAGS='-O2 -ftrapv' --enable-gtk-doc --disable-selinux --disable-smack --disable-ima --with-sysvinit-path="" --with-sysvrcnd-path="" --localstatedir=/var --libdir=/usr/lib --with-rootlibdir=/lib --with-rootprefix=/ --prefix=/ --bindir=/bin --sysconfdir=/etc --with-rc-local-script-path-stop=/etc/halt.local --with-firmware-path=/lib/firmware --with-kbd-setfont=/bin/setfont --with-kbd-loadkeys=/usr/bin/loadkeys --with-html-dir=/usr/share/doc/systemd --with-sysroot=/ --with-debug-tty=/dev/tty11 --with-bashcompletiondir=/etc/bash_completion.d/systemd --with-zshcompletiondir="" --datadir=/usr/share --mandir=/usr/share/man --datarootdir="/usr/share" --includedir=/usr/include/

make && make install

# missing from install
cp systemd /sbin

# Move locales
cd /share/locale/ ; for i in */* ; do mv $i/* /usr/share/locale/$i ; done
rm -fr /share

# add myhostname to /etc/nsswitch.conf

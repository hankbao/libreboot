---
title: GPG
...

    $ **gpg \--recv-keys**

The GPG key can also be downloaded with this exported dump of the
pubkey: [lbkey.asc](lbkey.asc).


    $ **sha512sum -c sha512sum.txt**
    $ **gpg \--verify sha512sum.txt.sig**

Old GPG key (no longer used):

    pub   4096R/656F212E 2014-07-04
          Vingerafdruk van de sleutel = C923 4BA3 200C F688 9CC0  764D 6E97 D575 656F 212E
          uid                  Libreboot Releases (signing key) <releases@libreboot.org>
          sub   4096R/EC42160E 2014-07-04


% Macbook 2,1

There is an Apple laptop called the macbook2,1 from late 2006 or early
2007 that uses the same i945 chipset as the ThinkPad X60 and ThinkPad
T60. A developer ported coreboot to their macbook2,1, and now libreboot
can run on it.

Mono Moosbart is the person who wrote the port for macbook2,1.
Referenced below are copies (up to date at the time of writing,
20140630) of the pages that this person wrote when porting coreboot to
the macbook2,1. They are included here in case the main site goes down
for whatever reason, since they include a lot of useful information.

Backups created using wget:

    $ wget -m -p -E -k -K -np http://macbook.donderklumpen.de/
**\$ wget -m -p -E -k -K -np
http://macbook.donderklumpen.de/coreboot/**\
Use **-e robots=off** if using this trick for other sites and the site
restricts using robots.txt

**Links to wget backups (and the backups themselves) of Mono's pages
(see above) removed temporarily. Mono has given me permission to
distribute them, but I need to ask this person to tell me what license
these works fall under first. Otherwise, the above URLs should be fine.
NOTE TO SELF: REMOVE THIS WHEN DONE**

### Installing GNU+Linux distributions (on Apple EFI firmware)

-   [Parabola GNU+Linux installation on a macbook2,1 with Apple EFI
    firmware](#) (this is a copy of Mono's page, see above)

How to boot an ISO: burn it to a CD (like you would normally) and hold
down the Alt/Control key while booting. The bootloader will detect the
GNU+Linux CD as 'Windows' (because Apple doesn't think GNU+Linux
exists). Install it like you normally would. When you boot up again,
hold Alt/Control once more. The installation (on the HDD) will once
again be seen as 'Windows'. (it's not actually Windows, but Apple
likes to think that Apple and Microsoft are all that exist.) Now to
install libreboot, follow
[../install/\#flashrom\_macbook21](../install/#flashrom_macbook21).

### Information about coreboot

-   [Coreboot on the macbook2,1](#) (this is a copy of Mono's page, see
    above)

### coreboot wiki page

-   <https://www.coreboot.org/Board:apple/macbook21>

### Compatible models

It is believed that all models are compatible, listed here:

-   <http://www.everymac.com/ultimate-mac-lookup/?search_keywords=MacBook2,1>

Specifically (Order No. / Model No. / CPU):

-   MA699LL/A / A1181 (EMC 2121) / Intel Core 2 Duo T5600 **(tested -
    working)**
-   MA701LL/A / A1181 (EMC 2121) / Intel Core 2 Duo T7200 **(tested -
    working)**
-   MB061LL/A / A1181 (EMC 2139) / Intel Core 2 Duo T7200 (untested)
-   MA700LL/A / A1181 (EMC 2121) / Intel Core 2 Duo T7200 **(tested -
    working)**
-   MB063LL/A / A1181 (EMC 2139) / Intel Core 2 Duo T7400 (works)
-   MB062LL/A / A1181 (EMC 2139) / Intel Core 2 Duo T7400 **(tested -
    working)**

Also of interest:
[../git/\#config\_macbook21](../git/#config_macbook21).

Unbricking: [this page shows disassembly
guides](https://www.ifixit.com/Device/MacBook_Core_2_Duo) and mono's
page (see above) shows the location of the SPI flash chip on the
motherboard. [How to remove the
motherboard](https://www.ifixit.com/Guide/MacBook+Core+2+Duo+PRAM+Battery+Replacement/529).

For external flashing, refer to
[../install/bbb\_setup.html](../install/bbb_setup.html).

You need to replace OS X with GNU+Linux before flashing libreboot. (OSX
won't run at all in libreboot).

There are some issues with this system (compared to other computers that
libreboot supports):

This is an apple laptop, so it comes with OS X: it has an Apple
keyboard, which means that certain keys are missing: insert, del, home,
end, pgup, pgdown. There is also one mouse button only. Battery life is
poor compared to X60/T60 (for now). It also has other issues: for
example, the Apple logo on the back is a hole, exposing the backlight,
which means that it glows. You should cover it up.

The system does get a bit hotter compared to when running the original
firmware. It is certainly hotter than an X60/T60. The heat issues have
been partially fixed by the following patch (now merged in libreboot):
<https://review.coreboot.org/#/c/7923/>.

**The MacBook2,1 comes with a webcam, which does not work without
proprietary software. Also, webcams are a security risk; cover it up! Or
remove it.**

A user reported that they could get better response from the touchpad
with the following in their xorg.conf:

    Section "InputClass"
     Identifier "Synaptics Touchpad"
     Driver "synaptics"
     MatchIsTouchpad "on"
     MatchDevicePath "/dev/input/event*"
     Driver "synaptics"
    # The next two values determine how much pressure one needs
    # for tapping, moving the cursor and other events.
     Option "FingerLow" "10"
     Option "FingerHigh" "15"
    # Do not emulate mouse buttons in the touchpad corners.
     Option "RTCornerButton" "0"
     Option "RBCornerButton" "0"
     Option "LTCornerButton" "0"
     Option "LBCornerButton" "0"
    # One finger tap = left-click
     Option "TapButton1" "1"
    # Two fingers tap = right-click
     Option "TapButton2" "3"
    # Three fingers tap = middle-mouse
     Option "TapButton3" "2"
    # Try to not count the palm of the hand landing on the touchpad
    # as a tap. Not sure if helps.
     Option "PalmDetect" "1"
    # The following modifies how long and how fast scrolling continues
    # after lifting the finger when scrolling
     Option "CoastingSpeed" "20"
     Option "CoastingFriction" "200"
    # Smaller number means that the finger has to travel less distance
    # for it to count as cursor movement. Larger number prevents cursor
    # shaking.
     Option "HorizHysteresis" "10"
     Option "VertHysteresis" "10"
    # Prevent two-finger scrolling. Very jerky movement
     Option "HorizTwoFingerScroll" "0"
     Option "VertTwoFingerScroll" "0"
    # Use edge scrolling
     Option "HorizEdgeScroll" "1"
     Option "VertEdgeScroll" "1"
    EndSection

A user reported that the above is only for linux kernel 3.15 or lower.
For newer kernels, the touchpad works fine out of the box, except middle
tapping.

A user submitted a utility to enable 3-finger tap on this laptop. It's
available at *resources/utilities/macbook21-three-finger-tap* in the
libreboot git repository.


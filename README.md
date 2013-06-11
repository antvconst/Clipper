Clipper
========

Univerval background application(link shortening, screenshot uploading, pastebin publishing, qr code generating).

Features:
---------
1. Making and uploading screenshots with only one key-press (using Imageshack.us API).
2. Shortening links (using Google Link Shortener API - goo.gl).
3. Rapid text/code publishing with ability to set a password on your paste (using tny.cz API).
4. Quick QR code creating from any text including URLs (using QRickit QR Code API).

Platforms:
----------
1. Linux
2. Windows

Building from source code:
--------------------------
1. Install Qt 5.0 or greater (Qt 5.1 recommended).
2. Go to 'src' folder.
3. Run 'qmake'.
4. Run 'make'.
5. Run 'sudo make install'.

Alternatively you can install binary package (available for Windows and Debian-based Linux distributions).

Application uses a bit modified parts of QxtGlobalShortcut and K8JSON libraries, all credits for them go to their developers.
-----------------------------------------------------------------------------------------------------------------------------
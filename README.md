#Clipper

[![Build Status](https://travis-ci.org/falceeffect/Clipper.png?branch=master)](https://travis-ci.org/falceeffect/Clipper)

###Features:

* Making and uploading screenshots (fullscreen or partial) with only one key-press (using Imageshack.us API).
   Screenshots can be saved as files in provided directory.
* Shortening links (using Google Link Shortener API - goo.gl).
* Rapid text/code publishing with ability to set a password on your paste (using tny.cz API).
* Quick QR code creating from any text including URLs (using QRickit QR Code API).
* Multiple text copying, which will be automerged and placed into clipboard.
* Tracking and logging clipboard history.

###Platforms:

* Linux
* Windows

Prebuild binary packages are available at [project's webpage](http://clipperapp.tk).

###Building from source code:

Run following commands from 'Clipper/src' directory.

* `./configure`
* `make`
* `sudo make install`

*Includes modified [K8JSON](https://gitorious.org/k8jsonqt).*
*Uses [UGlobalHotkey](https://github.com/falceeffect/UGlobalHotkey) library.*
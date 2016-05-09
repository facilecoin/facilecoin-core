FacileCoin Core 0.10.2
=====================

Setup
---------------------
...

Running
---------------------
The following are some helpful notes on how to run FacileCoin on your native platform. 

### Unix

You need the Qt4 run-time libraries to run FacileCoin-Qt. On Debian or Ubuntu:

	sudo apt-get install libqtgui4

Unpack the files into a directory and run:

- facilecoin-qt (GUI, 64-bit) or facilecoind (headless, 64-bit)



### Windows

Unpack the files into a directory, and then run facilecoin-qt.exe.

### OSX

Drag FacileCoin-Qt to your applications folder, and then run FacileCoin-Qt.


Building
---------------------
The following are developer notes on how to build FacileCoin on your native platform. They are not complete guides, but include notes on the necessary libraries, compile flags, etc.

- [OSX Build Notes](build-osx.md)
- [Unix Build Notes](build-unix.md)

Development
---------------------
The FacileCoin repo's [root README](https://github.com/facilecoin/facilecoin/blob/master/README.md) contains relevant information on the development process and automated testing.

- [Coding Guidelines](coding.md)
- [Multiwallet Qt Development](multiwallet-qt.md)
- [Release Notes](release-notes.md)
- [Release Process](release-process.md)
- [Source Code Documentation (External Link)](https://dev.visucore.com/facilecoin/doxygen/)
- [Translation Process](translation_process.md)
- [Unit Tests](unit-tests.md)


### Miscellaneous
- [Assets Attribution](assets-attribution.md)
- [Files](files.md)
- [Tor Support](tor.md)
- [Init Scripts (systemd/upstart/openrc)](init.md)

License
---------------------
This product includes software developed by the OpenSSL Project for use in the [OpenSSL Toolkit](https://www.openssl.org/). This product includes
cryptographic software written by Eric Young ([eay@cryptsoft.com](mailto:eay@cryptsoft.com)), and UPnP software written by Thomas Bernard.

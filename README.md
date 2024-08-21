# TOTPManager

[WIP] TOTP Codes Manager

### Features list
- [X] Parse TOTP
- [ ] Clean code
- [ ] Saving
- [ ] GUI?

### Building
1. Make sure you selected MSVC compiler
2. Run `cmake -DCMAKE_TOOLCHAIN_FILE="thirdparty/vcpkg/scripts/buildsystems/vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static`

### Usage
Run program and paste secret key. If you have a QR code -> use online scanner.

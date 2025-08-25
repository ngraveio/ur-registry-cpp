# ur-registry-cpp

C++17 library implementing the **UR Registry** used by NGRAVE ZERO hardware wallet. Objects are CBOR-encoded and transported via **UR** (Uniform Resources), typically displayed as QR codes, as specified in the [NGRAVE research papers](https://github.com/ngraveio/Research).

## Dependencies

Runtime/Link:
- **tinycbor**: CBOR library  
  https://github.com/ngraveio/tinycbor
- **bc-ur-cpp**: UR framing and CBOR helpers  
  https://github.com/ngraveio/bc-ur-cpp
- **libwally-core-lite**: crypto/BIP32 primitives  
  https://github.com/ngraveio/libwally-core-lite

> Ensure headers and libs are installed (typically under `/usr/local/include` and `/usr/local/lib`).

## Build & Install

```sh
./autogen.sh
./configure
make
sudo make install
```
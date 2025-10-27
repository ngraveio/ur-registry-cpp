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

Development/Test:
- **cppunit** 1.15.1
- **cppcheck** 2.7 (for static analysis)
- **flawfinder** (for security scanning)

## Build & Install

```sh
./autogen.sh
./configure
make
sudo make install
```

## Run Tests & Static Analysis

- Build and install the static library
```sh
./autogen.sh
./configure --disable-shared --enable-static
make
```

- To run both unit tests (cppunit) and static analysis (cppcheck and flawfinder), use:
```
make check
```

## Examples

Two interactive apps are provided to help explore and test the encoding and decoding of UR types:

1. `decode_ur_interactive`

An interactive decoder that takes a full UR string (e.g. ur:eth-sign-request/...) as input and displays its decoded fields.

- Build:

```
./autogen.sh
./configure --disable-shared --enable-static
make
```

- Run:

```
./examples/decode_ur_interactive
```

- Usage:
   - Paste or type any valid UR string (e.g. from a QR code).
   - Type demo to run built-in decoding examples.
   - Type q to quit.

- Example

```
./examples/decode_ur_interactive
UR Registry Decoder
Paste a UR below (or 'demo' to decode pre-defined UR types, or 'q' to quit):

UR> ur:sign-request/pdadtpdagdspoefsrfroadfdzconpsrpzmtswfnnmoaotaoyrhoeadayaocsfnaxtantjooyadlylrcsfzwkcsfzykaafxadaoaxahinjzinjskpiniehsjojoamadatfeadaoaxaaahaytaoystoyadoeadiodkfwfpghfxfddmaolnlfihghhddedydtahlfihghhddeehdtahlfihghhddeeydtahlfiygugafldeehdtahlfiygugafldeeydtahlfiygugafldeeodtahldvortso   
[sign-request]
  Request ID      : c8a23dbc-b801-48fd-a5ac-b6ffd7f39e92
 Coin Identity: [coin-identity]
  Coin ID         : uai://secp256k1.60
 Derivation path: [keypath]
  Derivation path : m/<64;64'>
  Sign Data       : 010203
  Origin          : liquidapp
  TX type         : 1
  Address         : 0102030405
 Intent: [intent]
  Prefix          : $BATCH.
  Fields          : [(TX(0), 5)(TX(1), 5)(TX(2), 5)(SIG(1), 5)(SIG(2), 5)(SIG(3), 5)]

UR> q
```

2. `encode_ur_interactive`

An interactive encoder that prompts you to enter data step-by-step to build and encode supported UR types.

- Build:

```
./autogen.sh
./configure --disable-shared --enable-static
make
```

- Run

```
./examples/encode_ur_interactive
```

- Usage:
  - When prompted, type the UR type you want to encode (e.g. coin-identity, hdkey, portfolio, etc.).
  - Fill in the requested fields interactively.
  - The resulting UR string will be printed to the console.
  - Type q to quit.

- Example:

```
./examples/encode_ur_interactive
Enter UR type to encode or 'q' to quit: sign-response
Enter Request ID (UUID format, e.g. 9b1deb4d-3b7d-4bad-9bdd-2b0d7b3dcb6d, leave empty to skip):
9b1deb4d-3b7d-4bad-9bdd-2b0d7b3dcb6d
Enter signature (hex without 0x prefix): d4f0a7bcd95bba1fbb1051885054730e3f47064288575aacc102fbbf6a9a14daa066991e360d3e3406c20c00a40973eff37c7d641e5b351ec4a99bfe86f335f713
Enter origin (any string, leave empty to skip): 
Enter public key (hex without 0x prefix, leave empty to skip): 
Encoded UR: ur:sign-response/oeadtpdagdndcawmgtfrkigrpmndutdnbtkgfssbjnaohdfptywtosrftahprdctrkbegylogdghjkbafhflamfwlohghtpsseaozorsimnybbtnnbiynlckenbtfmeeamsabnaeoxasjkwswfkekiieckhpecckssptndzelnwfecylbwdlsgvazt

Enter UR type to encode or 'q' to quit: q
```

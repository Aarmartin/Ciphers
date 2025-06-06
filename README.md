# Cipher Project

This project is a comprehensive C++ cryptography suite implementing a variety of classical and modern encryption schemes, including post-quantum cryptography. It supports modular key generation, encryption, and decryption for each cipher via unified interfaces.

## Project Structure

    Ciphers-master/
    ├── include/        # Header files
    ├── src/            # Cipher and utility implementations
    ├── Makefile        # Build instructions

After running `make`, three primary binaries are produced:

- `keygen` — Generates public/private keys
- `cipher` — Encrypts and decrypts files
- `tool` — (Optional) Utilities for math and testing

## Usage

### Key Generation

To generate key pairs:

    ./bin/keygen <cipher> <keyname>

- `<cipher>` can be one of:
  - `rsa`
  - `lwe`
  - `affine`
  - `caesar`
  - `vigenere`
- `<keyname>` is the base name for output files:
  - `<keyname>.pk` for the public key
  - `<keyname>.sk` for the private key
  - `<keyname>.key` for symmetric key files

**Examples:**

    ./bin/keygen rsa mykey
    ./bin/keygen lwe lwekey

This will produce:
- `mykey.pk` and `mykey.sk` for RSA
- `lwekey.pk` and `lwekey.sk` for LWE

### Encryption and Decryption

To encrypt or decrypt:

    ./bin/cipher <cipher> <action> <inputfile> <keyfile>

- `<cipher>`: one of `rsa`, `lwe`, `affine`, `caesar`, `vigenere`
- `<action>`: `encrypt` or `decrypt`
- `<inputfile>`: plaintext or ciphertext file depending on the mode
- `<keyfile>`: use the `.pk` file for encryption, the `.sk` for decryption, and the `.key` file for both if the cipher is symmetric

**Output files** will automatically be named based on the input:
- Encrypted files: `<inputfile>.enc`
- Decrypted files: `<inputfile>.dec`

**Examples:**

    ./bin/cipher encrypt rsa message.txt mykey.pk
    ./bin/cipher decrypt rsa message.enc mykey.sk

    ./bin/cipher encrypt lwe binary.txt lwekey.pk
    ./bin/cipher decrypt lwe binary.enc lwekey.sk

## Input Format Notes

### LWE Cipher

- The input **plaintext file must contain only a binary string**, i.e., a sequence of `0`s and `1`s with no spaces, newlines, or other characters.
- Example content: `0110101010110111`

### RSA, Affine, Caesar, Vigenère

- Accept standard ASCII text input — full sentences, words, etc.
- Example content: `Hello, world!`

## Notes

- All keys and ciphertexts are serialized using custom `<<` and `>>` stream operators.
- Internally, arithmetic utilities (GCD, mod inverse, congruences, etc.) live under the `CipherUtils` namespace.
- LWE and RSA use GMP (`gmpxx.h`) for large integer support.
- The project is modular and designed for extending to new cipher types easily.

## License and Contributions

This project is open to extension and adaptation for cryptographic experimentation and education. Feel free to fork or submit improvements.

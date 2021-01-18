# PS-HDD-Tools
:warning: These tools have not been thoroughly tested.

This repository includes a collection of tools for inspecting encrypted PS3 or PS4 hard drives.

Supports NAND or NOR PS3 hdd's and any PS4 hdd's.

Requires either the EID root key (PS3) or EAP hdd key (PS4).

# Thanks to
* flat_z
* glevand
* naehrwert
* 3141card

# Tools
## Decrypt Tool
The decryption tool will take an hdd image and a key file as input and output a decrypted user partition. On PS3, this would be the `dev_hdd0/` partition, and on PS4, it would be the `user/` partition. Other partitions will be supported eventually.
### Usage
```
decrypt-tool <input image> <key file> <output file>
```
## Hex Viewer
Hex viewer that allows you to view decrypted contents of an HDD image.

You can go to any offset by pressing `CTRL + G` and entering either a decimal value or a hexadecimal value beginning with "0x".

## Mount Tool
This tool demonstrates usage of the virtual file system and simply prints out the contents of each file system. Eventually more functionality will be added.
### Usage
```
mount-tool <input image> <key file>
```

# Todo
- Decrypt second layer of encryption for vflash (PS3)
- Add support for arcade hdd's
- Mount and display file systems
- Data recovery tools
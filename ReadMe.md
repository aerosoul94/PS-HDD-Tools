# PS-HDD-Tools
![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/aerosoul94/PS-HDD-Tools/main.yaml?branch=master)

:warning: These tools have not been thoroughly tested.

This repository includes a collection of tools for inspecting encrypted PS3 or PS4 hard drives.

Supports NAND or NOR PS3 hdd's and any PS4 hdd's.

Requires either the EID root key (PS3) or EAP hdd key (PS4).

# Contents
- [PS-HDD-Tools](#ps-hdd-tools)
- [Contents](#contents)
- [Thanks to](#thanks-to)
- [Contributors](#contributors)
- [Tools](#tools)
    - [Decrypt Tool](#decrypt-tool)
      - [Usage](#usage)
    - [Hex Viewer](#hex-viewer)
    - [Mount Tool](#mount-tool)
      - [Usage](#usage-1)
    - [File Explorer](#file-explorer)
- [Todo](#todo)

# Thanks to
* flat_z
* glevand
* naehrwert
* 3141card

# Contributors
* Tdijital
* jason098

# Tools
### Decrypt Tool
The decryption tool will take an hdd image and a key file as input and output a decrypted partition.
#### Usage
```
Usage: decrypt-tool [command] [arguments...]

Commands:
  list    <input> <key file>                       List available partitions.
  decrypt <input> <key file> <partition> <output>  Decrypt a partition to a new file.  
```
### Hex Viewer
Hex viewer that allows you to view decrypted contents of an HDD image.

You can go to any offset by pressing `CTRL + G` and entering either a decimal value or a hexadecimal value beginning with "0x".

### Mount Tool
This tool demonstrates usage of the virtual file system and simply prints out the contents of each file system. Eventually more functionality will be added.
#### Usage
```
Usage: mount-tool [command] [arguments...]

Commands:
  list    <input> <key file>              List available partitions.
  display <input> <key file> <partition>  Display file system contents.
```

### File Explorer
A GUI for exploring the drive's file systems.

# Todo
- Add support for arcade hdd's
- Data recovery tools
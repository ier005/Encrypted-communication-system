# Encrypted Communication System

An encrypted communication system:
- A Linux kernel module - enccom
- A userspace application to interact with *enccom* - uaccess


*Linux kernel version: 4.9.0*

## Config Rules

format: `in/out cipher ip key`

available cipher type: AES-ECB(128), AES-CBC(128), AES-XTS(128), AES-CTR(128)

For exanple:

	out AES-ECB(128) 192.168.98.238 0123456789abcdef


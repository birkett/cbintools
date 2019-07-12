# C Bin Tools<br />

### bin2c - Convert a binary into a C char array, for embedding into an application.<br />
Usage: `bin2c [-h] <source_bynary_file> <dest_header_file> <array_name>`
* `-h` : [Optional] Output in hexadecimal format. The default format is decimal.
* `<source_bynary_file>` : Path to the binary file to convert to C char array.
* `<dest_header_file>` : Path and name of the C header output file to generate.
* `<array_name>` : Name of the C array to generate from source_bynary_file.

Example:
`bin2c -h logo.bmp logo.h logoArray`

### c2bin - Convert a C char array back into a binary.<br />
Usage: `c2bin <source_header_file> <dest_bynary_file>`
* `<source_header_file>` : Path to the C header file that contains the C char array.
* `<dest_bynary_file>` : Path and name of the binary output file to generate.

Example:
`c2bin logo.h binarylogo.bmp`

## Support Me
[![Donate Via Paypal](https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif)](https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=CALMNQUWLZNYL)
<br /><br />
## Building
Solution is known to build with Visual Studio 2013 (Community, Pro and Ultimate tested, Express should work), no additional dependencies are needed.<br />
<br /><br />

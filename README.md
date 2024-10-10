# Ghostbusters The Video Game Texture Converter

This tool is designed to convert texture files from the TEX format used in Ghostbusters: The Video Game (2009) and Ghostbusters: The Video Game Remastered (2019) into the DDS (DirectDraw Surface) format.


# How to Build:

To compile the tool on Linux, use the following command:
`g++ -o tex2dds tex2dds.cpp`


# How to Use:

Run the converter by providing the path to a TEX file:
```sh
$ ./tex2dds input_texture.tex [OPTIONS]
```
```
Options:
  -o, --output <output_file.dds>  Specify the output DDS file path and name
  -h, --help                      Show this help message and exit
```
# Credits:

The original code was developed by Jonathan Wilson and barncastle. This version extends the tool with added support for Linux.

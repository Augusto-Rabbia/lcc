# Huffman Coding Compression Project

Use Huffman Coding to compress/decompress a text file. 

## How to use

To create the executable file, use ```make huff```

Then, to compress a file, use `huff C FILE_NAME.txt`. This will create a `.hf` file with the compressed content and a `.tree` file with the Huffman Tree data. 

To decompress the file, use `huff D FILE_NAME.hf`. Make sure the `.tree` file is in the same directory as the executable file.

The project is old, made as an introduction to many of these algorithms. It can definitely be improved.
# RSPS3001-barcode 
This prog encodes and encryptes ticket payload in accordance with RSPS3001 Barcode Presentation, Key Management and Data Specification, Version: 02-01.
## Build
g++ main.cpp -lcrypto -o main \
"public.pem" & "private.pem" must be provided in same directory.
## Trivia
There is fatal typo in spec, Appendix D: Base-26 decoding pseudocode \
in ``accumulator <- full_value & 256 /* where & is the bitwise AND operator*/`` there must be **%** instead of **&**. 
## License 
It's in my glove compartment gimme a sec \
 _\*tyres screeching\*_

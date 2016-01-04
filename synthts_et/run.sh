#!/bin/bash

#   -f      [sisendtekst utf8-s] 
#   -o      [väljund-wav] 
#   -lex    [analüüsi sõnastik] 
#   -lexd   [ühestaja sõnastik] 
#   -m      [hääle nimi, vt kataloogi htsvoices/]


bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o tulemus.wav -f in.txt -m htsvoices/eki_et_tnu.htsvoice 
	

#!/bin/bash

#   -f      [sisendtekst utf8-s] 
#   -o      [väljund-wav] 
#   -lex    [analüüsi sõnastik] 
#   -lexd   [ühestaja sõnastik] 
#   -m      [hääle nimi, vt kataloogi htsvoices/]


bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_tnu.wav -f in.txt -m htsvoices/eki_et_tnu.htsvoice
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_eva.wav -f in.txt -m htsvoices/eki_et_eva.htsvoice
	

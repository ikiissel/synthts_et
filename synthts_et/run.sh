#!/bin/bash

#   -f      [sisendtekst utf8-s] 
#   -o      [väljund-wav] 
#   -lex    [analüüsi sõnastik] 
#   -lexd   [ühestaja sõnastik] 
#   -m      [hääle nimi, vt kataloogi htsvoices/]

#   näide:
#   bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -m htsvoices/eki_et_eva003.htsvoice -o valjund.wav -f sisend.txt

bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o tulemus_c002.wav -f in.txt -m htsvoices/eki_et_tnu.htsvoice 
	

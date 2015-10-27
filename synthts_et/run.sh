#!/bin/bash

# -f [sisendtekst utf8-s] -o [väljund-wav] -lex [analüüsi sõnastik] -lexd [ühestaja sõnastik] -m [hääle nimi, vt htsvoices/]
#

bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -m htsvoices/eki_et_eva003.htsvoice -o tulemus_e003.wav -f sisend.txt
	

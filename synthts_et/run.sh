#!/bin/bash

#   -f      [sisendtekst utf8-s] 
#   -o      [väljund-wav] 
#   -lex    [analüüsi sõnastik] 
#   -lexd   [ühestaja sõnastik] 
#   -m      [hääle nimi, vt kataloogi htsvoices/]
#   -r      [kõnetempo, double, 0.01-2.76] 

bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_eva.wav -f in.txt -m htsvoices/eki_et_eva.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_lis.wav -f in.txt -m htsvoices/eki_et_lis.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_rna.wav -f in.txt -m htsvoices/eki_et_rna.htsvoice -r 1.1
bin/synthts_et -lex dct/et.dct -lexd dct/et3.dct -o out_tnu.wav -f in.txt -m htsvoices/eki_et_tnu.htsvoice -r 1.1



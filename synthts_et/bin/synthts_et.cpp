#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "../lib/etana/proof.h"
#include "../include/mklab.h"
extern "C" {
#include "../include/HTS_engine.h"
}

CDisambiguator Disambiguator;
CLinguistic Linguistic;


typedef unsigned char uchar;

wchar_t *UTF8_to_WChar(const char *string) {
    long b = 0,
            c = 0;
    for (const char *a = string; *a; a++)
        if (((uchar) * a) < 128 || (*a & 192) == 192)
            c++;
    wchar_t *res = new wchar_t[c + 1];

    res[c] = 0;
    for (uchar *a = (uchar*) string; *a; a++) {
        if (!(*a & 128))
            res[b] = *a;
        else if ((*a & 192) == 128)
            continue;
        else if ((*a & 224) == 192)
            res[b] = ((*a & 31) << 6) | a[1]&63;
        else if ((*a & 240) == 224)
            res[b] = ((*a & 15) << 12) | ((a[1]&63) << 6) | a[2]&63;
        else if ((*a & 248) == 240) {
            res[b] = '?';
        }
        b++;
    }
    return res;
}

void ReadUTF8Text(CFSWString &text, const char *fn) {

    std::ifstream fs;
    fs.open(fn, std::ios::binary);
    if (fs.fail()) {
        fprintf(stderr,"Ei leia sisendteksti!\n");
        exit(1);
    }
    fs.seekg(0, std::ios::end);
    size_t i = fs.tellg();
    char* buf = new char[i];
    fs.seekg(0, std::ios::beg);
    fs.read(buf, i);
    fs.close();

    wchar_t* w_temp;
    w_temp = UTF8_to_WChar(buf);
    delete [] buf;
    text = w_temp;
}

int PrintUsage() {
    fprintf(stderr,"\t-f 	[sisendtekst utf8-s] \n");
    fprintf(stderr,"\t-o 	[väljund-wav]  \n");
    fprintf(stderr,"\t-lex 	[analüüsi sõnastik]  \n");
    fprintf(stderr,"\t-lexd	[ühestaja sõnastik]  \n");
    fprintf(stderr,"\t-m 	[hääle nimi, vt kataloogi htsvoices/] \n");
    fprintf(stderr,"\t-r 	[kõnetempo, double, 0.01-2.76] \n");
    fprintf(stderr,"\t-ht 	[float]\n");
    fprintf(stderr,"\t-gvw1 	[float]\n");
    fprintf(stderr,"\t-gvw2 	[float]\n");
    fprintf(stderr,"\t-debug 	[prindi labeli struktuur]\n");
    fprintf(stderr,"\t-raw 	[väljund-raw]\n");
    fprintf(stderr,"\t-dur 	[foneemid koos kestustega, failinimi]\n");
    fprintf(stderr,"\n\tnäide: \n");
    fprintf(stderr,"\t\tbin/synthts_et -lex dct/et.dct -lexd dct/et3.dct \\ \n");
    fprintf(stderr,"\t\t-o out_tnu.wav -f in.txt -m htsvoices/eki_et_tnu.htsvoice \\\n");
    fprintf(stderr,"\t\t-r 1.1\n");
        
    exit(0);
}

char *convert_vec(const std::string & s) {
    char *pc = new char[s.size() + 1];
    strcpy(pc, s.c_str());
    return pc;
}

void fill_char_vector(std::vector<std::string>& v, std::vector<char*>& vc) {
    std::transform(v.begin(), v.end(), std::back_inserter(vc), convert_vec);
}

void clean_char_vector(std::vector<char*>& vc) {
    for (size_t x = 0; x < vc.size(); x++)
        delete [] vc[x];
}

std::string to_stdstring(CFSWString s) {
    std::string res = "";
    for (INTPTR i = 0; i < s.GetLength(); i++)
        res += s.GetAt(i);
    return res;
}

std::vector<std::string> to_vector(CFSArray<CFSWString> arr) {
    std::vector<std::string> v;
    for (INTPTR i = 0; i < arr.GetSize(); i++)
        v.push_back(to_stdstring(arr[i]));
    return v;
}

void cfileexists(const char * filename) {
    FILE *file;
    if (file = fopen(filename, "r")) {
        fclose(file);
        remove(filename);
    }
}

void samplerate(size_t &fr, size_t &fp, float &alpha, size_t br) {
    fr = br * 1000;
    fp = br / 2 * 10;

    if (fr <= 8000) alpha = 0.31; 
        else
    if (fr <= 10000) alpha = 0.35; 
        else
    if (fr <= 12000) alpha = 0.37; 
        else
    if (fr <= 16000) alpha = 0.42; 
        else
    if (fr <= 32000) alpha = 0.45; 
        else
    if (fr <= 44100) alpha = 0.53; 
        else
    if (fr <= 48000) alpha = 0.55; 
        else
            alpha = 0.55;
}

int main(int argc, char* argv[]) {
    size_t num_voices;
    char **fn_voices;
    char* in_fname;
    char* output_fname;
    FILE * outfp;
    char* dur_fname;
    FILE * durfp;    
    bool print_label = false;
    bool write_raw = false;
    bool write_durlabel = false;

    CFSAString LexFileName, LexDFileName;
    HTS_Engine engine;
    double speed = 1.1;
    size_t fr = 48000;
    size_t fp = 240;
    float alpha = 0.55;
    float beta = 0.0;
    float ht = 2.0;
    float th = 0.5;
    float gvw1 = 1.0;
    float gvw2 = 1.2;

    FSCInit();
    fn_voices = (char **) malloc(argc * sizeof (char *));
    
    if (argc < 11) {
        fprintf(stderr, "Viga: liiga vähe parameetreid\n\n");
        PrintUsage();
    }    

    for (int i = 0; i < argc; i++) {
        if (CFSAString("-lex") == argv[i]) {
            if (i + 1 < argc) {
                LexFileName = argv[++i];
            } else {
                return PrintUsage();
            }
        }
        if (CFSAString("-lexd") == argv[i]) {
            if (i + 1 < argc) {
                LexDFileName = argv[++i];
            } else {
                return PrintUsage();
            }
        }
        if (CFSAString("-m") == argv[i]) {
            if (i + 1 < argc) {
                fn_voices[0] = argv[i + 1];
            } else {
                fprintf(stderr, "Viga: puudub *.htsvoice fail\n");
                PrintUsage();
                exit(0);
            }
        }
        if (CFSAString("-o") == argv[i]) {
            if (i + 1 < argc) {
                output_fname = argv[i + 1];
                cfileexists(output_fname);
            } else {
                fprintf(stderr, "Viga: puudb väljundfaili nimi\n");
                PrintUsage();
                exit(0);
            }
        }
        if (CFSAString("-f") == argv[i]) {
            if (i + 1 < argc) {
                in_fname = argv[i + 1];
            } else {
                fprintf(stderr, "Viga: puudb sisendfaili nimi\n");
                PrintUsage();
                exit(0);
            }
        }
        if (CFSAString("-s") == argv[i]) {
            if (i + 1 < argc) {
                samplerate(fr, fp, alpha, atoi(argv[i + 1]));
            }
        }
        if (CFSAString("-r") == argv[i]) {
            if (i + 1 < argc) {
                speed = atof(argv[i + 1]);
            }
        }
        if (CFSAString("-ht") == argv[i]) {
            if (i + 1 < argc) {
                ht = atof(argv[i + 1]);
            }
        }
        if (CFSAString("-gvw1") == argv[i]) {
            if (i + 1 < argc) {
                gvw1 = atof(argv[i + 1]);
            }
        }
        if (CFSAString("-gvw2") == argv[i]) {
            if (i + 1 < argc) {
                gvw2 = atof(argv[i + 1]);
            }
        }        
        if (CFSAString("-debug") == argv[i]) {
            print_label = true;
        }
        if (CFSAString("-raw") == argv[i]) {
            write_raw = true;
        }
        if (CFSAString("-dur") == argv[i]) {
            if (i + 1 < argc) {
                dur_fname = argv[i + 1];
                cfileexists(dur_fname);
                write_durlabel = true;                
            } else {
                fprintf(stderr, "Viga: puudb kestustefaili nimi\n");
                PrintUsage();
                exit(0);
            }
        }

        
    }

    Linguistic.Open(LexFileName);
    Disambiguator.Open(LexDFileName);

    CFSWString text;
    ReadUTF8Text(text, in_fname);
    HTS_Engine_initialize(&engine);

    if (HTS_Engine_load(&engine, fn_voices, 1) != TRUE) {
        fprintf(stderr, "Viga: puudub *.htsvoice. %p\n", fn_voices[0]);
        free(fn_voices);
        HTS_Engine_clear(&engine);
        exit(1);
    }
    free(fn_voices);

    HTS_Engine_set_sampling_frequency(&engine, (size_t) fr);
    HTS_Engine_set_phoneme_alignment_flag(&engine, FALSE);
    HTS_Engine_set_fperiod(&engine, (size_t) fp);
    HTS_Engine_set_alpha(&engine, alpha);
    HTS_Engine_set_beta(&engine, beta);
    HTS_Engine_set_speed(&engine, speed);
    HTS_Engine_add_half_tone(&engine, ht);
    HTS_Engine_set_msd_threshold(&engine, 1, th);
    /*
    HTS_Engine_set_duration_interpolation_weight(&engine, 1, diw);
    HTS_Engine_set_parameter_interpolation_weight(&engine, 0, 0, piw1);
    HTS_Engine_set_parameter_interpolation_weight(&engine, 0, 1, piw2);
    HTS_Engine_set_gv_interpolation_weight(&engine, 0, 0, giw1);
    HTS_Engine_set_gv_interpolation_weight(&engine, 0, 1, giw2);
     */
    HTS_Engine_set_gv_weight(&engine, 0, gvw1);
    HTS_Engine_set_gv_weight(&engine, 1, gvw2);

    text = DealWithText(text);
    CFSArray<CFSWString> res = do_utterances(text);

    INTPTR data_size = 0;
    outfp = fopen(output_fname, "wb");
    if (write_durlabel) durfp = fopen(dur_fname, "w");
    if (!write_raw) HTS_Engine_write_header(&engine, outfp, 1);
    for (INTPTR i = 0; i < res.GetSize(); i++) {

        CFSArray<CFSWString> label = do_all(res[i], print_label);

        std::vector<std::string> v;
        v = to_vector(label);

        std::vector<char*> vc;
        fill_char_vector(v, vc);

        size_t n_lines = vc.size();

        if (HTS_Engine_synthesize_from_strings(&engine, &vc[0], n_lines) != TRUE) {
            fprintf(stderr, "Viga: süntees ebaonnestus.\n");            
            HTS_Engine_clear(&engine);
            exit(1);
        }

        clean_char_vector(vc);
        data_size += HTS_Engine_engine_speech_size(&engine);
        if (write_durlabel) HTS_Engine_save_durlabel(&engine, durfp);
        HTS_Engine_save_generated_speech(&engine, outfp);

        HTS_Engine_refresh(&engine);

    } //synth loop
    
    if (!write_raw) HTS_Engine_write_header(&engine, outfp, data_size);
    if (write_durlabel) fclose(durfp);
    fclose(outfp);

    HTS_Engine_clear(&engine);
    Linguistic.Close();

    FSCTerminate();
    return 0;

}



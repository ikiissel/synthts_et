#include <stdlib.h>             /* for atof() */
#include <string.h>             /* for strcpy() */
#include <math.h>               /* for pow() */

/* hts_engine libraries */
#include "HTS_engine.h"
#include "HTS_hidden.h"

void HTS_Engine_save_durlabel(HTS_Engine * engine, FILE * fp)
{
   size_t i, j;
   size_t frame, state, duration;
   unsigned long kestus = 0;

   int start, end, pikkus;

   HTS_Label *label = &engine->label;
   HTS_SStreamSet *sss = &engine->sss;
   size_t nstate = HTS_ModelSet_get_nstate(&engine->ms);
   double rate = engine->condition.fperiod * 1.0e+07 / engine->condition.sampling_frequency;

   for (i = 0, state = 0, frame = 0; i < HTS_Label_get_size(label); i++) {
      for (j = 0, duration = 0; j < nstate; j++) {
         duration += HTS_SStreamSet_get_duration(sss, state++);
      }

      kestus = ((frame + duration) * rate) - (frame * rate);

      const char * source = HTS_Label_get_string(label, i);
      char * found = strstr( source, "-" );
      start = found - source;
      found = strstr( source, "+" );
      end = found - source;
      pikkus = (end - start) - 1;
      char substr[pikkus];

      memcpy( substr, &source[start+1], pikkus );
      substr[pikkus] = '\0';

      if ((pikkus == 2) && (substr[1] == 'y')) {
         substr[1] = ':';
      }

      int kestus2 = (kestus / 1.0e+07) * 1000;

      //liisile
      fprintf(fp, "%s ", substr);

      frame += duration;
   }
}

int HTS_Engine_engine_speech_size(HTS_Engine * engine) {
   return HTS_GStreamSet_get_total_nsamples(&engine->gss) * sizeof(short);
}

void HTS_Engine_write_header(HTS_Engine * engine, FILE * fp, int fs)
{
   size_t i;
   double x;
   short temp;

   HTS_GStreamSet *gss = &engine->gss;
   char data_01_04[] = { 'R', 'I', 'F', 'F' };
   int data_05_08 = HTS_GStreamSet_get_total_nsamples(gss) * sizeof(short) + 36;
   char data_09_12[] = { 'W', 'A', 'V', 'E' };
   char data_13_16[] = { 'f', 'm', 't', ' ' };
   int data_17_20 = 16;
   short data_21_22 = 1;        /* PCM */
   short data_23_24 = 1;        /* monoral */
   int data_25_28 = engine->condition.sampling_frequency;
   int data_29_32 = engine->condition.sampling_frequency * sizeof(short);
   short data_33_34 = sizeof(short);
   short data_35_36 = (short) (sizeof(short) * 8);
   char data_37_40[] = { 'd', 'a', 't', 'a' };
   //int data_41_44 = HTS_GStreamSet_get_total_nsamples(gss) * sizeof(short);

   /* write header */
   fseek ( fp , 0 , SEEK_SET );
   HTS_fwrite_little_endian(data_01_04, sizeof(char), 4, fp);
   HTS_fwrite_little_endian(&data_05_08, sizeof(int), 1, fp);
   HTS_fwrite_little_endian(data_09_12, sizeof(char), 4, fp);
   HTS_fwrite_little_endian(data_13_16, sizeof(char), 4, fp);
   HTS_fwrite_little_endian(&data_17_20, sizeof(int), 1, fp);
   HTS_fwrite_little_endian(&data_21_22, sizeof(short), 1, fp);
   HTS_fwrite_little_endian(&data_23_24, sizeof(short), 1, fp);
   HTS_fwrite_little_endian(&data_25_28, sizeof(int), 1, fp);
   HTS_fwrite_little_endian(&data_29_32, sizeof(int), 1, fp);
   HTS_fwrite_little_endian(&data_33_34, sizeof(short), 1, fp);
   HTS_fwrite_little_endian(&data_35_36, sizeof(short), 1, fp);
   HTS_fwrite_little_endian(data_37_40, sizeof(char), 4, fp);
   HTS_fwrite_little_endian(&fs, sizeof(int), 1, fp);
}

// vim:ts=3:sw=3:et

/*** PI File IO ***/

extern "C" bool openpiread(char *filename);

extern "C" int openpiwriterdouble(char *filename, int width, int height, int type, bool overwrite);

extern "C" int openpiwriterfloat(char *filename, int width, int height, int type, bool overwrite);

/* return: 0=no error; -1=width, height, and/or frame number are incorrect; */ 
extern "C" int readpidouble(double *out, int width, int height, int index);

/* return: 0=no error; -1=width, height, and/or frame number are incorrect; */ 
extern "C" int readpifloat(float *out, int width, int height, int index);

extern "C" int closereader();

extern "C" int closewriter();

extern "C" int writepidouble(double* data, int num_frames);

extern "C" int writepifloat(float* data, int num_frames);

extern "C" int getwidth();

extern "C" int getheight();

extern "C" int getdepth();

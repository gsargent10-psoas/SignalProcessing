/*** PI File IO ***/

extern "C" bool openpiread(char *filename);

extern "C" int openpiwriter(char *filename, int width, int height, int type, bool overwrite);

/* return: 0=no error; -1=width, height, and/or frame number are incorrect; */ 
extern "C" int readpi(double *out, int width, int height, int index);

//extern "C" int closereader();

extern "C" int closewriter();

extern "C" int writepi(double* data, int num_frames);

extern "C" int getwidth();

extern "C" int getheight();

extern "C" int getdepth();


#define PI           3.14159265358979323846 
#define TWO_PI 2*PI

enum TopologyType {eRandom = 0, eNeighbor2D, eDoughNut, eFull};

float* reset_array(float *r, int size);
float sum(float *a, int size);
float *soft_max_exp(float *r, float *a, int size);
float *soft_max_pw(float *r, float *a, float pw, int size);
float random(float low, float high);
float* random(float low, float high, int sz);
float** random(float low, float high, int sx, int sy);
float **rand_topology(float fill, int sx, int sy);
float **apply_rand_topology(float** matrix, float fill, int sx, int sy);
//float floor(float val, float floor);
//float ceiling(float val, float ceiling);
float **sequentials(float **retval, float start, int x, int y);
float **transpose(float ** a_T, float ** a, int sizex, int sizey);
float **hstack(float **retval, float **a, int ax, float **b, int bx, int rety);
float **vstack(float **retval, float **a, int ay, float **b, int by, int rx);
float *multiply(float *retval, float *a, float *b, int size);
float **multiply(float **retval, float **a, float **b, int sizex, int sizey);
float *add(float *retval, float *a, float *b, int size);
float **add(float **retval, float **a, float **b, int sizex, int sizey);
float *reset(float *a, int size);
float **reset(float **a, int sizex, int sizey);
float dot(float *a, float *b, int size);
float **dot(float **retval, float **a, float **b, int ax, int ay, int by);
float **id(int size);

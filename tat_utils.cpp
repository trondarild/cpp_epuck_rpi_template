#include "tat_utils.h"
#include <iostream>
#include <cmath>

void *memcpy(void *dest, const void *src, size_t n){
	char *csrc = (char *) src;
	char *cdest = (char *) dest;
	for(int i=0; i<n; i++)
		cdest[i] = csrc[i];
}

float *
copy_array(float * r, float * a, int size)
{
    return (float *)memcpy(r, a, size*sizeof(float));
}

float* create_array(int size)
{
    return new float[size];
}

float **
create_matrix(int sizex, int sizey)
{
    if(sizex == 0 || sizey == 0)
    {
        std::cout << "Warning: Attempting to create matrix of size 0\n";
        return NULL;
    }
    
    float * a = new float[sizex*sizey]; //(float *)calloc(sizex*sizey, sizeof(float));
    //float * c = new float[sizey]; //(float **)malloc(sizey*sizeof(float *));
    float ** b = new float*[sizey];
    for (int j=0; j<sizey; j++)
        b[j] = &a[j*sizex];
    return b;
}

float* zeros(int size)
{
    float *retval = create_array(size);
    for (int i = 0; i < size; i++) retval[i] = 0.f;
    return retval;
}

float** zeros(int x, int y)
{
    float **retval = create_matrix(x, y);
    for (int j = 0; j < y; j++)
        for (int i = 0; i < x; i++) 
            retval[j][i] = 0.f;
    return retval;

}

float* ones(int size)
{
    float *retval = create_array(size);
    for (int i = 0; i < size; i++) retval[i] = 1.f;
    return retval;
}

void destroy_array(float *a)
{
    delete[] a;
}

void destroy_matrix(float **a)
{
    delete[] a[0];
    delete[] a; 
}

void rgb_hsv(float &rh, float &rs, float &rv, double r, double g, double b )
{
 
    // R, G, B values are divided by 255
    // to change the range from 0..255 to 0..1
    r = r / 255.0;
    g = g / 255.0;
    b = b / 255.0;

    //std::cout << "r: " << r << ", g: " << g << ", b: " << b << std::endl;
 
    // h, s, v = hue, saturation, value
    double cmax = fmax(r, fmax(g, b)); // maximum of r, g, b
    double cmin = fmin(r, fmin(g, b)); // minimum of r, g, b
    double diff = cmax - cmin; // diff of cmax and cmin.
    double h = -1, s = -1;
    //std::cout << "cmax: " << cmax << ", cmin: " << cmin << ", diff: " << diff << std::endl;
 
    // if cmax and cmax are equal then h = 0
    if (cmax == cmin)
        h = 0;
 
    // if cmax equal r then compute h
    else if (cmax == r){
        h = fmod(60 * ((g - b) / diff) + 360, 360);
        //std::cout << " cmax==r, g-b: " << g-b << ", g-b/diff: " << (g-b)/diff <<  std::endl;
    }
    // if cmax equal g then compute h
    else if (cmax == g)
        h = fmod(60 * ((b - r) / diff) + 120, 360);
 
    // if cmax equal b then compute h
    else if (cmax == b)
        h = fmod(60 * ((r - g) / diff) + 240, 360);
 
    // if cmax equal zero
    if (cmax == 0)
        s = 0;
    else
        s = (diff / cmax); // * 100;
 
    // compute v
    double v = cmax; // * 100;
    rh = (float)h;
    rs = (float)s;
    rv = (float)v;
    // std::cout << "(" << h << ", " << s << ", " << v << ")"
    //      << std::endl;
}


void hsv_rgb(int &rr, int &rg, int &rb, float H, float S,float V)
{
    if(H>360 || H<0 || S>1 || S<0 || V>1 || V<0){
        //std::cout<<"The given HSV values are not in valid range (0..1): "
        //    << "h: " << H << ", s: " << S << ", v: " << V  << std::endl;
        return;
    }
    float s = S; // /100;
    float v = V; // /100;
    float C = s*v;
    float X = C*(1-abs(fmod(H/60.0, 2)-1));
    float m = v-C;
    float r,g,b;
    if(H >= 0 && H < 60){
        r = C,g = X,b = 0;
    }
    else if(H >= 60 && H < 120){
        r = X,g = C,b = 0;
    }
    else if(H >= 120 && H < 180){
        r = 0,g = C,b = X;
    }
    else if(H >= 180 && H < 240){
        r = 0,g = X,b = C;
    }
    else if(H >= 240 && H < 300){
        r = X,g = 0,b = C;
    }
    else{
        r = C,g = 0,b = X;
    }
    // std::cout<< "r: "<< r << ", g: " << g << ", b: " << b << ", m: " << m << "\n";
    rr = (r+m)*255;
    rg = (g+m)*255;
    rb = (b+m)*255;
    
}

float hue(int r, int g, int b)
{
    float h, s, v;
    rgb_hsv(h, s, v, r, g, b);
    return h;
}

float hue(int *rgb)
{
    return hue(rgb[0], rgb[1], rgb[2]);
}

float saturation(int r, int g, int b)
{
    float h, s, v;
    rgb_hsv(h, s, v, r, g, b);
    return s;
}

float saturation(int *rgb)
{
    return saturation(rgb[0], rgb[1], rgb[2]);
}

void
print_matrix(const char * name, float ** m, int sizex, int sizey, int decimals)
{
    //bool matlab_format = decimals & MATLAB;
    //bool dim_format = decimals & DIM;
    decimals = decimals & 255;
    
    //if(matlab_format) // use MATLAB format
    //{
    //    decimals -= 1024;
    //    if(decimals == 0)
    //        decimals = 2;
    //    printf("%s = [", name);
    //    for (int j=0; j<sizey; j++)
    //    {
    //        for (int i=0; i < sizex; i++)
    //            printf(" %*.*f", 6+decimals, decimals, m[j][i]);
    //        if(j<sizey-1)
    //            printf(";...\n");
    //    }
    //    printf(" ];\n");
    //}
    //else
    //{
        //if(dim_format)
        //    printf("%s[%d][%d] = \n", name, sizey, sizex);
        //else
            printf("%s = \n", name);
        for (int j=0; j<sizey; j++)
        {
            for (int i=0; i < sizex; i++)
                printf("%*.*f", 6+decimals, decimals, m[j][i]);
            printf("\n");
        }
        printf("\n");
    //}
}

float map(float a, float from_min, float from_max, float to_min, float to_max){
    float from = (a - from_min)/(from_max - from_min);
    return from * (to_max - to_min) + to_min;
}

void print_array(float *a, int size) {
    for (int i=0; i < size; ++i)
        std::cout << a[i] << ", ";
    std::cout << "\n";
}

void print_array(std::string s, FloatList *l){
    std::cout << s;
    print_array(l->data(), l->size());
}

void println(){
    std::cout << "\n";
}

void println(std::string s){
    std::cout << s << "\n";
}

//
// Buffer
//
Buffer::Buffer(int sz){
    data.reserve(sz);
    for(int i=0; i<sz; i++)
        data.push_back(0);
}

FloatList Buffer::array(){
    return data;
}

float Buffer::head(){
    return data.at(0);
}

float Buffer::get(int ix){
    return data.at(ix);
}

void Buffer::append(float val){
    data.push_back(val);
    data.erase (data.begin());
}




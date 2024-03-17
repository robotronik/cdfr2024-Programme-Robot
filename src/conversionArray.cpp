#include "conversionArray.h"


// char: %c pour imprimer et %c pour numériser.
// signed char: %hhd pour imprimer et %hhd pour numériser.
// unsigned char: %hhu pour imprimer et %hhu pour numériser.
// short, short int, signed short, signed short int: %hd pour imprimer et %hd pour numériser.
// unsigned short, unsigned short int: %hu pour imprimer et %hu pour numériser.
// int, signed, signed int: %d pour imprimer et %d pour numériser.
// unsigned, unsigned int: %u pour imprimer et %u pour numériser.
// long, long int, signed long, signed long int: %ld pour imprimer et %ld pour numériser.
// unsigned long, unsigned long int: %lu pour imprimer et %lu pour numériser.
// long long, long long int, signed long long, signed long long int: %lld pour imprimer et %lld pour numériser.
// unsigned long long, unsigned long long int: %llu pour imprimer et %llu pour numériser.
// float: %f pour imprimer et %f pour numériser.
// double: %lf pour imprimer et %lf pour numériser.
// long double: %Lf pour imprimer et %Lf pour numériser.


template<typename T>
void getData(T& args, char* array,int countBiteArrayRead){
    args = 0;
    for (int i = 0; i < countBiteArrayRead; i++){
        args |= ((((T)array[i]) & 0xFF) << (8*(countBiteArrayRead-1-i)));
    }
}

template<typename T>
void getDataSinged(T& args, char* array,int countBiteArrayRead){
   getData<T>(args,array,countBiteArrayRead);
    //signed value convertion
    if(((args)>>((countBiteArrayRead*8-1)) & 0x1) == 1){
        args |= (-1 << countBiteArrayRead*8);
    }
}

int arrayToParameter(char* array, int size, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int PositionArray = 0;

    while (*format) {
         if(*format>='0' && *format<='9'){
            int count = *format - '0';
            format++;
            if(PositionArray+count>=size){
                return PositionArray;
            }
            //FIND %
            if(*format == '%') {
                format++;
                //FIND FORMAT
                if (strncmp(format, "c",1) == 0) {
                    getDataSinged<char>(*(char*) va_arg(args, char*),array,count);
                } else if (strncmp(format, "hhd",3) == 0) {
                    getDataSinged<signed char>(*(signed char*) va_arg(args, signed char*),array,count);
                } else if (strncmp(format, "hhu",3) == 0) {
                    getData<unsigned char>(*(unsigned char*) va_arg(args, unsigned char*),array,count);
                } else if (strncmp(format, "hd",3) == 0) {
                    getDataSinged<short>(*(short*) va_arg(args, short*),array,count);
                } else if (strncmp(format, "hu",3) == 0) {
                   getData<unsigned short>(*(unsigned short*) va_arg(args, unsigned short*),array,count);
                } else if (strncmp(format, "d",1) == 0) {
                   getDataSinged<int>(*(int*) va_arg(args, int*),array,count);
                } else if (strncmp(format, "u",1) == 0) {
                    getData<unsigned int>(*(unsigned int*) va_arg(args, unsigned int*),array,count);
                } else if (strncmp(format, "ld",2) == 0) {
                    getDataSinged<long>(*(long*) va_arg(args, long*),array,count);
                } else if (strncmp(format, "lu",2) == 0) {
                    getData<unsigned long>(*(unsigned long*) va_arg(args, unsigned long*),array,count);
                } else if (strncmp(format, "lld",3) == 0) {
                    getDataSinged<long long>(*(long long*) va_arg(args,long long*),array,count);
                } else if (strncmp(format, "llu",3) == 0) {
                    getData<unsigned long long>(*(unsigned long long*) va_arg(args, unsigned long long*),array,count);
                } else if (strncmp(format, "f",1) == 0) {
                    long vallong;
                    getData<long>(vallong,array,4);
                    *(float*) va_arg(args, float*) = *((float*)(&vallong));
                } else if (strncmp(format, "lf",2) == 0) {
                    long long vallong;
                    getData<long long>(vallong,array,8);
                    *(double*) va_arg(args, double*) = *((double*)(&vallong));
                } else if (strncmp(format, "Lf",2) == 0) {
                    //NOT SUPORTED
                }
                array = array + count;
                PositionArray += count;
            }
        }
        else{
            format++;
        }
    }
    va_end(args);
    return PositionArray;
}


template<typename T>
void setData(T args, char* array,int countBiteArrayRead){
    for (int i = 0; i < countBiteArrayRead; i++){
        array[i] = args >> (8*(countBiteArrayRead-1-i));
    }
}

int parameterToArray(char* array, int size, const char* format, ...) {
    va_list args;
    va_start(args, format);

    int PositionArray = 0;

    while (*format) {
        //FIND NUMBER
        if(*format>='0' && *format<='9'){
            int count = *format - '0';
            format++;
            if(PositionArray+count>=size){
                return PositionArray;
            }
            //FIND %
            if(*format == '%') {
                format++;
                //FIND FORMAT
                if (strncmp(format, "c",1) == 0) {
                    setData<char>(va_arg(args, int),array,count);
                } else if (strncmp(format, "hhd",3) == 0) {
                    setData<signed char>(va_arg(args, int),array,count);
                } else if (strncmp(format, "hhu",3) == 0) {
                    setData<unsigned char>(va_arg(args, int),array,count);
                } else if (strncmp(format, "hd",2) == 0) {
                    setData<short>(va_arg(args, int),array,count);
                } else if (strncmp(format, "hu",2) == 0) {
                    setData<unsigned short>(va_arg(args, int),array,count);
                } else if (strncmp(format, "d",1) == 0) {
                    setData<int>(va_arg(args, int),array,count);
                } else if (strncmp(format, "u",1) == 0) {
                    setData<unsigned int>(va_arg(args, unsigned int),array,count);
                } else if (strncmp(format, "ld",2) == 0) {
                    setData<long>(va_arg(args, long),array,count);
                } else if (strncmp(format, "lu",2) == 0) {
                    setData<unsigned long>(va_arg(args, unsigned long),array,count);
                } else if (strncmp(format, "lld",3) == 0) {
                    setData<long long>(va_arg(args, long long),array,count);
                } else if (strncmp(format, "llu",3) == 0) {
                    setData<unsigned long long>(va_arg(args, unsigned long long),array,count);
                } else if (strncmp(format, "f",1) == 0) {
                    float myFloat= (va_arg(args, double));
                    setData<long>((*((long*)&myFloat)),array,4);
                } else if (strncmp(format, "lf",2) == 0) {
                    double myFloat= (va_arg(args, double));
                    setData<long>((*((long long*)&myFloat)),array,8);
                } else if (strncmp(format, "Lf",2) == 0) {
                    //NOT SUPORTED
                }
                array = array + count;
                PositionArray += count;
            }
        }
        else{
            format++;
        }
    }
    va_end(args);
    return PositionArray;
}
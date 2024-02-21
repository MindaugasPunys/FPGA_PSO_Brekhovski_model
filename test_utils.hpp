#pragma open

#include "common_defs.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

class test_utils {
  private:
    std::string dir;

  public:
    test_utils() = default;
    ~test_utils() = default;

    void file_read_float(const char *filename, float *data, int size);
    void file_read_float(const char *filename, fixed_float *data, int size);
    void file_read_float(const char *filename, fixed_double *data, int size);
    void file_read_complex(const char *filename_re, const char *filename_im,
                           std::complex<fixed_double> *data, int size);

    void print_float(const char *name, float *data, int size);
    void print_float(const char *name, fixed_float *data, int size);
    void print_float(const char *name, fixed_double *data, int size);
    void print_complex(const char *name, std::complex<fixed_double> *data,
                       int size);
};

void test_utils::file_read_float(const char *filename, float *data, int size) {
    std::ifstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < size; i++) {
            if (file >> data[i]) {
                // Successfully read a float value from the file
            } else {
                printf("Failed to read float value from file: %s\n", filename);
                return;
            }
        }
        file.close();
    } else {
        printf("Failed to open file: %s\n", filename);
    }
}

void test_utils::file_read_float(const char *filename, fixed_float *data,
                                 int size) {
    std::ifstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < size; i++) {
            if (file >> data[i]) {
                // Successfully read a fixed_float value from the file
            } else {
                printf("Failed to read fixed_float value from file: %s\n",
                       filename);
                return;
            }
        }
        file.close();
    } else {
        printf("Failed to open file: %s\n", filename);
    }
}

void test_utils::file_read_float(const char *filename, fixed_double *data,
                                 int size) {
    std::ifstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < size; i++) {
            if (file >> data[i]) {
                // Successfully read a fixed_double value from the file
            } else {
                printf("Failed to read fixed_double value from file: %s\n",
                       filename);
                return;
            }
        }
        file.close();
    } else {
        printf("Failed to open file: %s\n", filename);
    }
}

void test_utils::file_read_complex(const char *filename_re,
                                   const char *filename_im,
                                   std::complex<fixed_double> *data, int size) {
    std::ifstream file_re(filename_re);
    std::ifstream file_im(filename_im);
    if (file_re.is_open() && file_im.is_open()) {
        for (int i = 0; i < size; i++) {
            fixed_double re, im;
            if ((file_re >> re) && (file_im >> im)) {
                data[i] = std::complex<fixed_double>(re, im);
            } else {
                printf("Failed to read complex value from files: %s, %s\n",
                       filename_re, filename_im);
                return;
            }
        }
        file_re.close();
        file_im.close();
    } else {
        printf("Failed to open files: %s, %s\n", filename_re, filename_im);
    }
}

void test_utils::print_float(const char *name, float *data, int size) {
	printf("========================================\n");
    printf("%s:\n", name);
    printf("========================================\n");
    for (int i = 0; i < size; i++) {
        printf("%f\n", data[i]);
    }
    printf("\n");
}
void test_utils::print_float(const char *name, fixed_float *data, int size) {
	printf("========================================\n");
    printf("%s:\n", name);
    printf("========================================\n");
    for (int i = 0; i < size; i++) {
        printf("%f\n", (float)data[i]);
    }
    printf("\n");
}
void test_utils::print_float(const char *name, fixed_double *data, int size) {
	printf("========================================\n");
    printf("%s:\n", name);
    printf("========================================\n");
    for (int i = 0; i < size; i++) {
        printf("%f\n", (float)data[i]);
    }
    printf("\n");
}
void test_utils::print_complex(const char *name,
                               std::complex<fixed_double> *data, int size) {
    printf("========================================\n");
    printf("%s:\n", name);
    printf("========================================\n");
    for (int i = 0; i < size; i++) {
        printf("%f + i%f\n", (float)data[i].real(), (float)data[i].imag());
    }
    printf("\n");
}

// Copyright 2020 Your Name <your_email>

#include <tester.hpp>
#include <iostream>

Tester::Tester(int L1, int L2):
    Lmin(L1), Lmax(L2), L1_size(64*1024 / sizeof(int)), tmp(0), count(0)
{
}

std::vector<int> Tester::define_series() {
  double start = Lmin * 0.5*1024/sizeof(int);
  double end = Lmax * 1.5*1024/sizeof(int);
  while (start < end){
    series.push_back(start);
    start = start*2;
  }
  series.push_back(end);
  return series;
}

int* Tester::get_random_array(int size) {
  int* array = new int [size];
  for (int i=0; i < size; ++i){
    array[i] = random();
  }
  return array;
}

void Tester::free_array(int* array){
  delete[] array;
}

int Tester::random_16(int zero, int array_size ) {
  int a = zero + random() % (array_size - zero + 1);
  if (a/step == 0){
    return a;
  } else{
    int b = a%step;
    a = a - b;
  }
  return a;
}

double Tester::direct(int array_size) {
  int* array = get_random_array(array_size);
  // warming up по размеру L1
  if (array_size < L1_size) {
    count = L1_size / array_size;
    for (int k = 0; k < count; ++k) {
      for (int i = 0; i < array_size; i += step) tmp = array[i];
    }
  } else {
    for (int i = 0; i < L1_size; i += step) tmp = array[i];
  }
  // test
  int elapsed_seconds = 0;
  for (int i = 0; i < iter; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < array_size; j += step) {
      tmp = array[j];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    elapsed_seconds +=
        std::chrono::duration_cast
            <std::chrono::microseconds>(finish - start).count();
  }
  free_array(array);
  tmp = 0;
  count = 0;
  return (static_cast<double> (elapsed_seconds) / iter);
}

double Tester::reverse(int array_size) {
  int* array = get_random_array(array_size);
  int elapsed_seconds = 0;
  // warming up по размеру L1
  if (array_size < L1_size) {
    count = L1_size / array_size;
    for (int k = 0; k < count; ++k) {
      for (int i = 0; i < array_size; i += step) tmp = array[i];
    }
  } else {
    for (int i = 0; i < L1_size; i += step) tmp = array[i];
  }
  // test
  for (int j = 0; j < iter; ++j) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = array_size - array_size % step - 1; i >= 0; i -= step) {
      tmp = array[i];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    elapsed_seconds =
        std::chrono::duration_cast
            <std::chrono::microseconds>(finish - start).count();
  }
  free_array(array);
  tmp = 0;
  count = 0;
  return (static_cast<double> (elapsed_seconds) / iter);
}

double Tester::randomn(int array_size) {
  int* array = get_random_array(array_size);
  // warming up по размеру L1
  if (array_size < L1_size) {
    count = L1_size / array_size;
    for (int k = 0; k < count; ++k) {
      for (int i = 0; i < array_size; i += step) tmp = array[i];
    }
  } else {
    for (int i = 0; i < L1_size; i += step) tmp = array[i];
  }
  // test
  int elapsed_seconds = 0;
  for (int i = 0; i < iter; ++i) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int j = 0; j < array_size; j += step) {
      tmp = array[random_16(0, array_size - 1)];
    }
    auto finish = std::chrono::high_resolution_clock::now();
    elapsed_seconds +=
        std::chrono::duration_cast
            <std::chrono::microseconds>(finish - start).count();
  }
  free_array(array);
  tmp = 0;
  count = 0;
  return (static_cast<double> (elapsed_seconds) / iter);
}

void Tester::experiment(std::ostream& out) {
  //direct
  out << "  investigation:" << std::endl << "    travel_order: Direction" <<\
 std::endl << "    experiments: " << std::endl;
  for (size_t i = 0; i < series.size(); ++i){
    out << "    - experiment:\n        number: " << i + 1
        << "\n        input_data:\n          buffer_size: " << series[i] / 256
        << "kb\n        results:\n         duration: "<< direct(series[i])
        << "mcs" << std::endl; }
  //reverse
  out << "  investigation:" << std::endl << "    travel_order: Reverse" <<\
 std::endl << "    experiments: " << std::endl;
  for (size_t i = 0; i < series.size(); ++i){
    out << "    - experiment:\n        number: " << i + 1
        << "\n        input_data:\n          buffer_size: " << series[i] / 256
        << "kb\n        results:\n         duration: "<< direct(series[i])
        << "mcs" << std::endl; }
  //random
  out << "  investigation:" << std::endl << "    travel_order: Random" <<\
 std::endl << "    experiments: " << std::endl;
  for (size_t i = 0; i < series.size(); ++i){
    out << "    - experiment:\n        number: " << i + 1
        << "\n        input_data:\n          buffer_size: " << series[i] / 256
        << "kb\n        results:\n         duration: "<< direct(series[i])
        << "mcs" << std::endl; }
}

Tester::~Tester()
{}

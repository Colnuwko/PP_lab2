#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include <fstream>
#include <string>
#include <locale.h>
#include <map>
#include <chrono>
#include <omp.h>
using namespace std;

void read_data(int** matrix, int size_matrix, string file_name) {

    ifstream f1(file_name); // окрываем файл для чтения
    for (int i = 0; i < size_matrix; i++)
    {
        for (int j = 0; j < size_matrix; j++)
        {
            f1 >> matrix[i][j];
            //cout << matrix[i][j] << " ";
        }
        //cout << endl;
    }
    f1.close();     // закрываем файл
}

void write_data(int x, string file_name) {
    std::cout << file_name;
    ofstream f;
    f.open(file_name);
    for (int i = 0; i < x * x; i++)
    {// или так
        f << rand() % 99 << " ";
    }
    f.close();
}

void write_result(int** matrix, string file_name, int matrix_size)
{
    ofstream f;
    f.open(file_name);
    for (int i = 0; i < matrix_size; i++)
    {
        for (int j = 0; j < matrix_size; j++)
        {
            f << matrix[i][j] << " ";
        }
    }
}
double matrix_multiplication(int** m_one, int** m_two, int size_matrix, int** result, int threads) {;
    omp_set_num_threads(threads);
    int i, j, k;
    auto start_time = std::chrono::steady_clock::now(); 
    #pragma omp parallel for shared(m_one, m_two, result) private(i, j, k)
    for (i = 0; i < size_matrix; i++)
    {
        for (j = 0; j < size_matrix; j++)
        {
            result[i][j] = 0;
            for (k = 0; k < size_matrix; k++)
            {

                result[i][j] += m_one[i][k] * m_two[k][j];
                //cout << result[i][j];
            }
            //cout << m_two[i][j] << "\t";
        }
        //std::cout << endl;
    }

    auto end_time = std::chrono::steady_clock::now();
    auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
    double time = (double)elapsed_ns.count() / 1000000000;

    return time;
}

int main() {
    setlocale(LC_ALL, "Russian");

        //cout << "Введите размерность матриц\n";
        //int size_matrix;
        //cin >> size_matrix;

        //write_data(size_matrix, file_name_first_matrix);
        //write_data(size_matrix, file_name_second_matrix);

        //int** matrix_first = new int* [size_matrix];
        //for (int i = 0; i < size_matrix; i++)
        //{
       //    matrix_first[i] = new int[size_matrix];
       // }

       // int** matrix_second = matrix_first;

        //read_data(matrix_first, size_matrix, file_name_first_matrix);
        //read_data(matrix_second, size_matrix, file_name_second_matrix);

        //int** result = matrix_first;
       std::cout << "\nВот собственно матрица )))\n";
 
       std::cout << "\nВведите число потоков от 0 до "<< omp_get_max_threads()<<"\n";
       int threads;
       std::cin >> threads;
       //auto time = matrix_multiplication(matrix_first, matrix_second, size_matrix, result);

        //cout << time;
       
       ofstream file;
        file.open("stats.csv", ios_base::app);
        for (int size_matrix = 100; size_matrix <= 2000; size_matrix += 100)
        {
            for (int k = 0; k < 10; k++) {
                string file_name_first_matrix{ to_string(size_matrix) + ".txt" };
                string file_name_second_matrix{ to_string(size_matrix) + "_2" + ".txt" };
                string file_name_result{ "result_" + to_string(size_matrix) + ".txt" };
                write_data(size_matrix, file_name_first_matrix);
                write_data(size_matrix, file_name_second_matrix);
                int** matrix_first = new int* [size_matrix];
                for (int i = 0; i < size_matrix; i++)
                {
                    matrix_first[i] = new int[size_matrix];
                }
                int** matrix_second = new int* [size_matrix];
                for (int i = 0; i < size_matrix; i++)
                {
                    matrix_second[i] = new int[size_matrix];
                }
                int** result = new int* [size_matrix];
                for (int i = 0; i < size_matrix; i++)
                {
                    result[i] = new int[size_matrix];
                }

                read_data(matrix_first, size_matrix, file_name_first_matrix);
                read_data(matrix_second, size_matrix, file_name_second_matrix);

                auto time = matrix_multiplication(matrix_first, matrix_second, size_matrix, result, threads);
                write_result(result, file_name_result, size_matrix);
                file << threads << size_matrix << "\t" << time << endl;
                if (size_matrix % 500 != 0)
                {
                    remove(file_name_first_matrix.c_str());
                    remove(file_name_second_matrix.c_str());
                    remove(file_name_result.c_str());
                }
            }

        }
        file << endl;
        file.close();
        
       /*
        auto start_time = std::chrono::steady_clock::now();
        for (int i = 0; i < 10000000000; i++) {
            
        }
        auto end_time = std::chrono::steady_clock::now();
        auto elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        double time = (double)elapsed_ns.count() / 1000000000;
        std::cout << "\nNo parallel " << time;  
        start_time = std::chrono::steady_clock::now();
        #pragma omp parallel for
        for (int i = 0; i < 10000000000; i++) {
            
        }
        end_time = std::chrono::steady_clock::now();
        elapsed_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        time = (double)elapsed_ns.count() / 1000000000;
        std::cout << "\nWith parallel " << time;
        */
    std::cout << "\ncomplete";
    std::cout << "processors" << omp_get_num_procs();
    std::cout << "threads" << omp_get_max_threads();
    return 0;

}
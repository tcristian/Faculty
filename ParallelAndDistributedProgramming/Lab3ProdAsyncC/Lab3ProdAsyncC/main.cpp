//
//  main.cpp
//  Lab3SumAsyncC
//
//  Created by Noane Dan on 28/10/2017.
//  Copyright © 2017 Noane Dan. All rights reserved.
//

#include <future>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

class Matrix {
    
private:
    int **matrix;
    int rows;
    int cols;
    
public:
    Matrix(int rows, int cols) {
        this->rows = rows;
        this->cols = cols;
        
        this->matrix = new int*[rows];
        for (int i = 0; i < rows; ++i)
            this->matrix[i] = new int[cols];
    }
    
    int getRows() {
        return this->rows;
    }
    
    int getCols() {
        return this->cols;
    }
    
    int size() {
        return this->rows * this->cols;
    }
    
    int get(int row, int col) {
        return this->matrix[row][col];
    }
    
    void set(int row, int col, int val) {
        this->matrix[row][col] = val;
    }
    
    void randInit() {
        
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->cols; ++j) {
                this->matrix[i][j] = rand() % 20;
            }
        }
    }
    
    void print() {
        
        for (int i = 0; i < this->rows; ++i) {
            for (int j = 0; j < this->cols; ++j) {
                std::cout << this->matrix[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }
};

class ComputedValue {
    
private:
    int row;
    int column;
    int value;
    
public:
    ComputedValue(int row, int column, int value) {
        this->row = row;
        this->column = column;
        this->value = value;
    }
    
    int getRow() {
        return this->row;
    }
    
    int getColumn() {
        return this->column;
    }
    
    int getValue() {
        return this->value;
    }
};

ComputedValue* prod(Matrix *a, Matrix *b, int row, int col) {
    
    int value = 0;
    for (int x = 0; x < a->getCols(); ++x) {
        value += a->get(row, x) * b->get(x, col);
    }
    
    return new ComputedValue(row, col, value);
}

int main(int argc, const char * argv[]) {
    
    srand((unsigned int) time(0));
    
    int n = 100;
    int m = 100;
    int p = 100;
    
    Matrix *a, *b, *c;
    a = new Matrix(n, m);
    b = new Matrix(m, p);
    c = new Matrix(n, p);
    
    a->randInit();
    b->randInit();

    std::vector<std::future<ComputedValue*>> jobs;
    
    auto started = std::chrono::high_resolution_clock::now();
    for (int row = 0; row < c->getRows(); ++row) {
        for (int col = 0; col < c->getCols(); ++col) {
            jobs.push_back(std::async(prod, a, b, row, col));
        }
    }
    
    ComputedValue* computedValue;
    for (int i = 0; i < jobs.size(); ++i) {
        computedValue = jobs[i].get();
        c->set(computedValue->getRow(), computedValue->getColumn(), computedValue->getValue());
    }
    auto done = std::chrono::high_resolution_clock::now();
    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count() << " ms to complete" << std::endl;
    
    //    a->print();
    //    b->print();
    //    c->print();
    
    return 0;
}

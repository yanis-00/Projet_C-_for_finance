#pragma once

#include <vector>
#include <iostream>
#include <iomanip>
#include <stdexcept>

template <typename T>
class BinaryTree {
private:
    int _depth;
    std::vector<std::vector<T>> _tree;

public:
    BinaryTree() : _depth(0) {}

    void setDepth(int depth) {
        if (depth < 0) {
            throw std::invalid_argument("Depth must be non-negative.");
        }
        _depth = depth;
        _tree.resize(_depth + 1);
        for (int n = 0; n <= _depth; ++n) {
            _tree[n].resize(n + 1);
        }
    }

    void setNode(int n, int i, T value) {
        if (n < 0 || n > _depth || i < 0 || i > n) {
            throw std::out_of_range("Node indices out of range.");
        }
        _tree[n][i] = value;
    }

    T getNode(int n, int i) const {
        if (n < 0 || n > _depth || i < 0 || i > n) {
            throw std::out_of_range("Node indices out of range.");
        }
        return _tree[n][i];
    }

    void display() const {
        // 1. Affichage triangulaire classique (aligné à gauche)
        for (int n = 0; n <= _depth; ++n) {
            for (int i = 0; i <= n; ++i) {
                std::cout << _tree[n][i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        // 2. Affichage pyramidal avec branches (/ et \) comme sur la Figure 2
        for (int n = 0; n <= _depth; ++n) {
            std::cout << std::string((_depth - n) * 3, ' ');
            for (int i = 0; i <= n; ++i) {
                std::cout << std::setw(3) << _tree[n][i];
                if (i < n) {
                    std::cout << "   ";
                }
            }
            std::cout << std::endl;

            if (n < _depth) {
                std::cout << std::string((_depth - n) * 3 - 1, ' ');
                for (int i = 0; i <= n; ++i) {
                    std::cout << "/   \\";
                    if (i < n) {
                        std::cout << " ";
                    }
                }
                std::cout << std::endl;
            }
        }
    }
};
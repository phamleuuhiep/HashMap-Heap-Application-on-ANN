/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt
 * to change this license Click
 * nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this
 * template
 */

/*
 * File:   Softmax.cpp
 * Author: ltsach
 *
 * Created on August 25, 2024, 2:46 PM
 */

#include "layer/Softmax.h"

#include <filesystem>  //require C++17

#include "ann/functions.h"
#include "sformat/fmt_lib.h"
namespace fs = std::filesystem;

Softmax::Softmax(int axis, string name) : m_nAxis(axis) {
  if (trim(name).size() != 0)
    m_sName = name;
  else
    m_sName = "Softmax_" + to_string(++m_unLayer_idx);
}

Softmax::Softmax(const Softmax& orig) {}

Softmax::~Softmax() {}

xt::xarray<double> Softmax::forward(xt::xarray<double> X) {
    m_aCached_Y = softmax(X, m_nAxis);  
    return m_aCached_Y;
}
xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
    // xt::xarray<double> outer_product = xt::linalg::outer(m_aCached_Y, xt::transpose(m_aCached_Y));
    xt::xarray<double> outer_product = outer_stack(m_aCached_Y, m_aCached_Y);
    // xt::xarray<double> diag_y = xt::diag(m_aCached_Y);
    xt::xarray<double> diag_y = diag_stack(m_aCached_Y);
    xt::xarray<double> jacobian = diag_y - outer_product;
    // return xt::linalg::dot(jacobian, DY);
    return matmul_on_stack(jacobian, DY);
} 
// xt::xarray<double> Softmax::backward(xt::xarray<double> DY) {
//     size_t row = DY.shape()[0];
//     size_t output_size = DY.shape()[1];
//     xt::xarray<double> grad = xt::zeros_like(DY);

//     for (int i = 0; i < row; i++) {
//         xt::xarray<double> dy_i = xt::view(DY, i, xt::all());
//         xt::xarray<double> y_i = xt::view(m_aCached_Y, i, xt::all());
//         xt::xarray<double> diag_y = xt::diag(y_i);
//         // xt::xarray<double> outer_y = xt::linalg::outer(y_i, y_i); 
//         xt::xarray<double> outer_y = xt::linalg::outer(y_i, xt::transpose(y_i)); 
//         xt::xarray<double> jacobian = diag_y - outer_y;

//         xt::view(grad, i, xt::all()) = xt::linalg::dot(jacobian, dy_i);
//     }
//     return grad;
// }




string Softmax::get_desc() {
  string desc = fmt::format("{:<10s}, {:<15s}: {:4d}", "Softmax",
                            this->getname(), m_nAxis);
  return desc;
}
